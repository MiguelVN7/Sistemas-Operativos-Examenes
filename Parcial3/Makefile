# Makefile para GSEA (Generador de Sufijos Encriptados y Archivados)
# Proyecto Parcial 3 - Sistemas Operativos

# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -pthread -I./src -I./src/compression -I./src/encryption -I./src/utils
LDFLAGS = -pthread

# Directorios
SRC_DIR = src
BUILD_DIR = build
COMPRESSION_DIR = $(SRC_DIR)/compression
ENCRYPTION_DIR = $(SRC_DIR)/encryption
UTILS_DIR = $(SRC_DIR)/utils
TEST_DIR = tests

# Debug flags (opcional)
ifdef DEBUG
	CFLAGS += -g -DDEBUG -DDEBUG_MEMORY
else
	CFLAGS += -O2
endif

# Nombre del ejecutable
TARGET = $(BUILD_DIR)/gsea

# Archivos fuente
MAIN_SOURCES = $(SRC_DIR)/main.c \
               $(SRC_DIR)/args_parser.c \
               $(SRC_DIR)/file_manager.c \
               $(SRC_DIR)/concurrency.c

COMPRESSION_SOURCES = $(COMPRESSION_DIR)/huffman.c \
                      $(COMPRESSION_DIR)/rle.c \
                      $(COMPRESSION_DIR)/lzw.c

ENCRYPTION_SOURCES = $(ENCRYPTION_DIR)/vigenere.c \
                     $(ENCRYPTION_DIR)/des.c \
                     $(ENCRYPTION_DIR)/aes_simple.c

UTILS_SOURCES = $(UTILS_DIR)/error_handler.c \
                $(UTILS_DIR)/memory_manager.c

# Todos los archivos fuente
ALL_SOURCES = $(MAIN_SOURCES) $(COMPRESSION_SOURCES) $(ENCRYPTION_SOURCES) $(UTILS_SOURCES)

# Archivos objeto
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(MAIN_SOURCES)) \
          $(patsubst $(COMPRESSION_DIR)/%.c,$(BUILD_DIR)/compression_%.o,$(COMPRESSION_SOURCES)) \
          $(patsubst $(ENCRYPTION_DIR)/%.c,$(BUILD_DIR)/encryption_%.o,$(ENCRYPTION_SOURCES)) \
          $(patsubst $(UTILS_DIR)/%.c,$(BUILD_DIR)/utils_%.o,$(UTILS_SOURCES))

# Archivos de prueba
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/test_%.o,$(TEST_SOURCES))
TEST_TARGETS = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/test_%,$(TEST_SOURCES))

# Regla principal
.PHONY: all clean test debug help structure

all: $(TARGET)

# Construir el ejecutable principal
$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	@echo "Linking $(TARGET)..."
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(TARGET)
	@echo "Build complete: $(TARGET)"

# Compilar archivos fuente principales
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compilar módulos de compresión
$(BUILD_DIR)/compression_%.o: $(COMPRESSION_DIR)/%.c | $(BUILD_DIR)
	@echo "Compiling compression module: $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compilar módulos de encriptación
$(BUILD_DIR)/encryption_%.o: $(ENCRYPTION_DIR)/%.c | $(BUILD_DIR)
	@echo "Compiling encryption module: $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compilar módulos de utilidades
$(BUILD_DIR)/utils_%.o: $(UTILS_DIR)/%.c | $(BUILD_DIR)
	@echo "Compiling utils module: $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compilar archivos de prueba
$(BUILD_DIR)/test_%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	@echo "Compiling test: $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Crear ejecutables de prueba
$(BUILD_DIR)/test_%: $(BUILD_DIR)/test_%.o $(filter-out $(BUILD_DIR)/main.o,$(OBJECTS)) | $(BUILD_DIR)
	@echo "Linking test: $@..."
	$(CC) $^ $(LDFLAGS) -o $@

# Crear directorio build si no existe
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compilar con símbolos de debug
debug:
	$(MAKE) DEBUG=1 all

# Ejecutar pruebas
test: $(TEST_TARGETS)
	@echo "Running tests..."
	@for test in $(TEST_TARGETS); do \
		echo "Running $$test..."; \
		$$test; \
	done

# Limpiar archivos generados
clean:
	@echo "Cleaning build files..."
	rm -rf $(BUILD_DIR)
	@echo "Clean complete"

# Limpiar y recompilar
rebuild: clean all

# Instalar (opcional)
install: $(TARGET)
	@echo "Installing $(TARGET) to /usr/local/bin..."
	@sudo cp $(TARGET) /usr/local/bin/gsea
	@echo "Installation complete"

# Desinstalar
uninstall:
	@echo "Uninstalling gsea from /usr/local/bin..."
	@sudo rm -f /usr/local/bin/gsea
	@echo "Uninstall complete"

# Mostrar estructura del proyecto
structure:
	@echo "GSEA Project Structure:"
	@tree -I 'build' . || find . -not -path '*/build/*' -not -path '*/.git/*' -type f

# Información de ayuda
help:
	@echo "GSEA - Generador de Sufijos Encriptados y Archivados"
	@echo ""
	@echo "Available targets:"
	@echo "  all      - Build the main executable (default)"
	@echo "  debug    - Build with debug symbols and memory tracking"
	@echo "  test     - Build and run all tests"
	@echo "  clean    - Remove all build files"
	@echo "  rebuild  - Clean and rebuild"
	@echo "  install  - Install to /usr/local/bin (requires sudo)"
	@echo "  uninstall- Remove from /usr/local/bin (requires sudo)"
	@echo "  structure- Show project directory structure"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Debug build:"
	@echo "  make DEBUG=1"
	@echo ""
	@echo "Example usage:"
	@echo "  make              # Build release version"
	@echo "  make debug        # Build with debugging"
	@echo "  make test         # Run tests"
	@echo "  ./build/gsea      # Run the program"

# Dependencias automáticas (para reconstruir cuando cambian headers)
-include $(OBJECTS:.o=.d)

$(BUILD_DIR)/%.d: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -MM -MT '$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$<)' $< > $@
