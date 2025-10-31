#include "memory_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @file memory_manager.c
 * @brief Implementación del sistema de gestión de memoria
 *
 * Mantiene un registro de todas las asignaciones de memoria
 * para detectar fugas y facilitar debugging.
 */

#define MAX_ALLOCATIONS 10000

typedef struct {
    void* ptr;
    size_t size;
    const char* file;
    int line;
    int active;
} AllocationRecord;

static AllocationRecord allocations[MAX_ALLOCATIONS];
static size_t total_allocated = 0;
static size_t allocation_count = 0;

void* tracked_malloc(size_t size, const char* file, int line) {
    void* ptr = malloc(size);

    if (!ptr) {
        fprintf(stderr, "Memory allocation failed: %s:%d (size: %zu)\n",
                file, line, size);
        return NULL;
    }

    // Registrar asignación
    for (size_t i = 0; i < MAX_ALLOCATIONS; i++) {
        if (!allocations[i].active) {
            allocations[i].ptr = ptr;
            allocations[i].size = size;
            allocations[i].file = file;
            allocations[i].line = line;
            allocations[i].active = 1;

            total_allocated += size;
            allocation_count++;
            break;
        }
    }

    return ptr;
}

void tracked_free(void* ptr, const char* file, int line) {
    if (!ptr) {
        return;
    }

    // Buscar y marcar como liberado
    int found = 0;
    for (size_t i = 0; i < MAX_ALLOCATIONS; i++) {
        if (allocations[i].active && allocations[i].ptr == ptr) {
            total_allocated -= allocations[i].size;
            allocations[i].active = 0;
            found = 1;
            break;
        }
    }

    if (!found) {
        fprintf(stderr, "Warning: Attempted to free untracked pointer at %s:%d\n",
                file, line);
    }

    free(ptr);
}

void* tracked_realloc(void* ptr, size_t new_size, const char* file, int line) {
    // Buscar registro existente
    size_t old_size = 0;
    int idx = -1;

    for (size_t i = 0; i < MAX_ALLOCATIONS; i++) {
        if (allocations[i].active && allocations[i].ptr == ptr) {
            old_size = allocations[i].size;
            idx = i;
            break;
        }
    }

    void* new_ptr = realloc(ptr, new_size);

    if (!new_ptr) {
        fprintf(stderr, "Memory reallocation failed: %s:%d (size: %zu)\n",
                file, line, new_size);
        return NULL;
    }

    // Actualizar registro
    if (idx >= 0) {
        total_allocated = total_allocated - old_size + new_size;
        allocations[idx].ptr = new_ptr;
        allocations[idx].size = new_size;
        allocations[idx].file = file;
        allocations[idx].line = line;
    } else {
        // Nuevo registro
        tracked_malloc(new_size, file, line);
    }

    return new_ptr;
}

void print_memory_leaks(void) {
    int leak_count = 0;
    size_t leaked_bytes = 0;

    printf("\n=== Memory Leak Report ===\n");

    for (size_t i = 0; i < MAX_ALLOCATIONS; i++) {
        if (allocations[i].active) {
            printf("LEAK: %zu bytes at %p (allocated at %s:%d)\n",
                   allocations[i].size,
                   allocations[i].ptr,
                   allocations[i].file,
                   allocations[i].line);
            leak_count++;
            leaked_bytes += allocations[i].size;
        }
    }

    if (leak_count == 0) {
        printf("No memory leaks detected!\n");
    } else {
        printf("\nTotal: %d leaks, %zu bytes\n", leak_count, leaked_bytes);
    }

    printf("Total allocations made: %zu\n", allocation_count);
    printf("==========================\n\n");
}

size_t get_allocated_memory(void) {
    return total_allocated;
}

void cleanup_all_memory(void) {
    for (size_t i = 0; i < MAX_ALLOCATIONS; i++) {
        if (allocations[i].active) {
            free(allocations[i].ptr);
            allocations[i].active = 0;
        }
    }
    total_allocated = 0;
}
