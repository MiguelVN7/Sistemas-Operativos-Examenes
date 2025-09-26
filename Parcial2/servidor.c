#include "estructuras.h"

#define MAX_USUARIOS_POR_SALA 20

struct sala_chat salas[MAX_SALAS];
int num_salas = 0;

// Función para crear una nueva sala
int crear_sala(const char *nombre) {
    if (num_salas >= MAX_SALAS) {
        return -1; // No se pueden crear más salas
    }

    // Crear una cola de mensajes para la sala
    key_t key = ftok("/tmp", num_salas + 1); // Generar una clave única
    int cola_id = msgget(key, IPC_CREAT | 0666);
    if (cola_id == -1) {
        perror("Error al crear la cola de la sala");
        return -1;
    }

    // Inicializar la sala
    strcpy(salas[num_salas].nombre, nombre);
    salas[num_salas].cola_id = cola_id;
    salas[num_salas].num_clientes = 0;

    num_salas++;
    return num_salas - 1; // Retornar el índice de la sala creada
}

// Función para buscar una sala por nombre
int buscar_sala(const char *nombre) {
    for (int i = 0; i < num_salas; i++) {
        if (strcmp(salas[i].nombre, nombre) == 0) {
            return i;
        }
    }
    return -1; // No encontrada
}

// Función para agregar un usuario a una sala
int agregar_usuario_a_sala(int indice_sala, const char *nombre_usuario) {
    if (indice_sala < 0 || indice_sala >= num_salas) {
        return -1;
    }

    struct sala_chat *s = &salas[indice_sala];
    if (s->num_clientes >= MAX_CLIENTES) {
        return -1; // Sala llena
    }

    // Verificar si el usuario ya está en la sala
    for (int i = 0; i < s->num_clientes; i++) {
        if (strcmp(s->clientes[i].nombre, nombre_usuario) == 0) {
            return -1; // Usuario ya está en la sala
        }
    }

    // Agregar el usuario
    strcpy(s->clientes[s->num_clientes].nombre, nombre_usuario);
    s->clientes[s->num_clientes].pid = getpid(); // Se puede mejorar para usar el PID real del cliente
    strcpy(s->clientes[s->num_clientes].sala_actual, s->nombre);
    s->num_clientes++;
    return 0;
}

// Función para enviar un mensaje a todos los usuarios de una sala
void enviar_a_todos_en_sala(int indice_sala, struct mensaje *msg) {
    if (indice_sala < 0 || indice_sala >= num_salas) {
        return;
    }

    struct sala_chat *s = &salas[indice_sala];
    for (int i = 0; i < s->num_clientes; i++) {
        // Enviar el mensaje a la cola de la sala
        if (msgsnd(s->cola_id, msg, sizeof(struct mensaje) - sizeof(long), 0) == -1) {
            perror("Error al enviar mensaje a la sala");
        }
    }
}

int main() {
    // Crear la cola global para solicitudes de clientes
    key_t key_global = ftok("/tmp", 'A');
    int cola_global = msgget(key_global, IPC_CREAT | 0666);
    if (cola_global == -1) {
        perror("Error al crear la cola global");
        exit(1);
    }

    printf("Servidor de chat iniciado. Esperando clientes...\n");

    struct mensaje msg;

    while (1) {
        // Recibir mensajes de la cola global
        if (msgrcv(cola_global, &msg, sizeof(struct mensaje) - sizeof(long), 0, 0) == -1) {
            perror("Error al recibir mensaje");
            continue;
        }

        // Procesar el mensaje según su tipo
        printf("Mensaje recibido: Tipo=%s, Remitente=%s, Sala=%s\n",
               obtener_nombre_tipo_mensaje(msg.mtype), msg.remitente, msg.sala);

        if (msg.mtype == MSG_JOIN) { // JOIN
            printf("Solicitud de unirse a la sala: %s por %s\n", msg.sala, msg.remitente);

            // Buscar o crear la sala
            int indice_sala = buscar_sala(msg.sala);
            if (indice_sala == -1) {
                indice_sala = crear_sala(msg.sala);
                if (indice_sala == -1) {
                    printf("No se pudo crear la sala %s\n", msg.sala);
                    continue;
                }
                printf("Nueva sala creada: %s\n", msg.sala);
            }

            // Agregar el usuario a la sala
            if (agregar_usuario_a_sala(indice_sala, msg.remitente) == 0) {
                printf("Usuario %s agregado a la sala %s\n", msg.remitente, msg.sala);

                // Enviar confirmación al cliente usando función auxiliar
                struct mensaje respuesta;
                char texto_respuesta[MAX_TEXTO];
                sprintf(texto_respuesta, "Te has unido a la sala: %s", msg.sala);
                crear_mensaje_respuesta(&respuesta, texto_respuesta);
                if (msgsnd(cola_global, &respuesta, sizeof(struct mensaje) - sizeof(long), 0) == -1) {
                    perror("Error al enviar confirmación");
                }
            } else {
                printf("No se pudo agregar al usuario %s a la sala %s\n", msg.remitente, msg.sala);
            }
        } else if (msg.mtype == MSG_CHAT) { // CHAT
            printf("Mensaje en la sala %s de %s: %s\n", msg.sala, msg.remitente, msg.texto);

            // Buscar la sala
            int indice_sala = buscar_sala(msg.sala);
            if (indice_sala != -1) {
                // Reenviar el mensaje a todos en la sala
                enviar_a_todos_en_sala(indice_sala, &msg);
            }
        } else if (msg.mtype == MSG_DISCONNECT) { // DISCONNECT
            printf("Usuario %s se desconecta de la sala %s\n", msg.remitente, msg.sala);

            // Buscar la sala y remover al usuario
            int indice_sala = buscar_sala(msg.sala);
            if (indice_sala != -1) {
                // Aquí se podría implementar la función remover_usuario_de_sala
                printf("Removiendo usuario %s de la sala %s\n", msg.remitente, msg.sala);
            }

        } else if (msg.mtype == MSG_LIST_ROOMS) { // LIST_ROOMS
            printf("Usuario %s solicita lista de salas\n", msg.remitente);

            // Crear respuesta con la lista de salas
            struct mensaje respuesta;
            char lista_salas[MAX_TEXTO] = "Salas disponibles: ";

            for (int i = 0; i < num_salas; i++) {
                strcat(lista_salas, salas[i].nombre);
                if (i < num_salas - 1) {
                    strcat(lista_salas, ", ");
                }
            }

            if (num_salas == 0) {
                strcpy(lista_salas, "No hay salas disponibles");
            }

            crear_mensaje_respuesta(&respuesta, lista_salas);
            if (msgsnd(cola_global, &respuesta, sizeof(struct mensaje) - sizeof(long), 0) == -1) {
                perror("Error al enviar lista de salas");
            }

        } else if (msg.mtype == MSG_LEAVE_ROOM) { // LEAVE_ROOM
            printf("Usuario %s abandona la sala %s\n", msg.remitente, msg.sala);

            // Buscar la sala y remover al usuario
            int indice_sala = buscar_sala(msg.sala);
            if (indice_sala != -1) {
                printf("Usuario %s ha abandonado la sala %s\n", msg.remitente, msg.sala);

                // Enviar confirmación
                struct mensaje respuesta;
                char texto_respuesta[MAX_TEXTO];
                sprintf(texto_respuesta, "Has abandonado la sala: %s", msg.sala);
                crear_mensaje_respuesta(&respuesta, texto_respuesta);
                if (msgsnd(cola_global, &respuesta, sizeof(struct mensaje) - sizeof(long), 0) == -1) {
                    perror("Error al enviar confirmación de abandono");
                }
            }
        } else {
            printf("Tipo de mensaje no reconocido: %ld\n", msg.mtype);
        }
    }

    return 0;
}