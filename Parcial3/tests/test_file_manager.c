#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/utils/error_handler.h"
#include "../src/utils/memory_manager.h"

/**
 * @file test_file_manager.c
 * @brief Pruebas para utilidades del sistema
 */

void test_error_handler() {
    printf("Testing error handler...\n");

    clear_error();
    assert(get_last_error() == ERROR_NONE);

    LOG_ERROR(ERROR_FILE_NOT_FOUND, SEVERITY_ERROR, "Test error message");
    assert(get_last_error() == ERROR_FILE_NOT_FOUND);

    const char* msg = error_to_string(ERROR_FILE_NOT_FOUND);
    assert(msg != NULL);
    assert(strlen(msg) > 0);
    printf("  Error message: %s\n", msg);

    clear_error();
    assert(get_last_error() == ERROR_NONE);

    printf("  ✓ Error handler test passed\n");
}

void test_memory_manager() {
    printf("Testing memory manager...\n");

    #ifdef DEBUG_MEMORY
    size_t initial_mem = get_allocated_memory();

    void* ptr1 = MALLOC(100);
    assert(ptr1 != NULL);
    assert(get_allocated_memory() == initial_mem + 100);

    void* ptr2 = MALLOC(200);
    assert(ptr2 != NULL);
    assert(get_allocated_memory() == initial_mem + 300);

    FREE(ptr1);
    assert(get_allocated_memory() == initial_mem + 200);

    FREE(ptr2);
    assert(get_allocated_memory() == initial_mem);

    printf("  ✓ Memory manager test passed\n");
    #else
    printf("  ⚠ Memory manager test skipped (DEBUG_MEMORY not defined)\n");
    printf("  Hint: compile with 'make DEBUG=1' to enable memory tracking\n");
    #endif
}

void test_error_codes() {
    printf("Testing all error codes...\n");

    ErrorCode codes[] = {
        ERROR_NONE,
        ERROR_INVALID_ARGUMENT,
        ERROR_MEMORY_ALLOCATION,
        ERROR_FILE_NOT_FOUND,
        ERROR_FILE_READ,
        ERROR_FILE_WRITE,
        ERROR_COMPRESSION_FAILED,
        ERROR_DECOMPRESSION_FAILED,
        ERROR_ENCRYPTION_FAILED,
        ERROR_DECRYPTION_FAILED,
        ERROR_THREAD_CREATION,
        ERROR_MUTEX_INIT,
        ERROR_UNKNOWN
    };

    int num_codes = sizeof(codes) / sizeof(codes[0]);

    for (int i = 0; i < num_codes; i++) {
        const char* msg = error_to_string(codes[i]);
        assert(msg != NULL);
        assert(strlen(msg) > 0);
    }

    printf("  Tested %d error codes\n", num_codes);
    printf("  ✓ Error codes test passed\n");
}

int main() {
    printf("\n=== Utilities Module Tests ===\n\n");

    test_error_handler();
    test_error_codes();
    test_memory_manager();

    printf("\n=== All utilities tests completed ===\n\n");

    #ifdef DEBUG_MEMORY
    print_memory_leaks();
    #endif

    return 0;
}
