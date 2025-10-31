#include "lzw.h"
#include <stdlib.h>
#include <string.h>

/**
 * @file lzw.c
 * @brief Implementación del algoritmo Lempel-Ziv-Welch
 *
 * Este es un esqueleto básico. La implementación completa requiere:
 * - Diccionario dinámico de secuencias
 * - Codificación de índices de diccionario
 * - Manejo de diccionario lleno
 */

unsigned char* lzw_compress(const unsigned char* input, size_t input_size, size_t* output_size) {
    if (!input || input_size == 0 || !output_size) {
        return NULL;
    }

    // TODO: Implementar algoritmo LZW completo
    // Por ahora, placeholder que copia los datos
    unsigned char* output = (unsigned char*)malloc(input_size);
    if (!output) {
        return NULL;
    }

    memcpy(output, input, input_size);
    *output_size = input_size;

    return output;
}

unsigned char* lzw_decompress(const unsigned char* input, size_t input_size, size_t* output_size) {
    if (!input || input_size == 0 || !output_size) {
        return NULL;
    }

    // TODO: Implementar descompresión LZW completa
    // Por ahora, placeholder que copia los datos
    unsigned char* output = (unsigned char*)malloc(input_size);
    if (!output) {
        return NULL;
    }

    memcpy(output, input, input_size);
    *output_size = input_size;

    return output;
}
