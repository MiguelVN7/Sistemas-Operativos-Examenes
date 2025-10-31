#include "aes_simple.h"
#include <stdlib.h>
#include <string.h>

/**
 * @file aes_simple.c
 * @brief Implementación simplificada del algoritmo AES
 *
 * NOTA: Esta es una implementación educativa simplificada.
 * Para producción, use librerías criptográficas establecidas como OpenSSL.
 *
 * AES-128 usa bloques de 128 bits (16 bytes) y clave de 128 bits.
 * Este es un placeholder que implementa XOR básico.
 */

unsigned char* aes_encrypt(const unsigned char* input, size_t input_size,
                          const unsigned char* key, size_t* output_size) {
    if (!input || !key || input_size == 0 || !output_size) {
        return NULL;
    }

    // TODO: Implementar AES completo con:
    // - SubBytes (S-box)
    // - ShiftRows
    // - MixColumns
    // - AddRoundKey
    // - Key expansion

    // Placeholder: Simple XOR con clave
    unsigned char* output = (unsigned char*)malloc(input_size);
    if (!output) {
        return NULL;
    }

    for (size_t i = 0; i < input_size; i++) {
        output[i] = input[i] ^ key[i % 16];
    }

    *output_size = input_size;
    return output;
}

unsigned char* aes_decrypt(const unsigned char* input, size_t input_size,
                          const unsigned char* key, size_t* output_size) {
    if (!input || !key || input_size == 0 || !output_size) {
        return NULL;
    }

    // Para XOR simple, decrypt es idéntico a encrypt
    return aes_encrypt(input, input_size, key, output_size);
}
