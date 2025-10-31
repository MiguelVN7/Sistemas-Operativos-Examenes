#include "rle.h"
#include <string.h>

/**
 * RLE (Run-Length Encoding)
 * Comprime secuencias repetidas de bytes
 * Formato: [count][byte] donde count es el número de repeticiones
 * 
 * TODO: Implementar compresión completa
 * TODO: Manejar casos edge (datos sin repetición)
 */

ssize_t rle_compress(const unsigned char* input, size_t input_size,
                    unsigned char* output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
    }

    // TODO: Implementar algoritmo RLE
    // Formato sugerido: [count][byte]
    // count = número de repeticiones (1-255)
    
    return -1;
}

ssize_t rle_decompress(const unsigned char* input, size_t input_size,
                      unsigned char* output, size_t output_capacity) {
    if (input == NULL || output == NULL || input_size == 0) {
        return -1;
    }

    // TODO: Implementar descompresión RLE
    // Leer [count][byte] y expandir
    
    return -1;
}
