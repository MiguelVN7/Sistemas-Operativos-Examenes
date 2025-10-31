#include "rle.h"
#include <string.h>

/**
 * RLE (Run-Length Encoding)
 * Comprime secuencias repetidas de bytes
 * Formato: [count][byte] donde count es el número de repeticiones
 */

ssize_t rle_compress(const unsigned char* input, size_t input_size,
                    unsigned char* output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
    }

    size_t out_pos = 0;
    size_t i = 0;

    while (i < input_size && out_pos < output_capacity - 2) {
        unsigned char current = input[i];
        unsigned char count = 1;

        // Contar repeticiones (máximo 255)
        while (i + count < input_size && 
               input[i + count] == current && 
               count < 255) {
            count++;
        }

        // Escribir count y byte
        output[out_pos++] = count;
        output[out_pos++] = current;

        i += count;
    }

    if (i < input_size) {
        return -1; // Buffer de salida insuficiente
    }

    return out_pos;
}

ssize_t rle_decompress(const unsigned char* input, size_t input_size,
                      unsigned char* output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
    }

    size_t out_pos = 0;
    size_t in_pos = 0;

    while (in_pos < input_size - 1) {
        unsigned char count = input[in_pos++];
        unsigned char byte = input[in_pos++];

        // Expandir la secuencia
        for (unsigned char j = 0; j < count; j++) {
            if (out_pos >= output_capacity) {
                return -1; // Buffer de salida insuficiente
            }
            output[out_pos++] = byte;
        }
    }

    return out_pos;
}
