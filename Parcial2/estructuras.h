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

// Estructura principal para los mensajes del sistema
struct mensaje {
    long mtype;                     // Tipo de mensaje (ver tipo_mensaje_t)
    char remitente[MAX_NOMBRE];     // Nombre del usuario que envía el mensaje
    char texto[MAX_TEXTO];          // Contenido del mensaje o información adicional
    char sala[MAX_NOMBRE];          // Nombre de la sala a la que pertenece el mensaje
    int cola_sala_id;               // ID de la cola de la sala (para JOIN_ACK)
    int codigo_respuesta;           // Código de respuesta (0=éxito, -1=error)
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
    MSG_JOIN_ACK = 2,       // Confirmación de unión exitosa con info de cola
    MSG_CHAT = 3,           // Mensaje de chat entre usuarios en la sala
    MSG_LEAVE = 4,          // Abandonar la sala actual
    MSG_LIST = 5,           // Solicitar lista de salas disponibles
    MSG_WHO = 6,            // Solicitar lista de usuarios en sala actual
    MSG_ERROR = 9,          // Mensaje de error del servidor
    MSG_DISCONNECT = 10     // Cliente se desconecta del sistema
} tipo_mensaje_t;

// Definiciones para compatibilidad con código existente
#define TIPO_JOIN 1
#define TIPO_JOIN_ACK 2
#define TIPO_CHAT 3
#define TIPO_LEAVE 4
#define TIPO_LIST 5
#define TIPO_WHO 6
#define TIPO_ERROR 9
#define TIPO_DISCONNECT 10

// Códigos de respuesta
#define RESP_OK "OK"
#define RESP_ERROR_SALA_LLENA "ERROR: Sala llena"
#define RESP_ERROR_USUARIO_DUPLICADO "ERROR: Usuario ya existe en la sala"
#define RESP_ERROR_SALA_NO_EXISTE "ERROR: Sala no existe"
#define RESP_ERROR_NO_EN_SALA "ERROR: No estás en ninguna sala"

// Archivos para generar claves ftok
#define ARCHIVO_COLA_GLOBAL "/tmp/chat_global.key"
#define PREFIJO_ARCHIVO_SALA "/tmp/chat_room_"
#define SUFIJO_ARCHIVO_SALA ".key"

// Funciones auxiliares para crear mensajes de diferentes tipos
static inline void crear_mensaje_join(struct mensaje *msg, const char *remitente, const char *sala) {
    msg->mtype = MSG_JOIN;
    strcpy(msg->remitente, remitente);
    strcpy(msg->sala, sala);
    strcpy(msg->texto, "");
    msg->cola_sala_id = -1;
    msg->codigo_respuesta = 0;
}

static inline void crear_mensaje_chat(struct mensaje *msg, const char *remitente, const char *sala, const char *texto) {
    msg->mtype = MSG_CHAT;
    strcpy(msg->remitente, remitente);
    strcpy(msg->sala, sala);
    strcpy(msg->texto, texto);
    msg->cola_sala_id = -1;
    msg->codigo_respuesta = 0;
}

static inline void crear_mensaje_respuesta(struct mensaje *msg, const char *texto) {
    msg->mtype = MSG_JOIN_ACK;
    strcpy(msg->remitente, "SERVIDOR");
    strcpy(msg->sala, "");
    strcpy(msg->texto, texto);
    msg->cola_sala_id = -1;
    msg->codigo_respuesta = 0;
}

static inline void crear_mensaje_disconnect(struct mensaje *msg, const char *remitente, const char *sala) {
    msg->mtype = MSG_DISCONNECT;
    strcpy(msg->remitente, remitente);
    strcpy(msg->sala, sala);
    strcpy(msg->texto, "");
    msg->cola_sala_id = -1;
    msg->codigo_respuesta = 0;
}

static inline void crear_mensaje_list_rooms(struct mensaje *msg, const char *remitente) {
    msg->mtype = MSG_LIST;
    strcpy(msg->remitente, remitente);
    strcpy(msg->sala, "");
    strcpy(msg->texto, "");
    msg->cola_sala_id = -1;
    msg->codigo_respuesta = 0;
}

static inline void crear_mensaje_leave_room(struct mensaje *msg, const char *remitente, const char *sala) {
    msg->mtype = MSG_LEAVE;
    strcpy(msg->remitente, remitente);
    strcpy(msg->sala, sala);
    strcpy(msg->texto, "");
    msg->cola_sala_id = -1;
    msg->codigo_respuesta = 0;
}

// Función auxiliar para crear mensaje WHO
static inline void crear_mensaje_who(struct mensaje *msg, const char *remitente, const char *sala) {
    msg->mtype = MSG_WHO;
    strcpy(msg->remitente, remitente);
    strcpy(msg->sala, sala);
    strcpy(msg->texto, "");
    msg->cola_sala_id = -1;
    msg->codigo_respuesta = 0;
}

// Función auxiliar para crear mensaje JOIN_ACK con información de cola
static inline void crear_mensaje_join_ack(struct mensaje *msg, const char *texto, int cola_id, int codigo) {
    msg->mtype = MSG_JOIN_ACK;
    strcpy(msg->remitente, "SERVIDOR");
    strcpy(msg->sala, "");
    strcpy(msg->texto, texto);
    msg->cola_sala_id = cola_id;
    msg->codigo_respuesta = codigo;
}

// Función auxiliar para crear mensaje ERROR
static inline void crear_mensaje_error(struct mensaje *msg, const char *texto) {
    msg->mtype = MSG_ERROR;
    strcpy(msg->remitente, "SERVIDOR");
    strcpy(msg->sala, "");
    strcpy(msg->texto, texto);
    msg->cola_sala_id = -1;
    msg->codigo_respuesta = -1;
}

// Función para obtener el nombre del tipo de mensaje (útil para debugging)
static inline const char* obtener_nombre_tipo_mensaje(long tipo) {
    switch(tipo) {
        case MSG_JOIN: return "JOIN";
        case MSG_JOIN_ACK: return "JOIN_ACK";
        case MSG_CHAT: return "CHAT";
        case MSG_LEAVE: return "LEAVE";
        case MSG_LIST: return "LIST";
        case MSG_WHO: return "WHO";
        case MSG_ERROR: return "ERROR";
        case MSG_DISCONNECT: return "DISCONNECT";
        default: return "DESCONOCIDO";
    }
}

// Funciones para manejo seguro de claves ftok
static inline int crear_archivo_clave(const char *ruta) {
    FILE *f = fopen(ruta, "w");
    if (f) {
        fprintf(f, "clave");
        fclose(f);
        return 0;
    }
    return -1;
}

static inline key_t obtener_clave_global() {
    crear_archivo_clave(ARCHIVO_COLA_GLOBAL);
    return ftok(ARCHIVO_COLA_GLOBAL, 'A');
}

static inline key_t obtener_clave_sala(const char *nombre_sala) {
    char ruta[256];
    snprintf(ruta, sizeof(ruta), "%s%s%s", PREFIJO_ARCHIVO_SALA, nombre_sala, SUFIJO_ARCHIVO_SALA);
    crear_archivo_clave(ruta);
    return ftok(ruta, 'R');
}

#endif