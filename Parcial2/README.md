# Sistema de Chat IPC con Colas de Mensajes

**Autor:** Sistemas Operativos - Parcial 2  
**Fecha:** Septiembre 2025  
**Lenguaje:** C (estándar C99)  
**Plataforma:** Linux  

## Descripción del Proyecto

Este proyecto implementa un sistema de chat multi-proceso en Linux utilizando **colas de mensajes System V** como mecanismo de IPC. El sistema soporta múltiples salas de chat donde los usuarios pueden unirse, enviar mensajes y recibir mensajes de otros usuarios en tiempo (casi) real.

### Características Principales

- ✅ **Servidor central** que gestiona salas y reenvía mensajes
- ✅ **Múltiples salas** de chat dinámicas (≥10 salas, ≥20 usuarios por sala)
- ✅ **Colas de mensajes System V** con esquema de claves ftok seguro
- ✅ **Concurrencia** con pthreads para recepción de mensajes sin busy-wait
- ✅ **Protocolo robusto** con tipos de mensaje bien definidos
- ✅ **Manejo de señales** para limpieza automática de recursos IPC
- ✅ **Validación completa** de errores y casos límite
- ✅ **Thread-safety** con mutexes para estructuras compartidas

## Arquitectura del Sistema

### Componentes

1. **Servidor (`servidor.c`)**: Proceso central que:
   - Gestiona la cola global para solicitudes de clientes
   - Crea y administra salas de chat dinámicamente
   - Reenvía mensajes entre usuarios de la misma sala
   - Mantiene listas de usuarios por sala con exclusión mutua

2. **Cliente (`cliente.c`)**: Procesos individuales que:
   - Se conectan al servidor a través de la cola global
   - Utilizan un hilo receptor para mensajes en tiempo real
   - Implementan interfaz de comandos interactiva
   - Manejan reconexión y limpieza de recursos

3. **Estructuras (`estructuras.h`)**: Definiciones compartidas:
   - Protocolo de mensajes con tipos bien definidos
   - Estructuras de datos para salas y usuarios
   - Funciones auxiliares para manejo seguro de claves ftok

### Protocolo de Mensajes

| Tipo | Valor | Descripción |
|------|-------|-------------|
| MSG_JOIN | 1 | Solicitud de unirse a una sala |
| MSG_JOIN_ACK | 2 | Confirmación de unión con info de cola |
| MSG_CHAT | 3 | Mensaje de chat entre usuarios |
| MSG_LEAVE | 4 | Abandonar sala actual |
| MSG_LIST | 5 | Solicitar lista de salas |
| MSG_WHO | 6 | Listar usuarios en sala |
| MSG_ERROR | 9 | Mensaje de error del servidor |
| MSG_DISCONNECT | 10 | Desconexión del sistema |

### Esquema de Claves IPC

- **Cola Global**: `ftok("/tmp/chat_global.key", 'A')`
- **Colas de Sala**: `ftok("/tmp/chat_room_<sala>.key", 'R')`

Este esquema garantiza claves únicas y persistentes usando archivos reales.

## Compilación y Ejecución

### Requisitos del Sistema

- Linux (cualquier distribución reciente)
- GCC con soporte C99
- Make
- pthreads (libpthread)

### Compilación

```bash
# Compilar todo el sistema
make

# Compilar solo el servidor
make servidor

# Compilar solo el cliente  
make cliente

# Compilación con debug
make debug

# Compilación optimizada
make release
```

### Ejecución Básica

**1. Iniciar el servidor:**
```bash
make run-server
# o directamente:
./servidor
```

**2. Conectar clientes (en terminales separadas):**
```bash
make run-client USER=María
make run-client USER=Juan
make run-client USER=Camila

# o directamente:
./cliente María
./cliente Juan
./cliente Camila
```

### Comandos del Cliente

| Comando | Descripción | Ejemplo |
|---------|-------------|---------|
| `join <sala>` | Unirse a una sala | `join General` |
| `list` | Ver salas disponibles | `list` |
| `who` | Ver usuarios en sala actual | `who` |
| `leave` | Abandonar sala actual | `leave` |
| `help` | Mostrar ayuda | `help` |
| `exit` | Salir del chat | `exit` |
| `<mensaje>` | Enviar mensaje | `Hola a todos!` |

## Demostración Práctica

### Escenario: Usuarios Colombianos

**Usuarios:** María, Juan, Camila  
**Salas:** General, Deportes

### Transcripción de Uso Real

```
=== TERMINAL 1: Servidor ===
$ ./servidor
=== SERVIDOR DE CHAT INICIADO ===
Cola global ID: 32768
Esperando clientes... (Ctrl+C para salir)

[JOIN] María -> General: 
Nueva sala creada: 'General' con cola ID 32769
Usuario 'María' (PID: 1234) agregado a sala 'General' (1/20)

[CHAT] María -> General: Hola a todos en General!

[JOIN] Juan -> General: 
Usuario 'Juan' (PID: 1235) agregado a sala 'General' (2/20)

[CHAT] Juan -> General: Hola María! ¿Cómo estás?
```

```
=== TERMINAL 2: Cliente María ===
$ ./cliente María
=== CHAT IPC - Bienvenido/a María ===

> list
No hay salas disponibles

> join General  
Bienvenido a la sala 'General'
Conectado a sala 'General'. ¡Comienza a chatear!

> Hola a todos en General!

> who
Usuarios en sala: María

> Juan: Hola María! ¿Cómo estás?

> Estoy bien, gracias por preguntar
```

```  
=== TERMINAL 3: Cliente Juan ===
$ ./cliente Juan

> join General
Bienvenido a la sala 'General'  

> Hola María! ¿Cómo estás?

> María: Estoy bien, gracias por preguntar

> who
Usuarios en sala: María, Juan

> join Deportes
Abandonando sala actual: General
Bienvenido a la sala 'Deportes'

> ¡El partido de ayer estuvo increíble!
```

### Ejecutar Demo Automática

```bash
# Demo interactiva con usuarios colombianos
./demo_chat.sh
```

## Pruebas y Validación

### Ejecutar Pruebas Automatizadas

```bash
# Suite completa de pruebas
./test_chat.sh

# Pruebas básicas integradas
make test

# Pruebas con múltiples clientes
make test-clients
```

### Análisis con Herramientas

```bash
# Verificación de sintaxis
make check

# Análisis estático
make analyze

# Análisis de memoria con Valgrind
make valgrind-server
make valgrind-client USER=TestUser
```

### Monitoreo de Recursos IPC

```bash
# Ver colas de mensajes activas
make show-ipc
ipcs -q

# Limpiar recursos huérfanos  
make clean-ipc
```

## Gestión de Errores

El sistema maneja robustamente:

- ✅ **Sala llena** (MAX_CLIENTES alcanzado)
- ✅ **Usuario duplicado** en la misma sala
- ✅ **Sala inexistente** para comandos
- ✅ **Mensaje demasiado largo** (MAX_TEXTO)
- ✅ **Comandos inválidos** con mensajes descriptivos  
- ✅ **Pérdida de conexión** del servidor
- ✅ **Recursos IPC huérfanos** con limpieza automática
- ✅ **Señales del sistema** (SIGINT, SIGTERM)

## Concurrencia y Thread Safety

### Cliente
- **Hilo principal**: Interfaz de usuario y envío de comandos
- **Hilo receptor**: Recepción de mensajes sin busy-wait usando `msgrcv` bloqueante
- **Sincronización**: Variables globales protegidas, limpieza ordenada con `pthread_cancel`

### Servidor  
- **Hilo único**: Procesamiento secuencial de la cola global
- **Exclusión mutua**: `pthread_mutex_t` para proteger array de salas
- **Thread-safety**: Todas las operaciones sobre estructuras compartidas están protegidas

## Escalabilidad y Extensiones

### Parámetros Configurables
```c
#define MAX_TEXTO 256        // Tamaño máximo de mensaje
#define MAX_NOMBRE 50        // Tamaño máximo de nombre
#define MAX_SALAS 10         // Número máximo de salas
#define MAX_CLIENTES 20      // Usuarios máximos por sala
```

### Posibles Mejoras Futuras
- **Persistencia**: Guardar salas y usuarios en archivo/BD
- **Autenticación**: Sistema de login/passwords  
- **Salas privadas**: Salas con acceso restringido
- **Mensajes privados**: Chat directo entre usuarios
- **Historial**: Almacenar mensajes históricos
- **Escalabilidad**: Usar epoll/kqueue para más conexiones
- **Red**: Migrar a sockets TCP/UDP para distribución

## Estructura del Proyecto

```
Parcial2/
├── README.md              # Este archivo
├── estructuras.h          # Definiciones y protocolo compartido
├── servidor.c             # Implementación del servidor
├── cliente.c              # Implementación del cliente
├── Makefile              # Sistema de construcción completo
├── test_chat.sh          # Suite de pruebas automatizadas
└── demo_chat.sh          # Demostración interactiva
```

## Plan de Desarrollo (10 días)

### Hitos Completados ✅

**Días 1-2: Diseño y Estructura Base**
- [x] Definición del protocolo de mensajes
- [x] Estructuras de datos principales  
- [x] Esquema de claves ftok seguro

**Días 3-4: Implementación Core**  
- [x] Servidor con gestión de salas
- [x] Cliente con interfaz básica
- [x] Cola global y colas por sala

**Días 5-6: Concurrencia y IPC**
- [x] Hilos receptores sin busy-wait
- [x] Exclusión mutua en servidor
- [x] Manejo de señales y limpieza

**Días 7-8: Validación y Robustez**
- [x] Manejo completo de errores
- [x] Validación de entrada de usuario  
- [x] Casos límite y edge cases

**Días 9-10: Pruebas y Documentación**
- [x] Suite de pruebas automatizadas
- [x] Scripts de demostración
- [x] Documentación completa
- [x] Makefile con todas las opciones

## Compilación con Warnings Estrictos

El proyecto compila sin warnings con:
```bash
gcc -std=c99 -Wall -Wextra -Wpedantic -Werror -pthread
```

## Créditos y Licencia

**Desarrollado para:** Curso de Sistemas Operativos  
**Universidad:** [Tu Universidad]  
**Semestre:** 2025-II

Este proyecto es material académico desarrollado con fines educativos para demostrar conceptos de IPC, concurrencia y diseño de sistemas distribuidos en entornos UNIX/Linux.
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