#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h>

/**
 * @file memory_manager.h
 * @brief Sistema de gestión de memoria para evitar fugas
 */

/**
 * @brief Asigna memoria y la registra para tracking
 *
 * @param size Tamaño en bytes a asignar
 * @param file Archivo donde se hace la asignación
 * @param line Línea donde se hace la asignación
 * @return Puntero a memoria asignada o NULL si falla
 */
void* tracked_malloc(size_t size, const char* file, int line);

/**
 * @brief Libera memoria previamente asignada con tracked_malloc
 *
 * @param ptr Puntero a liberar
 * @param file Archivo donde se hace la liberación
 * @param line Línea donde se hace la liberación
 */
void tracked_free(void* ptr, const char* file, int line);

/**
 * @brief Reasigna memoria y la registra
 *
 * @param ptr Puntero existente
 * @param new_size Nuevo tamaño
 * @param file Archivo donde se hace la reasignación
 * @param line Línea donde se hace la reasignación
 * @return Puntero a memoria reasignada o NULL si falla
 */
void* tracked_realloc(void* ptr, size_t new_size, const char* file, int line);

/**
 * @brief Imprime reporte de memoria asignada no liberada
 */
void print_memory_leaks(void);

/**
 * @brief Obtiene total de bytes asignados actualmente
 *
 * @return Total de bytes en uso
 */
size_t get_allocated_memory(void);

/**
 * @brief Libera toda la memoria tracked (útil al cerrar programa)
 */
void cleanup_all_memory(void);

/**
 * @brief Macros para facilitar el uso
 */
#ifdef DEBUG_MEMORY
    #define MALLOC(size) tracked_malloc(size, __FILE__, __LINE__)
    #define FREE(ptr) tracked_free(ptr, __FILE__, __LINE__)
    #define REALLOC(ptr, size) tracked_realloc(ptr, size, __FILE__, __LINE__)
#else
    #define MALLOC(size) malloc(size)
    #define FREE(ptr) free(ptr)
    #define REALLOC(ptr, size) realloc(ptr, size)
#endif

#endif // MEMORY_MANAGER_H
