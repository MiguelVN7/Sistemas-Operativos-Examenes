#include "rle.h"
#include <stdlib.h>
#include <string.h>

/**
 * @file rle.c
 * @brief Implementación del algoritmo Run-Length Encoding
 *
 * RLE comprime secuencias de datos repetidos almacenando el valor
 * y el número de repeticiones.
 */

unsigned char* rle_compress(const unsigned char* input, size_t input_size, size_t* output_size) {
    if (!input || input_size == 0 || !output_size) {
        return NULL;
    }

    // Peor caso: cada byte diferente = 2 bytes en salida (valor + count=1)
    unsigned char* output = (unsigned char*)malloc(input_size * 2);
    if (!output) {
        return NULL;
    }

    size_t out_idx = 0;
    size_t i = 0;

    while (i < input_size) {
        unsigned char current = input[i];
        size_t count = 1;

        // Contar repeticiones (máximo 255)
        while (i + count < input_size && input[i + count] == current && count < 255) {
            count++;
        }

        // Guardar: byte + count
        output[out_idx++] = current;
        output[out_idx++] = (unsigned char)count;

        i += count;
    }

    *output_size = out_idx;
    return output;
}

unsigned char* rle_decompress(const unsigned char* input, size_t input_size, size_t* output_size) {
    if (!input || input_size == 0 || input_size % 2 != 0 || !output_size) {
        return NULL;
    }

    // Calcular tamaño de salida
    size_t total_size = 0;
    for (size_t i = 1; i < input_size; i += 2) {
        total_size += input[i];
    }

    unsigned char* output = (unsigned char*)malloc(total_size);
    if (!output) {
        return NULL;
    }

    size_t out_idx = 0;
    for (size_t i = 0; i < input_size; i += 2) {
        unsigned char value = input[i];
        unsigned char count = input[i + 1];

        for (unsigned char j = 0; j < count; j++) {
            output[out_idx++] = value;
        }
    }

    *output_size = total_size;
    return output;
}
