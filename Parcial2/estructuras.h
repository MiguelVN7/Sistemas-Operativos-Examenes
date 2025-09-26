#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MAX_TEXTO 256
#define MAX_NOMBRE 50
#define MAX_SALAS 10
#define MAX_CLIENTES 20

// Estructura simple para los mensajes (compatible con cliente.c existente)
struct mensaje {
    long mtype;                     // Tipo de mensaje (1=JOIN, 2=RESPUESTA, 3=CHAT)
    char remitente[MAX_NOMBRE];     // Nombre del usuario que envía el mensaje
    char texto[MAX_TEXTO];          // Contenido del mensaje o información adicional
    char sala[MAX_NOMBRE];          // Nombre de la sala a la que pertenece el mensaje
};

// Estructura para guardar información de un cliente conectado
struct cliente {
    char nombre[MAX_NOMBRE];        // Nombre del usuario (ej: "María", "Juan")
    pid_t pid;                      // ID del proceso del cliente
    char sala_actual[MAX_NOMBRE];   // Nombre de la sala donde está el cliente
};

// Estructura para guardar información de una sala de chat
struct sala_chat {
    char nombre[MAX_NOMBRE];        // Nombre de la sala (ej: "General", "Deportes")
    int cola_id;                    // ID de la cola de mensajes de esta sala
    struct cliente clientes[MAX_CLIENTES]; // Array de clientes en esta sala
    int num_clientes;               // Cantidad actual de clientes en la sala
};

// Estructura principal del servidor que maneja todo el sistema
struct servidor {
    struct sala_chat salas[MAX_SALAS];  // Array de todas las salas disponibles
    int num_salas;                      // Cantidad actual de salas creadas
    int cola_global;                    // ID de la cola global para comunicación inicial
};

// Tipos de mensaje basados en el flujo del sistema de chat
typedef enum {
    MSG_JOIN = 1,           // Cliente solicita unirse a una sala
    MSG_RESPUESTA = 2,      // Respuesta del servidor a solicitudes
    MSG_CHAT = 3,           // Mensaje de chat entre usuarios en la sala
    MSG_DISCONNECT = 4,     // Cliente se desconecta del sistema
    MSG_LIST_ROOMS = 5,     // Solicitar lista de salas disponibles
    MSG_LEAVE_ROOM = 6      // Abandonar la sala actual
} tipo_mensaje_t;

// Definiciones para compatibilidad con código existente
#define TIPO_JOIN 1
#define TIPO_RESPUESTA 2
#define TIPO_CHAT 3
#define TIPO_DISCONNECT 4
#define TIPO_LIST_ROOMS 5
#define TIPO_LEAVE_ROOM 6

// Funciones auxiliares para crear mensajes de diferentes tipos
static inline void crear_mensaje_join(struct mensaje *msg, const char *remitente, const char *sala) {
    msg->mtype = MSG_JOIN;
    strcpy(msg->remitente, remitente);
    strcpy(msg->sala, sala);
    strcpy(msg->texto, "");
}

static inline void crear_mensaje_chat(struct mensaje *msg, const char *remitente, const char *sala, const char *texto) {
    msg->mtype = MSG_CHAT;
    strcpy(msg->remitente, remitente);
    strcpy(msg->sala, sala);
    strcpy(msg->texto, texto);
}

static inline void crear_mensaje_respuesta(struct mensaje *msg, const char *texto) {
    msg->mtype = MSG_RESPUESTA;
    strcpy(msg->remitente, "SERVIDOR");
    strcpy(msg->sala, "");
    strcpy(msg->texto, texto);
}

static inline void crear_mensaje_disconnect(struct mensaje *msg, const char *remitente, const char *sala) {
    msg->mtype = MSG_DISCONNECT;
    strcpy(msg->remitente, remitente);
    strcpy(msg->sala, sala);
    strcpy(msg->texto, "");
}

static inline void crear_mensaje_list_rooms(struct mensaje *msg, const char *remitente) {
    msg->mtype = MSG_LIST_ROOMS;
    strcpy(msg->remitente, remitente);
    strcpy(msg->sala, "");
    strcpy(msg->texto, "");
}

static inline void crear_mensaje_leave_room(struct mensaje *msg, const char *remitente, const char *sala) {
    msg->mtype = MSG_LEAVE_ROOM;
    strcpy(msg->remitente, remitente);
    strcpy(msg->sala, sala);
    strcpy(msg->texto, "");
}

// Función para obtener el nombre del tipo de mensaje (útil para debugging)
static inline const char* obtener_nombre_tipo_mensaje(long tipo) {
    switch(tipo) {
        case MSG_JOIN: return "JOIN";
        case MSG_RESPUESTA: return "RESPUESTA";
        case MSG_CHAT: return "CHAT";
        case MSG_DISCONNECT: return "DISCONNECT";
        case MSG_LIST_ROOMS: return "LIST_ROOMS";
        case MSG_LEAVE_ROOM: return "LEAVE_ROOM";
        default: return "DESCONOCIDO";
    }
}

#endif