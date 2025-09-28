#include <pthread.h>
#include <signal.h>
#include <time.h>  // Para nanosleep
#include <unistd.h>
#include "estructuras.h"

// Variables globales del cliente
int cola_global;
int cola_sala = -1;
char nombre_usuario[MAX_NOMBRE];
char sala_actual[MAX_NOMBRE] = "";
pthread_t hilo_receptor;
int cliente_activo = 1;

// Función de limpieza al recibir señal
void cleanup_cliente(int sig) {
    (void)sig; // Suprimir warning
    printf("\n¡Hasta luego, %s!\n", nombre_usuario);
    cliente_activo = 0;
    
    // Enviar mensaje de desconexión si está en una sala
    if (strlen(sala_actual) > 0) {
        struct mensaje msg;
        crear_mensaje_disconnect(&msg, nombre_usuario, sala_actual);
        msgsnd(cola_global, &msg, sizeof(struct mensaje) - sizeof(long), IPC_NOWAIT);
    }
    
    // Cancelar hilo receptor
    if (pthread_cancel(hilo_receptor) == 0) {
        pthread_join(hilo_receptor, NULL);
    }
    
    exit(0);
}

// Función para el hilo receptor (sin busy-wait)
void *recibir_mensajes(void *arg) {
    (void)arg; // Suprimir warning
    struct mensaje msg;
    
    while (cliente_activo) {
        if (cola_sala != -1) {
            // Usar msgrcv bloqueante 
            if (msgrcv(cola_sala, &msg, sizeof(struct mensaje) - sizeof(long), MSG_CHAT, 0) > 0) {
                // Solo mostrar mensajes de otros usuarios (no los propios)
                if (strcmp(msg.remitente, nombre_usuario) != 0) {
                    printf("%s: %s\n", msg.remitente, msg.texto);
                    fflush(stdout);
                }
            }
        } else {
            // Si no hay sala activa, esperar un poco
            struct timespec ts = {0, 100000000}; // 100ms
            nanosleep(&ts, NULL);
        }
    }
    
    return NULL;
}

// Función para esperar respuesta del servidor
int esperar_respuesta(struct mensaje *respuesta) {
    // Esperar respuesta JOIN_ACK o ERROR
    if (msgrcv(cola_global, respuesta, sizeof(struct mensaje) - sizeof(long), 0, 0) == -1) {
        perror("Error al recibir respuesta del servidor");
        return -1;
    }
    
    if (respuesta->mtype == MSG_ERROR) {
        printf("Error: %s\n", respuesta->texto);
        return -1;
    }
    
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <nombre_usuario>\n", argv[0]);
        printf("Ejemplo: %s María\n", argv[0]);
        exit(1);
    }

    strcpy(nombre_usuario, argv[1]);

    // Configurar manejador de señales
    signal(SIGINT, cleanup_cliente);
    signal(SIGTERM, cleanup_cliente);

    // Conectarse a la cola global usando ftok seguro
    key_t key_global = obtener_clave_global();
    if (key_global == -1) {
        fprintf(stderr, "Error al generar clave global\n");
        exit(1);
    }
    
    cola_global = msgget(key_global, 0666);
    if (cola_global == -1) {
        perror("Error: No se pudo conectar al servidor. ¿Está ejecutándose?");
        exit(1);
    }

    printf("Bienvenido, %s. Salas disponibles: General, Deportes\n", nombre_usuario);

    // Crear hilo receptor
    if (pthread_create(&hilo_receptor, NULL, recibir_mensajes, NULL) != 0) {
        perror("Error al crear hilo receptor");
        exit(1);
    }

    struct mensaje msg;
    char comando[MAX_TEXTO];
    char parametro[MAX_NOMBRE];

    // Bucle principal de comandos
    while (cliente_activo) {
        printf("> ");
        fflush(stdout);
        
        if (!fgets(comando, MAX_TEXTO, stdin)) {
            break; // EOF o error
        }
        
        comando[strcspn(comando, "\n")] = '\0'; // Eliminar salto de línea
        
        // Parsear comando
        if (strncmp(comando, "join ", 5) == 0) {
            if (sscanf(comando, "join %s", parametro) != 1) {
                printf("Uso: join <nombre_sala>\n");
                continue;
            }
            
            // Abandonar sala actual si existe
            if (strlen(sala_actual) > 0) {
                crear_mensaje_leave_room(&msg, nombre_usuario, sala_actual);
                msgsnd(cola_global, &msg, sizeof(struct mensaje) - sizeof(long), IPC_NOWAIT);
                cola_sala = -1;
                strcpy(sala_actual, "");
            }
            
            // Unirse a nueva sala
            crear_mensaje_join(&msg, nombre_usuario, parametro);
            if (msgsnd(cola_global, &msg, sizeof(struct mensaje) - sizeof(long), 0) == -1) {
                perror("Error al enviar solicitud JOIN");
                continue;
            }
            
            struct mensaje respuesta;
            if (esperar_respuesta(&respuesta) == 0) {
                printf("Te has unido a la sala: %s\n", parametro);
                if (respuesta.mtype == MSG_JOIN_ACK && respuesta.codigo_respuesta == 0) {
                    // Conectar a cola de sala
                    key_t key_sala = obtener_clave_sala(parametro);
                    cola_sala = msgget(key_sala, 0666);
                    if (cola_sala != -1) {
                        strcpy(sala_actual, parametro);
                    }
                }
            }
            
        } else if (strcmp(comando, "list") == 0) {
            crear_mensaje_list_rooms(&msg, nombre_usuario);
            if (msgsnd(cola_global, &msg, sizeof(struct mensaje) - sizeof(long), 0) == -1) {
                perror("Error al solicitar lista");
                continue;
            }
            
            struct mensaje respuesta;
            if (esperar_respuesta(&respuesta) == 0) {
                printf("%s\n", respuesta.texto);
            }
            
        } else if (strcmp(comando, "who") == 0) {
            if (strlen(sala_actual) == 0) {
                printf("No estás en ninguna sala.\n");
                continue;
            }
            
            crear_mensaje_who(&msg, nombre_usuario, sala_actual);
            if (msgsnd(cola_global, &msg, sizeof(struct mensaje) - sizeof(long), 0) == -1) {
                perror("Error al solicitar usuarios");
                continue;
            }
            
            struct mensaje respuesta;
            if (esperar_respuesta(&respuesta) == 0) {
                printf("%s\n", respuesta.texto);
            }
            
        } else if (strcmp(comando, "leave") == 0) {
            if (strlen(sala_actual) == 0) {
                printf("No estás en ninguna sala.\n");
                continue;
            }
            
            crear_mensaje_leave_room(&msg, nombre_usuario, sala_actual);
            if (msgsnd(cola_global, &msg, sizeof(struct mensaje) - sizeof(long), 0) == -1) {
                perror("Error al abandonar sala");
                continue;
            }
            
            struct mensaje respuesta;
            if (esperar_respuesta(&respuesta) == 0) {
                printf("%s\n", respuesta.texto);
                strcpy(sala_actual, "");
                cola_sala = -1;
            }
            
        } else if (strcmp(comando, "help") == 0) {
            printf("Comandos disponibles:\n");
            printf("  join <sala>  - Unirse a una sala (ej: join General)\n");
            printf("  list         - Ver salas disponibles\n");
            printf("  who          - Ver usuarios en sala actual\n");
            printf("  leave        - Abandonar sala actual\n");
            printf("  help         - Mostrar esta ayuda\n");
            printf("  exit         - Salir del chat\n");
            printf("  <mensaje>    - Enviar mensaje a la sala actual\n");
            
        } else if (strcmp(comando, "exit") == 0 || strcmp(comando, "quit") == 0) {
            break;
            
        } else if (strlen(comando) > 0) {
            // Enviar mensaje a la sala
            if (strlen(sala_actual) == 0) {
                printf("Debes unirte a una sala primero. Usa: join <sala>\n");
                continue;
            }
            
            // Validar longitud del mensaje
            if (strlen(comando) > MAX_TEXTO - 1) {
                printf("Mensaje demasiado largo (máximo %d caracteres).\n", MAX_TEXTO - 1);
                continue;
            }
            
            // Mostrar el mensaje propio inmediatamente (como en el ejemplo)
            printf("%s\n", comando);
            
            crear_mensaje_chat(&msg, nombre_usuario, sala_actual, comando);
            if (msgsnd(cola_global, &msg, sizeof(struct mensaje) - sizeof(long), 0) == -1) {
                perror("Error al enviar mensaje");
                continue;
            }
        }
    }
    
    // Limpieza al salir
    cleanup_cliente(0);
    return 0;
}