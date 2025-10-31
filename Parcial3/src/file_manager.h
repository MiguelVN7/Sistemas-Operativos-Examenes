#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <stddef.h>
#include <sys/types.h>

/**
 * @file file_manager.h
 * @brief Gestor de archivos usando syscalls directas del sistema operativo
 */

#define MAX_PATH_LENGTH 4096
#define BUFFER_SIZE 8192

typedef struct {
    char path[MAX_PATH_LENGTH];
    size_t size;
    int is_directory;
    mode_t mode;
} FileInfo;

typedef struct {
    unsigned char* data;
    size_t size;
    size_t capacity;
} FileBuffer;

typedef struct {
    FileInfo* files;
    size_t count;
    size_t capacity;
} FileList;

ssize_t read_file(const char* path, FileBuffer* buffer);
ssize_t write_file(const char* path, const FileBuffer* buffer);
int read_directory(const char* dir_path, FileList* file_list);
int get_file_info(const char* path, FileInfo* info);
int is_directory(const char* path);
int file_exists(const char* path);

FileBuffer* create_file_buffer(size_t initial_capacity);
int resize_file_buffer(FileBuffer* buffer, size_t new_capacity);
void free_file_buffer(FileBuffer* buffer);

FileList* create_file_list(void);
int add_file_to_list(FileList* list, const FileInfo* info);
void free_file_list(FileList* list);

int create_directory(const char* path);
int build_output_path(const char* input_path, const char* output_base,
                     char* output_path, size_t max_length);

#endif // FILE_MANAGER_H
