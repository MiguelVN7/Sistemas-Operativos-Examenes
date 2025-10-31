#include "concurrency.h"
#include "utils/error_handler.h"
#include "utils/memory_manager.h"
#include "compression/compression.h"
#include "encryption/encryption.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void* process_file_thread(void* arg) {
    FileTask* task = (FileTask*)arg;

    if (task->verbose) {
        printf("[Thread %d] Procesando: %s\n", task->thread_id, task->input_path);
    }

    task->success = process_single_file(task->input_path, task->output_path,
                                       &(Arguments){
                                           .operations = task->operations,
                                           .compression_alg = task->compression_alg,
                                           .encryption_alg = task->encryption_alg,
                                           .key = task->key,
                                           .verbose = task->verbose
                                       });

    if (task->verbose) {
        if (task->success == 0) {
            printf("[Thread %d] Completado: %s\n", task->thread_id, task->output_path);
        } else {
            fprintf(stderr, "[Thread %d] Error procesando: %s\n", task->thread_id, task->input_path);
        }
    }

    return NULL;
}

int process_single_file(const char* input_path, const char* output_path,
                       const Arguments* args) {
    FileBuffer* input_buffer = create_file_buffer(BUFFER_SIZE);
    if (input_buffer == NULL) {
        return -1;
    }

    if (read_file(input_path, input_buffer) < 0) {
        free_file_buffer(input_buffer);
        return -1;
    }

    FileBuffer* current_buffer = input_buffer;
    FileBuffer* temp_buffer = NULL;

    // Comprimir si se requiere
    if (args->operations & OP_COMPRESS) {
        temp_buffer = create_file_buffer(current_buffer->size * 2);
        if (temp_buffer == NULL) {
            free_file_buffer(current_buffer);
            return -1;
        }

        ssize_t result = compress_data(current_buffer->data, current_buffer->size,
                                      temp_buffer, args->compression_alg);
        if (result < 0) {
            free_file_buffer(current_buffer);
            free_file_buffer(temp_buffer);
            return -1;
        }

        temp_buffer->size = result;
        if (current_buffer != input_buffer) {
            free_file_buffer(current_buffer);
        }
        current_buffer = temp_buffer;
    }

    // Descomprimir si se requiere
    if (args->operations & OP_DECOMPRESS) {
        temp_buffer = create_file_buffer(current_buffer->size * 4);
        if (temp_buffer == NULL) {
            free_file_buffer(current_buffer);
            return -1;
        }

        ssize_t result = decompress_data(current_buffer->data, current_buffer->size,
                                        temp_buffer, args->compression_alg);
        if (result < 0) {
            free_file_buffer(current_buffer);
            free_file_buffer(temp_buffer);
            return -1;
        }

        temp_buffer->size = result;
        if (current_buffer != input_buffer) {
            free_file_buffer(current_buffer);
        }
        current_buffer = temp_buffer;
    }

    // Encriptar si se requiere
    if (args->operations & OP_ENCRYPT) {
        temp_buffer = create_file_buffer(current_buffer->size + 256);
        if (temp_buffer == NULL) {
            free_file_buffer(current_buffer);
            return -1;
        }

        ssize_t result = encrypt_data(current_buffer->data, current_buffer->size,
                                     temp_buffer, args->encryption_alg,
                                     (unsigned char*)args->key, strlen(args->key));
        if (result < 0) {
            free_file_buffer(current_buffer);
            free_file_buffer(temp_buffer);
            return -1;
        }

        temp_buffer->size = result;
        if (current_buffer != input_buffer) {
            free_file_buffer(current_buffer);
        }
        current_buffer = temp_buffer;
    }

    // Desencriptar si se requiere
    if (args->operations & OP_DECRYPT) {
        temp_buffer = create_file_buffer(current_buffer->size);
        if (temp_buffer == NULL) {
            free_file_buffer(current_buffer);
            return -1;
        }

        ssize_t result = decrypt_data(current_buffer->data, current_buffer->size,
                                     temp_buffer, args->encryption_alg,
                                     (unsigned char*)args->key, strlen(args->key));
        if (result < 0) {
            free_file_buffer(current_buffer);
            free_file_buffer(temp_buffer);
            return -1;
        }

        temp_buffer->size = result;
        if (current_buffer != input_buffer) {
            free_file_buffer(current_buffer);
        }
        current_buffer = temp_buffer;
    }

    // Escribir resultado
    if (write_file(output_path, current_buffer) < 0) {
        free_file_buffer(current_buffer);
        return -1;
    }

    free_file_buffer(current_buffer);
    return 0;
}

int process_files_concurrent(FileList* file_list, const Arguments* args) {
    if (file_list == NULL || file_list->count == 0) {
        return -1;
    }

    ThreadPool* pool = create_thread_pool(file_list->count);
    if (pool == NULL) {
        return -1;
    }

    for (size_t i = 0; i < file_list->count; i++) {
        if (file_list->files[i].is_directory) {
            continue;
        }

        FileTask task;
        strncpy(task.input_path, file_list->files[i].path, MAX_PATH_LENGTH - 1);
        task.input_path[MAX_PATH_LENGTH - 1] = '\0';

        build_output_path(task.input_path, args->output_path,
                         task.output_path, MAX_PATH_LENGTH);

        task.operations = args->operations;
        task.compression_alg = args->compression_alg;
        task.encryption_alg = args->encryption_alg;
        task.key = args->key;
        task.verbose = args->verbose;
        task.thread_id = i;
        task.success = 0;

        add_task_to_pool(pool, &task);
    }

    int result = wait_all_threads(pool);
    int errors = pool->errors;

    free_thread_pool(pool);

    return errors > 0 ? -1 : result;
}

ThreadPool* create_thread_pool(int max_threads) {
    ThreadPool* pool = MALLOC(sizeof(ThreadPool));
    if (pool == NULL) {
        return NULL;
    }

    if (max_threads > MAX_THREADS) {
        max_threads = MAX_THREADS;
    }

    pool->tasks = MALLOC(sizeof(FileTask) * max_threads);
    if (pool->tasks == NULL) {
        FREE(pool);
        return NULL;
    }

    pool->task_count = 0;
    pool->active_threads = 0;
    pool->errors = 0;

    if (pthread_mutex_init(&pool->mutex, NULL) != 0) {
        FREE(pool->tasks);
        FREE(pool);
        return NULL;
    }

    return pool;
}

int add_task_to_pool(ThreadPool* pool, const FileTask* task) {
    if (pool == NULL || task == NULL || pool->active_threads >= MAX_THREADS) {
        return -1;
    }

    memcpy(&pool->tasks[pool->task_count], task, sizeof(FileTask));

    if (pthread_create(&pool->threads[pool->active_threads], NULL,
                      process_file_thread, &pool->tasks[pool->task_count]) != 0) {
        LOG_ERROR(ERROR_THREAD_CREATION, SEVERITY_ERROR, "Failed to create thread");
        pool->errors++;
        return -1;
    }

    pool->active_threads++;
    pool->task_count++;

    return 0;
}

int wait_all_threads(ThreadPool* pool) {
    if (pool == NULL) {
        return -1;
    }

    for (int i = 0; i < pool->active_threads; i++) {
        pthread_join(pool->threads[i], NULL);

        if (pool->tasks[i].success != 0) {
            pool->errors++;
        }
    }

    return pool->errors > 0 ? -1 : 0;
}

void free_thread_pool(ThreadPool* pool) {
    if (pool != NULL) {
        pthread_mutex_destroy(&pool->mutex);
        if (pool->tasks != NULL) {
            FREE(pool->tasks);
        }
        FREE(pool);
    }
}
