#include "rle.h"
#include <string.h>

/**
 * @brief Comprime datos usando Run-Length Encoding (RLE)
 * Formato: [Count][Byte]
 */
ssize_t rle_compress(const unsigned char* input, size_t input_size,
                    unsigned char* output, size_t output_capacity) {
    if (input == NULL || output == NULL) {
        return -1;
    }

    size_t in_idx = 0;
    size_t out_idx = 0;

    while (in_idx < input_size) {
        unsigned char current_byte = input[in_idx];
        unsigned int count = 1;

        // Contar repeticiones (max 255)
        while ((in_idx + count < input_size) && 
               (input[in_idx + count] == current_byte) && 
               (count < 255)) {
            count++;
        }

        // Verificar capacidad de salida (necesitamos 2 bytes: count + byte)
        if (out_idx + 2 > output_capacity) {
            return -1; // Buffer insuficiente
        }

        // Escribir par [Count][Byte]
        output[out_idx++] = (unsigned char)count;
        output[out_idx++] = current_byte;

        in_idx += count;
    }

    return out_idx;
}

/**
 * @brief Descomprime datos RLE
 */
ssize_t rle_decompress(const unsigned char* input, size_t input_size,
                      unsigned char* output, size_t output_capacity) {
    if (input == NULL || output == NULL) {
        return -1;
    }

    // El tamaño de entrada debe ser par (pares Count+Byte)
    if (input_size % 2 != 0) {
        return -1;
    }

    size_t in_idx = 0;
    size_t out_idx = 0;

    while (in_idx < input_size) {
        unsigned char count = input[in_idx++];
        unsigned char byte = input[in_idx++];

        // Verificar capacidad
        if (out_idx + count > output_capacity) {
            return -1; // Buffer insuficiente
        }

        // Escribir 'count' veces el 'byte'
        memset(output + out_idx, byte, count);
        out_idx += count;
    }

    return out_idx;
}
