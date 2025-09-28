#include "estructuras.h"
#include <signal.h>
#include <pthread.h>

// Variables globales del servidor
struct sala_chat salas[MAX_SALAS];
int num_salas = 0;
int cola_global = -1;
pthread_mutex_t mutex_salas = PTHREAD_MUTEX_INITIALIZER;

// Función para limpieza al recibir señal
void cleanup_handler(int sig) {
    printf("\nRecibida señal %d. Cerrando servidor...\n", sig);
    
    // Eliminar todas las colas de salas
    pthread_mutex_lock(&mutex_salas);
    for (int i = 0; i < num_salas; i++) {
        if (msgctl(salas[i].cola_id, IPC_RMID, NULL) == -1) {
            perror("Error al eliminar cola de sala");
        } else {
            printf("Cola de sala '%s' eliminada\n", salas[i].nombre);
        }
    }
    pthread_mutex_unlock(&mutex_salas);
    
    // Eliminar cola global
    if (cola_global != -1) {
        if (msgctl(cola_global, IPC_RMID, NULL) == -1) {
            perror("Error al eliminar cola global");
        } else {
            printf("Cola global eliminada\n");
        }
    }
    
    printf("Servidor cerrado correctamente.\n");
    exit(0);
}

// Función para crear una nueva sala con esquema de claves seguro
int crear_sala(const char *nombre) {
    pthread_mutex_lock(&mutex_salas);
    
    if (num_salas >= MAX_SALAS) {
        pthread_mutex_unlock(&mutex_salas);
        return -1; // No se pueden crear más salas
    }

    // Generar clave segura usando ftok
    key_t key = obtener_clave_sala(nombre);
    if (key == -1) {
        pthread_mutex_unlock(&mutex_salas);
        return -1;
    }

    int cola_id = msgget(key, IPC_CREAT | 0666);
    if (cola_id == -1) {
        perror("Error al crear la cola de la sala");
        pthread_mutex_unlock(&mutex_salas);
        return -1;
    }

    // Inicializar la sala
    strcpy(salas[num_salas].nombre, nombre);
    salas[num_salas].cola_id = cola_id;
    salas[num_salas].num_clientes = 0;

    printf("Nueva sala creada: '%s' con cola ID %d\n", nombre, cola_id);
    
    int indice = num_salas;
    num_salas++;
    
    pthread_mutex_unlock(&mutex_salas);
    return indice; // Retornar el índice de la sala creada
}

// Función para buscar una sala por nombre (thread-safe)
int buscar_sala(const char *nombre) {
    pthread_mutex_lock(&mutex_salas);
    for (int i = 0; i < num_salas; i++) {
        if (strcmp(salas[i].nombre, nombre) == 0) {
            pthread_mutex_unlock(&mutex_salas);
            return i;
        }
    }
    pthread_mutex_unlock(&mutex_salas);
    return -1; // No encontrada
}

// Función para agregar un usuario a una sala (thread-safe)
int agregar_usuario_a_sala(int indice_sala, const char *nombre_usuario, pid_t pid) {
    pthread_mutex_lock(&mutex_salas);
    
    if (indice_sala < 0 || indice_sala >= num_salas) {
        pthread_mutex_unlock(&mutex_salas);
        return -1;
    }

    struct sala_chat *s = &salas[indice_sala];
    if (s->num_clientes >= MAX_CLIENTES) {
        pthread_mutex_unlock(&mutex_salas);
        return -2; // Sala llena
    }

    // Verificar si el usuario ya está en la sala
    for (int i = 0; i < s->num_clientes; i++) {
        if (strcmp(s->clientes[i].nombre, nombre_usuario) == 0) {
            pthread_mutex_unlock(&mutex_salas);
            return -3; // Usuario ya está en la sala
        }
    }

    // Agregar el usuario
    strcpy(s->clientes[s->num_clientes].nombre, nombre_usuario);
    s->clientes[s->num_clientes].pid = pid;
    strcpy(s->clientes[s->num_clientes].sala_actual, s->nombre);
    s->num_clientes++;
    
    printf("Usuario '%s' (PID: %d) agregado a sala '%s' (%d/%d)\n", 
           nombre_usuario, pid, s->nombre, s->num_clientes, MAX_CLIENTES);
    
    pthread_mutex_unlock(&mutex_salas);
    return 0;
}

// Función para remover usuario de una sala
int remover_usuario_de_sala(int indice_sala, const char *nombre_usuario) {
    pthread_mutex_lock(&mutex_salas);
    
    if (indice_sala < 0 || indice_sala >= num_salas) {
        pthread_mutex_unlock(&mutex_salas);
        return -1;
    }

    struct sala_chat *s = &salas[indice_sala];
    for (int i = 0; i < s->num_clientes; i++) {
        if (strcmp(s->clientes[i].nombre, nombre_usuario) == 0) {
            // Mover el último cliente a esta posición
            if (i < s->num_clientes - 1) {
                s->clientes[i] = s->clientes[s->num_clientes - 1];
            }
            s->num_clientes--;
            printf("Usuario '%s' removido de sala '%s' (%d/%d)\n", 
                   nombre_usuario, s->nombre, s->num_clientes, MAX_CLIENTES);
            pthread_mutex_unlock(&mutex_salas);
            return 0;
        }
    }
    
    pthread_mutex_unlock(&mutex_salas);
    return -1; // Usuario no encontrado
}

// Función para enviar un mensaje a todos los usuarios de una sala (excepto al remitente)
void enviar_a_todos_en_sala(int indice_sala, struct mensaje *msg) {
    pthread_mutex_lock(&mutex_salas);
    
    if (indice_sala < 0 || indice_sala >= num_salas) {
        pthread_mutex_unlock(&mutex_salas);
        return;
    }

    struct sala_chat *s = &salas[indice_sala];
    
    // Enviar a todos los usuarios en la sala
    for (int i = 0; i < s->num_clientes; i++) {
        // No enviar al remitente del mensaje original
        if (strcmp(s->clientes[i].nombre, msg->remitente) != 0) {
            // Establecer mtype para que cada cliente pueda recibir sus mensajes
            msg->mtype = MSG_CHAT;
            if (msgsnd(s->cola_id, msg, sizeof(struct mensaje) - sizeof(long), IPC_NOWAIT) == -1) {
                perror("Error al enviar mensaje a la sala");
            }
        }
    }
    
    pthread_mutex_unlock(&mutex_salas);
}

// Función para obtener lista de usuarios en una sala
void obtener_usuarios_sala(int indice_sala, char *buffer, int buffer_size) {
    (void)buffer_size; // Suprimir warning de parámetro no usado
    
    pthread_mutex_lock(&mutex_salas);
    
    if (indice_sala < 0 || indice_sala >= num_salas) {
        strcpy(buffer, "ERROR: Sala no existe");
        pthread_mutex_unlock(&mutex_salas);
        return;
    }

    struct sala_chat *s = &salas[indice_sala];
    strcpy(buffer, "Usuarios en sala: ");
    
    for (int i = 0; i < s->num_clientes; i++) {
        strcat(buffer, s->clientes[i].nombre);
        if (i < s->num_clientes - 1) {
            strcat(buffer, ", ");
        }
    }
    
    if (s->num_clientes == 0) {
        strcpy(buffer, "No hay usuarios en la sala");
    }
    
    pthread_mutex_unlock(&mutex_salas);
}

    int main() {
    // Configurar manejadores de señales
    signal(SIGINT, cleanup_handler);
    signal(SIGTERM, cleanup_handler);
    
    // Crear la cola global para solicitudes de clientes
    key_t key_global = obtener_clave_global();
    if (key_global == -1) {
        fprintf(stderr, "Error al generar clave global\n");
        exit(1);
    }
    
    cola_global = msgget(key_global, IPC_CREAT | 0666);
    if (cola_global == -1) {
        perror("Error al crear la cola global");
        exit(1);
    }

    printf("=== SERVIDOR DE CHAT INICIADO ===\n");
    printf("Cola global ID: %d\n", cola_global);
    
    // Crear salas por defecto como en el ejemplo
    crear_sala("General");
    crear_sala("Deportes");
    
    printf("Esperando clientes... (Ctrl+C para salir)\n\n");

    struct mensaje msg;
    struct mensaje respuesta;

    while (1) {
        // Recibir mensajes de la cola global
        if (msgrcv(cola_global, &msg, sizeof(struct mensaje) - sizeof(long), 0, 0) == -1) {
            perror("Error al recibir mensaje");
            continue;
        }

        printf("[%s] %s -> %s: %s\n", 
               obtener_nombre_tipo_mensaje(msg.mtype), 
               msg.remitente, msg.sala, msg.texto);

        switch (msg.mtype) {
            case MSG_JOIN: {
                // Solicitud de unirse a una sala
                int indice_sala = buscar_sala(msg.sala);
                if (indice_sala == -1) {
                    // Crear nueva sala
                    indice_sala = crear_sala(msg.sala);
                    if (indice_sala == -1) {
                        crear_mensaje_error(&respuesta, "No se pueden crear más salas");
                        break;
                    }
                }

                // Agregar usuario a la sala
                int resultado = agregar_usuario_a_sala(indice_sala, msg.remitente, getpid());
                if (resultado == 0) {
                    // Éxito - enviar JOIN_ACK con info de cola
                    char texto[MAX_TEXTO];
                    snprintf(texto, sizeof(texto), "Bienvenido a la sala '%s'", msg.sala);
                    crear_mensaje_join_ack(&respuesta, texto, salas[indice_sala].cola_id, 0);
                } else if (resultado == -2) {
                    crear_mensaje_error(&respuesta, RESP_ERROR_SALA_LLENA);
                } else if (resultado == -3) {
                    crear_mensaje_error(&respuesta, RESP_ERROR_USUARIO_DUPLICADO);
                } else {
                    crear_mensaje_error(&respuesta, "Error al unirse a la sala");
                }
                break;
            }

            case MSG_CHAT: {
                // Mensaje de chat - reenviar a todos en la sala
                int indice_sala = buscar_sala(msg.sala);
                if (indice_sala != -1) {
                    // Reenviar el mensaje a todos los usuarios en la cola de la sala
                    pthread_mutex_lock(&mutex_salas);
                    struct sala_chat *s = &salas[indice_sala];
                    
                    // Enviar a todos los usuarios en la sala (incluyendo el remitente para que aparezca en su terminal)
                    for (int i = 0; i < s->num_clientes; i++) {
                        struct mensaje msg_sala = msg;  // Copiar mensaje
                        msg_sala.mtype = MSG_CHAT;
                        
                        if (msgsnd(s->cola_id, &msg_sala, sizeof(struct mensaje) - sizeof(long), IPC_NOWAIT) == -1) {
                            perror("Error al enviar mensaje a la sala");
                        }
                    }
                    pthread_mutex_unlock(&mutex_salas);
                    continue; // No enviar respuesta al cliente para mensajes de chat
                } else {
                    crear_mensaje_error(&respuesta, RESP_ERROR_SALA_NO_EXISTE);
                }
                break;
            }

            case MSG_LEAVE: {
                // Abandonar sala
                int indice_sala = buscar_sala(msg.sala);
                if (indice_sala != -1) {
                    if (remover_usuario_de_sala(indice_sala, msg.remitente) == 0) {
                        char texto[MAX_TEXTO];
                        snprintf(texto, sizeof(texto), "Has abandonado la sala '%s'", msg.sala);
                        crear_mensaje_join_ack(&respuesta, texto, -1, 0);
                    } else {
                        crear_mensaje_error(&respuesta, "No estás en esa sala");
                    }
                } else {
                    crear_mensaje_error(&respuesta, RESP_ERROR_SALA_NO_EXISTE);
                }
                break;
            }

            case MSG_LIST: {
                // Listar salas disponibles
                char lista[MAX_TEXTO] = "";
                pthread_mutex_lock(&mutex_salas);
                if (num_salas == 0) {
                    strcpy(lista, "No hay salas disponibles");
                } else {
                    strcpy(lista, "Salas disponibles: ");
                    for (int i = 0; i < num_salas; i++) {
                        strcat(lista, salas[i].nombre);
                        char info[50];
                        snprintf(info, sizeof(info), " (%d/%d)", salas[i].num_clientes, MAX_CLIENTES);
                        strcat(lista, info);
                        if (i < num_salas - 1) strcat(lista, ", ");
                    }
                }
                pthread_mutex_unlock(&mutex_salas);
                crear_mensaje_join_ack(&respuesta, lista, -1, 0);
                break;
            }

            case MSG_WHO: {
                // Listar usuarios en sala
                int indice_sala = buscar_sala(msg.sala);
                if (indice_sala != -1) {
                    char usuarios[MAX_TEXTO];
                    obtener_usuarios_sala(indice_sala, usuarios, sizeof(usuarios));
                    crear_mensaje_join_ack(&respuesta, usuarios, -1, 0);
                } else {
                    crear_mensaje_error(&respuesta, RESP_ERROR_SALA_NO_EXISTE);
                }
                break;
            }

            case MSG_DISCONNECT: {
                // Desconectar usuario
                if (strlen(msg.sala) > 0) {
                    int indice_sala = buscar_sala(msg.sala);
                    if (indice_sala != -1) {
                        remover_usuario_de_sala(indice_sala, msg.remitente);
                    }
                }
                printf("Usuario %s desconectado\n", msg.remitente);
                continue; // No responder
            }

            default:
                printf("Tipo de mensaje desconocido: %ld\n", msg.mtype);
                continue;
        }

        // Enviar respuesta (excepto para CHAT y DISCONNECT)
        if (msgsnd(cola_global, &respuesta, sizeof(struct mensaje) - sizeof(long), IPC_NOWAIT) == -1) {
            perror("Error al enviar respuesta");
        }
    }

    return 0;
}