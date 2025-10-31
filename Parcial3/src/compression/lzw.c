#include "lzw.h"
#include <stdlib.h>
#include <string.h>

/**
 * LZW (Lempel-Ziv-Welch) - Implementación simplificada
 * Este es un stub básico para el avance
 * TODO: Implementar diccionario completo de LZW
 */

#define MAX_DICT_SIZE 4096

ssize_t lzw_compress(const unsigned char* input, size_t input_size,
                    unsigned char* output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
    }

    // Implementación temporal - solo copia
    // TODO: Implementar algoritmo completo de LZW con diccionario
    if (input_size > output_capacity) {
        return -1;
    }

    memcpy(output, input, input_size);
    
    return input_size;
}

ssize_t lzw_decompress(const unsigned char* input, size_t input_size,
                      unsigned char* output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
    }

    // Implementación temporal - solo copia
    if (input_size > output_capacity) {
        return -1;
    }

    memcpy(output, input, input_size);
    
    return input_size;
}
