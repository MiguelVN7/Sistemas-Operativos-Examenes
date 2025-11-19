#include "concurrency.h"
#include "utils/error_handler.h"
#include "compression/rle.h"
#include "encryption/vigenere.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Función interna para procesar el buffer en memoria
 */
static int process_buffer(FileBuffer** buffer, const FileTask* task) {
    FileBuffer* input = *buffer;
    FileBuffer* output = NULL;
    
    // Pipeline: Decrypt -> Decompress -> Compress -> Encrypt
    
    // 1. Desencriptar
    if (task->operations & OP_DECRYPT) {
        if (task->encryption_alg == ENC_VIGENERE) {
            output = create_file_buffer(input->size);
            if (!output) return -1;
            
            ssize_t res = vigenere_decrypt(input->data, input->size, output->data, 
                                         (unsigned char*)task->key, strlen(task->key));
            if (res < 0) {
                free_file_buffer(output);
                return -1;
            }
            output->size = res;
            
            // Swap buffers
            free_file_buffer(input);
            input = output;
        }
    }
    
    // 2. Descomprimir
    if (task->operations & OP_DECOMPRESS) {
        if (task->compression_alg == COMP_RLE) {
            // Estimar tamaño descomprimido (peor caso o suficiente)
            // RLE puede expandir hasta 2x en el peor caso, pero descomprimir puede ser grande.
            // Asumimos un factor razonable o usamos realloc dinámico si fuera complejo.
            // Por simplicidad, usaremos 10x input size o 1MB min.
            size_t estimated_size = input->size * 10;
            if (estimated_size < 1024*1024) estimated_size = 1024*1024;
            
            output = create_file_buffer(estimated_size);
            if (!output) return -1;
            
            ssize_t res = rle_decompress(input->data, input->size, output->data, output->capacity);
            if (res < 0) {
                free_file_buffer(output);
                return -1;
            }
            output->size = res;
            
            free_file_buffer(input);
            input = output;
        }
    }
    
    // 3. Comprimir
    if (task->operations & OP_COMPRESS) {
        if (task->compression_alg == COMP_RLE) {
            // RLE peor caso: 2x tamaño original
            output = create_file_buffer(input->size * 2 + 1024);
            if (!output) return -1;
            
            ssize_t res = rle_compress(input->data, input->size, output->data, output->capacity);
            if (res < 0) {
                free_file_buffer(output);
                return -1;
            }
            output->size = res;
            
            free_file_buffer(input);
            input = output;
        }
    }
    
    // 4. Encriptar
    if (task->operations & OP_ENCRYPT) {
        if (task->encryption_alg == ENC_VIGENERE) {
            output = create_file_buffer(input->size);
            if (!output) return -1;
            
            ssize_t res = vigenere_encrypt(input->data, input->size, output->data, 
                                         (unsigned char*)task->key, strlen(task->key));
            if (res < 0) {
                free_file_buffer(output);
                return -1;
            }
            output->size = res;
            
            free_file_buffer(input);
            input = output;
        }
    }
    
    *buffer = input;
    return 0;
}

/**
 * @brief Función que ejecuta un thread para procesar un archivo
 */
void* process_file_thread(void* arg) {
    FileTask* task = (FileTask*)arg;

    if (task->verbose) {
        printf("[Thread %d] Procesando %s -> %s\n", task->thread_id, task->input_path, task->output_path);
    }

    // Leer archivo
    FileBuffer* buffer = create_file_buffer(8192); // Inicial
    if (!buffer) {
        task->success = 0;
        return NULL;
    }
    
    ssize_t read_bytes = read_file(task->input_path, buffer);
    if (read_bytes < 0) {
        free_file_buffer(buffer);
        task->success = 0;
        return NULL;
    }
    
    // Procesar
    if (process_buffer(&buffer, task) != 0) {
        free_file_buffer(buffer);
        task->success = 0;
        return NULL;
    }
    
    // Escribir archivo
    if (write_file(task->output_path, buffer) < 0) {
        free_file_buffer(buffer);
        task->success = 0;
        return NULL;
    }
    
    free_file_buffer(buffer);
    task->success = 1;

    if (task->verbose) {
        printf("[Thread %d] Completado exitosamente.\n", task->thread_id);
    }

    return NULL;
}

/**
 * @brief Procesa un solo archivo
 */
int process_single_file(const char* input_path, const char* output_path,
                       const Arguments* args) {
    if (input_path == NULL || output_path == NULL || args == NULL) {
        return -1;
    }

    FileTask task;
    // Usar memcpy para evitar warnings de strncpy
    size_t input_len = strlen(input_path);
    if (input_len >= MAX_PATH_LENGTH) input_len = MAX_PATH_LENGTH - 1;
    memcpy(task.input_path, input_path, input_len);
    task.input_path[input_len] = '\0';

    size_t output_len = strlen(output_path);
    if (output_len >= MAX_PATH_LENGTH) output_len = MAX_PATH_LENGTH - 1;
    memcpy(task.output_path, output_path, output_len);
    task.output_path[output_len] = '\0';
    task.operations = args->operations;
    task.compression_alg = args->compression_alg;
    task.encryption_alg = args->encryption_alg;
    task.key = args->key;
    task.verbose = args->verbose;
    task.thread_id = 0;
    task.success = 0;
    
    process_file_thread(&task);
    
    return task.success ? 0 : -1;
}

/**
 * @brief Procesa múltiples archivos en paralelo
 */
int process_files_concurrent(FileList* file_list, const Arguments* args) {
    if (file_list == NULL || file_list->count == 0) {
        return -1;
    }
    
    int total_files = file_list->count;
    int processed_count = 0;
    
    // Procesar en lotes de MAX_THREADS
    while (processed_count < total_files) {
        ThreadPool* pool = create_thread_pool(MAX_THREADS);
        if (!pool) return -1;
        
        int batch_limit = total_files - processed_count;
        if (batch_limit > MAX_THREADS) batch_limit = MAX_THREADS;
        
        for (int i = 0; i < batch_limit; i++) {
            FileInfo* file_info = &file_list->files[processed_count + i];
            
            FileTask task;
            // Usar memcpy para evitar warnings de strncpy
            size_t len = strlen(file_info->path);
            if (len >= MAX_PATH_LENGTH) len = MAX_PATH_LENGTH - 1;
            memcpy(task.input_path, file_info->path, len);
            task.input_path[len] = '\0';
            
    // Generar output path
    if (build_output_path(file_info->path, args->output_path, task.output_path, MAX_PATH_LENGTH) != 0) {
        continue; 
    }
            
            task.operations = args->operations;
            task.compression_alg = args->compression_alg;
            task.encryption_alg = args->encryption_alg;
            task.key = args->key;
            task.verbose = args->verbose;
            task.thread_id = i;
            task.success = 0;
            
            add_task_to_pool(pool, &task);
        }
        
        wait_all_threads(pool);
        free_thread_pool(pool);
        
        processed_count += batch_limit;
    }

    return 0;
}

/**
 * @brief Crea un pool de threads
 */
ThreadPool* create_thread_pool(int max_threads) {
    ThreadPool* pool = malloc(sizeof(ThreadPool));
    if (pool == NULL) {
        return NULL;
    }

    if (max_threads > MAX_THREADS) {
        max_threads = MAX_THREADS;
    }

    pool->tasks = malloc(sizeof(FileTask) * max_threads);
    if (pool->tasks == NULL) {
        free(pool);
        return NULL;
    }

    pool->task_count = 0;
    pool->active_threads = 0;
    pool->errors = 0;

    if (pthread_mutex_init(&pool->mutex, NULL) != 0) {
        free(pool->tasks);
        free(pool);
        return NULL;
    }

    return pool;
}

/**
 * @brief Agrega una tarea al pool y lanza el thread
 */
int add_task_to_pool(ThreadPool* pool, const FileTask* task) {
    if (pool == NULL || task == NULL || pool->active_threads >= MAX_THREADS) {
        return -1;
    }

    pthread_mutex_lock(&pool->mutex);
    
    int index = pool->task_count;
    memcpy(&pool->tasks[index], task, sizeof(FileTask));
    pool->task_count++;
    pool->active_threads++; // Incrementamos antes de crear para reservar
    
    // Lanzar thread
    if (pthread_create(&pool->threads[index], NULL, process_file_thread, &pool->tasks[index]) != 0) {
        pool->active_threads--;
        pool->task_count--;
        pool->errors++;
        pthread_mutex_unlock(&pool->mutex);
        return -1;
    }

    pthread_mutex_unlock(&pool->mutex);
    return 0;
}

/**
 * @brief Espera a que todos los threads terminen
 */
int wait_all_threads(ThreadPool* pool) {
    if (pool == NULL) {
        return -1;
    }

    for (int i = 0; i < pool->task_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    return pool->errors > 0 ? -1 : 0;
}

/**
 * @brief Libera el pool de threads
 */
void free_thread_pool(ThreadPool* pool) {
    if (pool != NULL) {
        pthread_mutex_destroy(&pool->mutex);
        if (pool->tasks != NULL) {
            free(pool->tasks);
        }
        free(pool);
    }
}
