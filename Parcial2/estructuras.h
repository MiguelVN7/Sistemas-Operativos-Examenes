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

// Tipos de mensaje (más simple)
#define MSG_JOIN 1      // Unirse a una sala
#define MSG_RESPUESTA 2 // Respuesta del servidor
#define MSG_CHAT 3      // Mensaje de chat

#endif