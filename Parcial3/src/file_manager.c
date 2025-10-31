#include "file_manager.h"
#include "utils/error_handler.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/**
 * @brief Lee un archivo completo usando syscalls
 * TODO: Optimizar para archivos muy grandes
 */
ssize_t read_file(const char* path, FileBuffer* buffer) {
    if (path == NULL || buffer == NULL) {
        LOG_ERROR(ERROR_INVALID_ARGUMENT, SEVERITY_ERROR, "Invalid parameters");
        return -1;
    }

    // TODO: Implementar lectura completa con open, read, close
    LOG_ERROR(ERROR_FILE_READ, SEVERITY_WARNING, "Function not implemented");
    return -1;
}

/**
 * @brief Escribe datos a un archivo usando syscalls
 * TODO: Implementar escritura con manejo de errores
 */
ssize_t write_file(const char* path, const FileBuffer* buffer) {
    if (path == NULL || buffer == NULL) {
        LOG_ERROR(ERROR_INVALID_ARGUMENT, SEVERITY_ERROR, "Invalid parameters");
        return -1;
    }

    // TODO: Implementar escritura completa con open, write, close
    LOG_ERROR(ERROR_FILE_WRITE, SEVERITY_WARNING, "Function not implemented");
    return -1;
}

/**
 * @brief Lee todos los archivos de un directorio
 * TODO: Implementar con opendir, readdir, closedir
 */
int read_directory(const char* dir_path, FileList* file_list) {
    if (dir_path == NULL || file_list == NULL) {
        LOG_ERROR(ERROR_INVALID_ARGUMENT, SEVERITY_ERROR, "Invalid parameters");
        return -1;
    }

    // TODO: Implementar lectura de directorio
    LOG_ERROR(ERROR_DIRECTORY_ACCESS, SEVERITY_WARNING, "Function not implemented");
    return -1;
}

/**
 * @brief Obtiene información de un archivo
 * TODO: Implementar con stat
 */
int get_file_info(const char* path, FileInfo* info) {
    if (path == NULL || info == NULL) {
        return -1;
    }

    // TODO: Implementar con stat()
    return -1;
}

/**
 * @brief Verifica si una ruta es un directorio
 */
int is_directory(const char* path) {
    struct stat st;
    if (stat(path, &st) < 0) {
        return -1;
    }
    return S_ISDIR(st.st_mode) ? 1 : 0;
}

/**
 * @brief Verifica si un archivo existe
 */
int file_exists(const char* path) {
    return access(path, F_OK) == 0 ? 1 : 0;
}

/**
 * @brief Crea un buffer para datos de archivo
 * TODO: Integrar con memory manager
 */
FileBuffer* create_file_buffer(size_t initial_capacity) {
    FileBuffer* buffer = malloc(sizeof(FileBuffer));
    if (buffer == NULL) {
        return NULL;
    }

    buffer->data = malloc(initial_capacity);
    if (buffer->data == NULL) {
        free(buffer);
        return NULL;
    }

    buffer->size = 0;
    buffer->capacity = initial_capacity;

    return buffer;
}

/**
 * @brief Redimensiona un buffer
 */
int resize_file_buffer(FileBuffer* buffer, size_t new_capacity) {
    if (buffer == NULL) {
        return -1;
    }

    unsigned char* new_data = realloc(buffer->data, new_capacity);
    if (new_data == NULL) {
        LOG_ERROR(ERROR_MEMORY_ALLOCATION, SEVERITY_ERROR, "Failed to resize buffer");
        return -1;
    }

    buffer->data = new_data;
    buffer->capacity = new_capacity;

    return 0;
}

/**
 * @brief Libera un buffer
 */
void free_file_buffer(FileBuffer* buffer) {
    if (buffer != NULL) {
        if (buffer->data != NULL) {
            free(buffer->data);
        }
        free(buffer);
    }
}

/**
 * @brief Crea una lista de archivos
 */
FileList* create_file_list(void) {
    FileList* list = malloc(sizeof(FileList));
    if (list == NULL) {
        return NULL;
    }

    list->capacity = 16;
    list->files = malloc(sizeof(FileInfo) * list->capacity);
    if (list->files == NULL) {
        free(list);
        return NULL;
    }

    list->count = 0;
    return list;
}

/**
 * @brief Agrega un archivo a la lista
 */
int add_file_to_list(FileList* list, const FileInfo* info) {
    if (list == NULL || info == NULL) {
        return -1;
    }

    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        FileInfo* new_files = realloc(list->files, sizeof(FileInfo) * new_capacity);
        if (new_files == NULL) {
            return -1;
        }
        list->files = new_files;
        list->capacity = new_capacity;
    }

    memcpy(&list->files[list->count], info, sizeof(FileInfo));
    list->count++;

    return 0;
}

/**
 * @brief Libera una lista de archivos
 */
void free_file_list(FileList* list) {
    if (list != NULL) {
        if (list->files != NULL) {
            free(list->files);
        }
        free(list);
    }
}

/**
 * @brief Crea un directorio
 * TODO: Implementar con mkdir
 */
int create_directory(const char* path) {
    if (path == NULL) {
        return -1;
    }

    // TODO: Implementar con mkdir()
    LOG_ERROR(ERROR_FILE_WRITE, SEVERITY_WARNING, "Function not implemented");
    return -1;
}

/**
 * @brief Construye ruta de salida basada en entrada
 * TODO: Mejorar manejo de rutas
 */
int build_output_path(const char* input_path, const char* output_base,
                     char* output_path, size_t max_length) {
    if (input_path == NULL || output_base == NULL || output_path == NULL) {
        return -1;
    }

    const char* filename = strrchr(input_path, '/');
    if (filename == NULL) {
        filename = input_path;
    } else {
        filename++;
    }

    int written = snprintf(output_path, max_length, "%s/%s", output_base, filename);

    if (written < 0 || (size_t)written >= max_length) {
        return -1;
    }

    return 0;
}
