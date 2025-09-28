# Makefile para Sistema de Chat IPC con Colas de Mensajes
# Autor: Sistemas Operativos - Chat IPC
# Fecha: Septiembre 2025

# Configuración del compilador
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -Werror -pthread -g
LDFLAGS = -pthread

# Archivos fuente
SERVIDOR_SRC = servidor.c
CLIENTE_SRC = cliente.c
HEADERS = estructuras.h

# Archivos ejecutables
SERVIDOR_BIN = servidor
CLIENTE_BIN = cliente

# Directorio temporal para archivos de clave ftok
TMP_DIR = /tmp

# Objetivo por defecto
all: $(SERVIDOR_BIN) $(CLIENTE_BIN)
	@echo "=== Sistema de Chat IPC compilado correctamente ==="
	@echo "Ejecutables generados: $(SERVIDOR_BIN), $(CLIENTE_BIN)"

# Compilar servidor
$(SERVIDOR_BIN): $(SERVIDOR_SRC) $(HEADERS)
	@echo "Compilando servidor..."
	$(CC) $(CFLAGS) -o $(SERVIDOR_BIN) $(SERVIDOR_SRC) $(LDFLAGS)

# Compilar cliente
$(CLIENTE_BIN): $(CLIENTE_SRC) $(HEADERS)
	@echo "Compilando cliente..."
	$(CC) $(CFLAGS) -o $(CLIENTE_BIN) $(CLIENTE_SRC) $(LDFLAGS)

# Ejecutar servidor en background
run-server: $(SERVIDOR_BIN)
	@echo "Iniciando servidor..."
	@echo "Para detener el servidor: make stop-server"
	./$(SERVIDOR_BIN) &
	@echo "Servidor ejecutándose en background (PID: $$!)"

# Ejecutar cliente (requiere nombre de usuario)
run-client: $(CLIENTE_BIN)
	@if [ -z "$(USER)" ]; then \
		echo "Uso: make run-client USER=<nombre>"; \
		echo "Ejemplo: make run-client USER=María"; \
		exit 1; \
	fi
	@echo "Iniciando cliente para usuario: $(USER)"
	./$(CLIENTE_BIN) $(USER)

# Ejecutar múltiples clientes de prueba
test-clients: $(CLIENTE_BIN)
	@echo "Iniciando clientes de prueba..."
	gnome-terminal -- bash -c "./$(CLIENTE_BIN) María; exec bash" 2>/dev/null || \
	xterm -e "./$(CLIENTE_BIN) María" 2>/dev/null || \
	echo "Para probar múltiples clientes, abre nuevas terminales y ejecuta:"
	@echo "  ./$(CLIENTE_BIN) Juan"
	@echo "  ./$(CLIENTE_BIN) Camila"

# Detener servidor
stop-server:
	@echo "Deteniendo servidor..."
	@pkill -f "./$(SERVIDOR_BIN)" 2>/dev/null || echo "Servidor no está ejecutándose"

# Limpiar colas de mensajes huérfanas
clean-ipc:
	@echo "Limpiando colas de mensajes del sistema..."
	@for queue in $$(ipcs -q | grep $(USER) | awk '{print $$2}'); do \
		echo "Eliminando cola: $$queue"; \
		ipcrm -q $$queue 2>/dev/null || true; \
	done
	@echo "Limpiando archivos de clave..."
	@rm -f $(TMP_DIR)/chat_*.key 2>/dev/null || true

# Mostrar colas de mensajes activas
show-ipc:
	@echo "=== Colas de mensajes activas ==="
	@ipcs -q
	@echo "=== Archivos de clave ==="
	@ls -la $(TMP_DIR)/chat_*.key 2>/dev/null || echo "No hay archivos de clave"

# Compilar con información de debug
debug: CFLAGS += -DDEBUG -O0
debug: clean all
	@echo "Compilación con debug activado"

# Compilar versión release (optimizada)
release: CFLAGS += -O3 -DNDEBUG
release: clean all
	@echo "Compilación release (optimizada)"

# Ejecutar pruebas básicas
test: all
	@echo "=== Ejecutando pruebas básicas ==="
	@echo "Iniciando servidor de prueba..."
	@./$(SERVIDOR_BIN) &
	@SERVER_PID=$$!; \
	sleep 2; \
	echo "Servidor iniciado (PID: $$SERVER_PID)"; \
	echo "Ejecutando cliente de prueba..."; \
	echo -e "list\nexit" | ./$(CLIENTE_BIN) TestUser; \
	echo "Deteniendo servidor..."; \
	kill $$SERVER_PID 2>/dev/null; \
	wait $$SERVER_PID 2>/dev/null; \
	echo "Pruebas básicas completadas"

# Verificar sintaxis y compilación
check: CFLAGS += -fsyntax-only
check:
	@echo "Verificando sintaxis..."
	$(CC) $(CFLAGS) $(SERVIDOR_SRC)
	$(CC) $(CFLAGS) $(CLIENTE_SRC)
	@echo "Sintaxis correcta"

# Analizar código con herramientas estáticas
analyze:
	@echo "=== Análisis estático de código ==="
	@which cppcheck > /dev/null && cppcheck --enable=all --std=c99 *.c || echo "cppcheck no disponible"
	@which valgrind > /dev/null && echo "Para análisis con valgrind: make valgrind-server o make valgrind-client" || echo "valgrind no disponible"

# Ejecutar servidor con valgrind
valgrind-server: $(SERVIDOR_BIN)
	@echo "Ejecutando servidor con valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(SERVIDOR_BIN)

# Ejecutar cliente con valgrind
valgrind-client: $(CLIENTE_BIN)
	@if [ -z "$(USER)" ]; then \
		echo "Uso: make valgrind-client USER=<nombre>"; \
		exit 1; \
	fi
	@echo "Ejecutando cliente con valgrind..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(CLIENTE_BIN) $(USER)

# Limpiar archivos compilados
clean:
	@echo "Limpiando archivos compilados..."
	rm -f $(SERVIDOR_BIN) $(CLIENTE_BIN)
	rm -f core core.*
	rm -f *.o
	@echo "Limpieza completada"

# Limpiar todo (archivos + IPC)
clean-all: clean clean-ipc
	@echo "Limpieza completa realizada"

# Mostrar ayuda
help:
	@echo "=== Sistema de Chat IPC - Ayuda de Makefile ==="
	@echo ""
	@echo "Objetivos principales:"
	@echo "  all           - Compilar servidor y cliente"
	@echo "  clean         - Limpiar archivos compilados"
	@echo "  clean-all     - Limpiar archivos + recursos IPC"
	@echo ""
	@echo "Ejecución:"
	@echo "  run-server           - Ejecutar servidor"
	@echo "  run-client USER=<nombre> - Ejecutar cliente"
	@echo "  stop-server          - Detener servidor"
	@echo "  test-clients         - Abrir múltiples clientes de prueba"
	@echo ""
	@echo "Depuración:"
	@echo "  debug            - Compilar con información de debug"
	@echo "  valgrind-server  - Ejecutar servidor con valgrind"
	@echo "  valgrind-client USER=<nombre> - Ejecutar cliente con valgrind"
	@echo ""
	@echo "Herramientas:"
	@echo "  show-ipc    - Mostrar colas de mensajes activas"
	@echo "  clean-ipc   - Limpiar recursos IPC huérfanos"
	@echo "  check       - Verificar sintaxis"
	@echo "  analyze     - Análisis estático de código"
	@echo "  test        - Ejecutar pruebas básicas"
	@echo ""
	@echo "Ejemplos:"
	@echo "  make && make run-server"
	@echo "  make run-client USER=María"
	@echo "  make test-clients"

# Configurar objetivo .PHONY para evitar conflictos con archivos
.PHONY: all clean clean-all clean-ipc show-ipc run-server run-client stop-server test-clients debug release test check analyze valgrind-server valgrind-client help

# Configuración de dependencias
$(SERVIDOR_BIN) $(CLIENTE_BIN): $(HEADERS)