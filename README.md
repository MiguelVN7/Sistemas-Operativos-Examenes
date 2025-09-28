# Sistemas-Operativos
Este repositorio contendra todo el desarrollo del parciales/Proyectos de la asignatura de Sistemas Operativos del Semestre 2025-2. Realizado por Miguel Villegas, Juan Jose Restrepo y Esteban Molina

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



Para compilar los programas, utiliza:

gcc servidor.c -o servidor -lpthread
gcc cliente.c -o cliente -lpthread
Para ejecutar:

Inicia el servidor en una terminal:

./servidor
Inicia los clientes en otras terminales:

./cliente María
./cliente Juan
./cliente Camila