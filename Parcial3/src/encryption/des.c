#include "des.h"
#include <string.h>

/**
 * DES (Data Encryption Standard) - Implementación simplificada
 * Esta es una versión básica para el avance
 * TODO: Implementar DES completo con sus rondas y permutaciones
 */

static void simple_des_round(unsigned char* block, const unsigned char* key, size_t key_len) {
    for (size_t i = 0; i < 8; i++) {
        block[i] ^= key[i % key_len];
        block[i] = ((block[i] << 1) | (block[i] >> 7));  // Rotación
    }
}

ssize_t des_encrypt(const unsigned char* input, size_t input_size,
                   unsigned char* output, const unsigned char* key, size_t key_len) {
    if (input == NULL || output == NULL || key == NULL || key_len == 0) {
        return -1;
    }

    // Procesar en bloques de 8 bytes
    size_t i;
    for (i = 0; i + 8 <= input_size; i += 8) {
        memcpy(output + i, input + i, 8);
        
        // 4 rondas simplificadas
        for (int round = 0; round < 4; round++) {
            simple_des_round(output + i, key, key_len);
        }
    }

    // Procesar bytes restantes
    if (i < input_size) {
        unsigned char block[8] = {0};
        size_t remaining = input_size - i;
        memcpy(block, input + i, remaining);
        
        for (int round = 0; round < 4; round++) {
            simple_des_round(block, key, key_len);
        }
        
        memcpy(output + i, block, remaining);
        i += remaining;
    }

    return i;
}

ssize_t des_decrypt(const unsigned char* input, size_t input_size,
                   unsigned char* output, const unsigned char* key, size_t key_len) {
    if (input == NULL || output == NULL || key == NULL || key_len == 0) {
        return -1;
    }

    // Para desencriptar, aplicamos las operaciones inversas
    size_t i;
    for (i = 0; i + 8 <= input_size; i += 8) {
        memcpy(output + i, input + i, 8);
        
        // Rondas en orden inverso
        for (int round = 0; round < 4; round++) {
            // Rotación inversa
            for (size_t j = 0; j < 8; j++) {
                output[i + j] = ((output[i + j] >> 1) | (output[i + j] << 7));
                output[i + j] ^= key[j % key_len];
            }
        }
    }

    // Procesar bytes restantes
    if (i < input_size) {
        unsigned char block[8] = {0};
        size_t remaining = input_size - i;
        memcpy(block, input + i, remaining);
        
        for (int round = 0; round < 4; round++) {
            for (size_t j = 0; j < 8; j++) {
                block[j] = ((block[j] >> 1) | (block[j] << 7));
                block[j] ^= key[j % key_len];
            }
        }
        
        memcpy(output + i, block, remaining);
        i += remaining;
    }

    return i;
}
