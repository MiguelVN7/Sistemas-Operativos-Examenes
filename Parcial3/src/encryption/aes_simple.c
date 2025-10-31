#include "aes_simple.h"
#include <string.h>

/**
 * AES (Advanced Encryption Standard) - Implementación muy simplificada
 * Esta es una versión básica para el avance
 * TODO: Implementar AES completo con S-boxes, ShiftRows, MixColumns
 */

static void simple_aes_round(unsigned char* block, const unsigned char* key, size_t key_len) {
    // Versión simplificada - solo XOR y permutaciones básicas
    for (size_t i = 0; i < 16; i++) {
        block[i] ^= key[i % key_len];
        
        // Sustitución simple (no es S-box real de AES)
        block[i] = ((block[i] << 4) | (block[i] >> 4));
    }
    
    // ShiftRows simplificado
    unsigned char temp = block[1];
    block[1] = block[5];
    block[5] = block[9];
    block[9] = block[13];
    block[13] = temp;
}

ssize_t aes_encrypt(const unsigned char* input, size_t input_size,
                   unsigned char* output, const unsigned char* key, size_t key_len) {
    if (input == NULL || output == NULL || key == NULL || key_len == 0) {
        return -1;
    }

    // Procesar en bloques de 16 bytes
    size_t i;
    for (i = 0; i + 16 <= input_size; i += 16) {
        memcpy(output + i, input + i, 16);
        
        // 10 rondas simplificadas
        for (int round = 0; round < 10; round++) {
            simple_aes_round(output + i, key, key_len);
        }
    }

    // Procesar bytes restantes
    if (i < input_size) {
        unsigned char block[16] = {0};
        size_t remaining = input_size - i;
        memcpy(block, input + i, remaining);
        
        for (int round = 0; round < 10; round++) {
            simple_aes_round(block, key, key_len);
        }
        
        memcpy(output + i, block, remaining);
        i += remaining;
    }

    return i;
}

ssize_t aes_decrypt(const unsigned char* input, size_t input_size,
                   unsigned char* output, const unsigned char* key, size_t key_len) {
    if (input == NULL || output == NULL || key == NULL || key_len == 0) {
        return -1;
    }

    // Implementación temporal - XOR simétrico
    // TODO: Implementar desencriptación completa de AES
    size_t i;
    for (i = 0; i < input_size; i++) {
        output[i] = input[i] ^ key[i % key_len];
    }

    return i;
}
