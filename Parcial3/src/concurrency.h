#ifndef CONCURRENCY_H
#define CONCURRENCY_H

#include "file_manager.h"
#include "args_parser.h"
#include <pthread.h>

/**
 * @file concurrency.h
 * @brief Sistema de concurrencia para procesamiento paralelo de archivos
 */

#define MAX_THREADS 64

typedef struct {
    char input_path[MAX_PATH_LENGTH];
    char output_path[MAX_PATH_LENGTH];
    Operation operations;
    CompressionAlgorithm compression_alg;
    EncryptionAlgorithm encryption_alg;
    char* key;
    int verbose;
    int success;
    int thread_id;
} FileTask;

typedef struct {
    pthread_t threads[MAX_THREADS];
    FileTask* tasks;
    int task_count;
    int active_threads;
    pthread_mutex_t mutex;
    int errors;
} ThreadPool;

/**
 * @brief Procesa un archivo usando el thread actual
 */
void* process_file_thread(void* arg);

/**
 * @brief Procesa múltiples archivos en paralelo
 */
int process_files_concurrent(FileList* file_list, const Arguments* args);

/**
 * @brief Procesa un solo archivo (versión secuencial)
 */
int process_single_file(const char* input_path, const char* output_path,
                       const Arguments* args);

/**
 * @brief Inicializa el pool de threads
 */
ThreadPool* create_thread_pool(int max_threads);

/**
 * @brief Agrega una tarea al pool
 */
int add_task_to_pool(ThreadPool* pool, const FileTask* task);

/**
 * @brief Espera a que todos los threads terminen
 */
int wait_all_threads(ThreadPool* pool);

/**
 * @brief Libera el pool de threads
 */
void free_thread_pool(ThreadPool* pool);

#endif // CONCURRENCY_H
