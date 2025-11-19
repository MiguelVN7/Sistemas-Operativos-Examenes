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
 */
ssize_t read_file(const char* path, FileBuffer* buffer) {
    if (path == NULL || buffer == NULL) {
        LOG_ERROR(ERROR_INVALID_ARGUMENT, SEVERITY_ERROR, "Invalid parameters for read_file");
        return -1;
    }

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        LOG_ERROR(ERROR_FILE_OPEN, SEVERITY_ERROR, "Failed to open file for reading");
        return -1;
    }

    // Obtener tamaño del archivo
    struct stat st;
    if (fstat(fd, &st) < 0) {
        LOG_ERROR(ERROR_FILE_READ, SEVERITY_ERROR, "Failed to get file stats");
        close(fd);
        return -1;
    }

    // Asegurar que el buffer tenga suficiente capacidad
    if (buffer->capacity < (size_t)st.st_size) {
        if (resize_file_buffer(buffer, st.st_size) != 0) {
            close(fd);
            return -1;
        }
    }

    // Leer archivo
    ssize_t total_read = 0;
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer->data + total_read, st.st_size - total_read)) > 0) {
        total_read += bytes_read;
    }

    if (bytes_read < 0) {
        LOG_ERROR(ERROR_FILE_READ, SEVERITY_ERROR, "Failed to read content");
        close(fd);
        return -1;
    }

    buffer->size = total_read;
    close(fd);
    return total_read;
}

/**
 * @brief Escribe datos a un archivo usando syscalls
 */
ssize_t write_file(const char* path, const FileBuffer* buffer) {
    if (path == NULL || buffer == NULL) {
        LOG_ERROR(ERROR_INVALID_ARGUMENT, SEVERITY_ERROR, "Invalid parameters for write_file");
        return -1;
    }

    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        LOG_ERROR(ERROR_FILE_OPEN, SEVERITY_ERROR, "Failed to open file for writing");
        return -1;
    }

    ssize_t total_written = 0;
    ssize_t bytes_written;
    size_t to_write = buffer->size;

    while (to_write > 0) {
        bytes_written = write(fd, buffer->data + total_written, to_write);
        if (bytes_written < 0) {
            if (errno == EINTR) continue;
            LOG_ERROR(ERROR_FILE_WRITE, SEVERITY_ERROR, "Failed to write content");
            close(fd);
            return -1;
        }
        total_written += bytes_written;
        to_write -= bytes_written;
    }

    close(fd);
    return total_written;
}

/**
 * @brief Lee todos los archivos de un directorio
 */
int read_directory(const char* dir_path, FileList* file_list) {
    if (dir_path == NULL || file_list == NULL) {
        LOG_ERROR(ERROR_INVALID_ARGUMENT, SEVERITY_ERROR, "Invalid parameters for read_directory");
        return -1;
    }

    DIR* dir = opendir(dir_path);
    if (dir == NULL) {
        LOG_ERROR(ERROR_DIRECTORY_ACCESS, SEVERITY_ERROR, "Failed to open directory");
        return -1;
    }

    struct dirent* entry;
    char full_path[MAX_PATH_LENGTH];

    while ((entry = readdir(dir)) != NULL) {
        // Ignorar . y ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(full_path, MAX_PATH_LENGTH, "%s/%s", dir_path, entry->d_name);

        FileInfo info;
        if (get_file_info(full_path, &info) == 0) {
            // Solo procesamos archivos regulares por ahora
            if (!info.is_directory) {
                add_file_to_list(file_list, &info);
            }
        }
    }

    closedir(dir);
    return 0;
}

/**
 * @brief Obtiene información de un archivo
 */
int get_file_info(const char* path, FileInfo* info) {
    if (path == NULL || info == NULL) {
        return -1;
    }

    struct stat st;
    if (stat(path, &st) < 0) {
        return -1;
    }

    strncpy(info->path, path, MAX_PATH_LENGTH - 1);
    info->path[MAX_PATH_LENGTH - 1] = '\0';
    info->size = st.st_size;
    info->is_directory = S_ISDIR(st.st_mode);
    info->mode = st.st_mode;

    return 0;
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
 */
int create_directory(const char* path) {
    if (path == NULL) {
        return -1;
    }

    // mkdir devuelve 0 en éxito, -1 en error
    if (mkdir(path, 0755) < 0) {
        if (errno == EEXIST) {
            return 0; // Ya existe, no es error crítico
        }
        return -1;
    }
    return 0;
}

/**
 * @brief Construye ruta de salida basada en entrada
 */
int build_output_path(const char* input_path, const char* output_base,
                     char* output_path, size_t max_length) {
    if (input_path == NULL || output_base == NULL || output_path == NULL) {
        return -1;
    }

    // Obtener nombre base del archivo
    const char* filename = strrchr(input_path, '/');
    if (filename == NULL) {
        const char* backslash_filename = strrchr(input_path, '\\');
         if (backslash_filename == NULL) {
            filename = input_path;
         } else {
             filename = backslash_filename + 1;
         }
    } else {
        filename++;
    }

    int written = snprintf(output_path, max_length, "%s/%s", output_base, filename);

    if (written < 0 || (size_t)written >= max_length) {
        return -1;
    }

    return 0;
}
