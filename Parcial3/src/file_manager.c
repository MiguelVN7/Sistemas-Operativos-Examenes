#include "file_manager.h"
#include "utils/error_handler.h"
#include "utils/memory_manager.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

ssize_t read_file(const char* path, FileBuffer* buffer) {
    if (path == NULL || buffer == NULL) {
        LOG_ERROR(ERROR_INVALID_ARGUMENT, SEVERITY_ERROR, "Invalid parameters");
        return -1;
    }

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        LOG_ERROR(ERROR_FILE_NOT_FOUND, SEVERITY_ERROR, path);
        return -1;
    }

    struct stat st;
    if (fstat(fd, &st) < 0) {
        close(fd);
        LOG_ERROR(ERROR_FILE_READ, SEVERITY_ERROR, "Cannot get file stats");
        return -1;
    }

    size_t file_size = st.st_size;

    if (buffer->capacity < file_size) {
        if (resize_file_buffer(buffer, file_size) < 0) {
            close(fd);
            return -1;
        }
    }

    ssize_t total_read = 0;
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer->data + total_read, file_size - total_read)) > 0) {
        total_read += bytes_read;
    }

    if (bytes_read < 0) {
        close(fd);
        LOG_ERROR(ERROR_FILE_READ, SEVERITY_ERROR, strerror(errno));
        return -1;
    }

    buffer->size = total_read;
    close(fd);

    return total_read;
}

ssize_t write_file(const char* path, const FileBuffer* buffer) {
    if (path == NULL || buffer == NULL) {
        LOG_ERROR(ERROR_INVALID_ARGUMENT, SEVERITY_ERROR, "Invalid parameters");
        return -1;
    }

    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        LOG_ERROR(ERROR_FILE_WRITE, SEVERITY_ERROR, path);
        return -1;
    }

    ssize_t total_written = 0;
    ssize_t bytes_written;

    while (total_written < (ssize_t)buffer->size) {
        bytes_written = write(fd, buffer->data + total_written,
                            buffer->size - total_written);
        if (bytes_written < 0) {
            close(fd);
            LOG_ERROR(ERROR_FILE_WRITE, SEVERITY_ERROR, strerror(errno));
            return -1;
        }
        total_written += bytes_written;
    }

    close(fd);
    return total_written;
}

int read_directory(const char* dir_path, FileList* file_list) {
    if (dir_path == NULL || file_list == NULL) {
        LOG_ERROR(ERROR_INVALID_ARGUMENT, SEVERITY_ERROR, "Invalid parameters");
        return -1;
    }

    DIR* dir = opendir(dir_path);
    if (dir == NULL) {
        LOG_ERROR(ERROR_DIRECTORY_ACCESS, SEVERITY_ERROR, dir_path);
        return -1;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[MAX_PATH_LENGTH];
        snprintf(full_path, MAX_PATH_LENGTH, "%s/%s", dir_path, entry->d_name);

        FileInfo info;
        if (get_file_info(full_path, &info) == 0) {
            add_file_to_list(file_list, &info);
        }
    }

    closedir(dir);
    return 0;
}

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
    info->mode = st.st_mode;
    info->is_directory = S_ISDIR(st.st_mode);

    return 0;
}

int is_directory(const char* path) {
    struct stat st;
    if (stat(path, &st) < 0) {
        return -1;
    }
    return S_ISDIR(st.st_mode) ? 1 : 0;
}

int file_exists(const char* path) {
    return access(path, F_OK) == 0 ? 1 : 0;
}

FileBuffer* create_file_buffer(size_t initial_capacity) {
    FileBuffer* buffer = MALLOC(sizeof(FileBuffer));
    if (buffer == NULL) {
        return NULL;
    }

    buffer->data = MALLOC(initial_capacity);
    if (buffer->data == NULL) {
        FREE(buffer);
        return NULL;
    }

    buffer->size = 0;
    buffer->capacity = initial_capacity;

    return buffer;
}

int resize_file_buffer(FileBuffer* buffer, size_t new_capacity) {
    if (buffer == NULL) {
        return -1;
    }

    unsigned char* new_data = REALLOC(buffer->data, new_capacity);
    if (new_data == NULL) {
        LOG_ERROR(ERROR_MEMORY_ALLOCATION, SEVERITY_ERROR, "Failed to resize buffer");
        return -1;
    }

    buffer->data = new_data;
    buffer->capacity = new_capacity;

    return 0;
}

void free_file_buffer(FileBuffer* buffer) {
    if (buffer != NULL) {
        if (buffer->data != NULL) {
            FREE(buffer->data);
        }
        FREE(buffer);
    }
}

FileList* create_file_list(void) {
    FileList* list = MALLOC(sizeof(FileList));
    if (list == NULL) {
        return NULL;
    }

    list->capacity = 16;
    list->files = MALLOC(sizeof(FileInfo) * list->capacity);
    if (list->files == NULL) {
        FREE(list);
        return NULL;
    }

    list->count = 0;
    return list;
}

int add_file_to_list(FileList* list, const FileInfo* info) {
    if (list == NULL || info == NULL) {
        return -1;
    }

    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        FileInfo* new_files = REALLOC(list->files, sizeof(FileInfo) * new_capacity);
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

void free_file_list(FileList* list) {
    if (list != NULL) {
        if (list->files != NULL) {
            FREE(list->files);
        }
        FREE(list);
    }
}

int create_directory(const char* path) {
    if (path == NULL) {
        return -1;
    }

    if (mkdir(path, 0755) < 0) {
        if (errno != EEXIST) {
            LOG_ERROR(ERROR_FILE_WRITE, SEVERITY_ERROR, "Failed to create directory");
            return -1;
        }
    }

    return 0;
}

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
