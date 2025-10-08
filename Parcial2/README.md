# Sistema de Chat IPC con Colas de Mensajes

**Autor:** Sistemas Operativos - Parcial 2  
**Fecha:** Septiembre 2025  
**Lenguaje:** C (estándar C99)  
**Plataforma:** Linux  

## Descripción del Proyecto

Este proyecto implementa un sistema de chat multi-proceso en Linux utilizando **colas de mensajes System V** como mecanismo de IPC. El sistema soporta múltiples salas de chat donde los usuarios pueden unirse, enviar mensajes y recibir mensajes de otros usuarios en tiempo (casi) real.

### Características Principales

-  **Servidor central** que gestiona salas y reenvía mensajes
-  **Múltiples salas** de chat dinámicas (≥10 salas, ≥20 usuarios por sala)
-  **Colas de mensajes System V** con esquema de claves ftok seguro
-  **Concurrencia** con pthreads para recepción de mensajes sin busy-wait
-  **Protocolo robusto** con tipos de mensaje bien definidos
-  **Manejo de señales** para limpieza automática de recursos IPC
-  **Validación completa** de errores y casos límite
-  **Thread-safety** con mutexes para estructuras compartidas

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

