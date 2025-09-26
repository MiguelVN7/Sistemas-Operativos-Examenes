La carpeta Parcial2 contiene el desarrollo del reto 1 -> Sistema de Chat con Colas de Mensajes

Descripción del Reto
En el mundo actual, las aplicaciones de mensajería instantánea como WhatsApp son fundamentales para la comunicación diaria. En este reto, los estudiantes deberán implementar un sistema de chat simple entre procesos utilizando colas de mensajes (message queues) como mecanismo de comunicación entre procesos (IPC). El sistema debe soportar múltiples salas de chat, donde los usuarios pueden unirse a diferentes salas y enviar mensajes que serán recibidos por todos los miembros de la sala.

Objetivos del Reto
Implementar un sistema de chat que permita la comunicación entre múltiples procesos.
Utilizar colas de mensajes como mecanismo de IPC.
Soportar múltiples salas de chat, donde cada sala tiene su propia cola de mensajes.
Implementar un servidor central que gestione las salas y los mensajes.
Permitir que los procesos clientes se unan a salas, envíen mensajes y reciban mensajes de la sala.

Requisitos Técnicos
Lenguaje: C
Mecanismo IPC: Colas de mensajes (System V o POSIX)
Sistema Operativo: Linux
Trabajo en equipo: 2-5 personas por equipo
Fecha de entrega: 10 días a partir de la publicación del reto

Ejemplo Práctico: Funcionamiento del Sistema
Arquitectura del Sistema
El sistema estará compuesto por:

Servidor Central: Un proceso que se encarga de:

Crear y gestionar las colas de mensajes para cada sala.
Recibir mensajes de los clientes y reenviarlos a todos los miembros de la sala.
Gestionar la lista de salas y los usuarios en cada sala.
Clientes: Procesos que representan a los usuarios. Cada cliente:

Se une a una sala específica.
Puede enviar mensajes a la sala.
Recibe todos los mensajes enviados a la sala.
Flujo de Comunicación
Inicialización:

El servidor crea una cola de mensajes global para solicitudes de clientes.
Los clientes se conectan al servidor enviando un mensaje a la cola global.
Unirse a una sala:

Un cliente envía un mensaje de tipo "JOIN" a la cola global, indicando la sala a la que quiere unirse.
El servidor crea una cola para la sala (si no existe) y añade al cliente a la lista de esa sala.
El servidor responde al cliente con el identificador de la cola de la sala.
Enviar un mensaje:

Un cliente envía un mensaje a la cola de su sala.
El servidor recibe el mensaje y lo reenvía a todos los clientes de la sala (excepto al remitente).
Recibir mensajes:

Cada cliente está escuchando en la cola de su sala.
Cuando llega un mensaje, el cliente lo muestra en pantalla.
Ejemplo de Ejecución con Nombres Colombianos
Supongamos que tenemos dos salas: "General" y "Deportes". Tres clientes: María, Juan y Camila.

Servidor:

$ ./servidor
Servidor de chat iniciado. Esperando clientes...
Cliente María (se une a "General"):

$ ./cliente María
Bienvenido, María. Salas disponibles: General, Deportes
> join General
Te has unido a la sala: General
> Hola a todos en General!
Cliente Juan (se une a "General"):

$ ./cliente Juan
Bienvenido, Juan. Salas disponibles: General, Deportes
> join General
Te has unido a la sala: General
> Hola María!
Cliente Camila (se une a "Deportes"):

$ ./cliente Camila
Bienvenido, Camila. Salas disponibles: General, Deportes
> join Deportes
Te has unido a la sala: Deportes
> ¿Alguien vio el partido del América?
Salida en María:

Hola a todos en General!
Juan: Hola María!
Salida en Juan:

María: Hola a todos en General!
Salida en Camila:

¿Alguien vio el partido del América?


Desarrollado por:
Miguel Villegas, Juan Jose Restrepo y Esteban Molina.