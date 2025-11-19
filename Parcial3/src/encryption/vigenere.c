#include "vigenere.h"
#include <string.h>

/**
 * @brief Encripta datos usando Cifrado Vigenère
 * Algoritmo: C[i] = (M[i] + K[i % K_len]) % 256
 */
ssize_t vigenere_encrypt(const unsigned char* input, size_t input_size,
                        unsigned char* output, const unsigned char* key, size_t key_len) {
    if (input == NULL || output == NULL || key == NULL || key_len == 0) {
        return -1;
    }

    for (size_t i = 0; i < input_size; i++) {
        unsigned int val = (unsigned int)input[i] + (unsigned int)key[i % key_len];
        output[i] = (unsigned char)(val % 256);
    }

    return input_size;
}

/**
 * @brief Desencripta datos usando Cifrado Vigenère
 * Algoritmo: M[i] = (C[i] - K[i % K_len] + 256) % 256
 */
ssize_t vigenere_decrypt(const unsigned char* input, size_t input_size,
                        unsigned char* output, const unsigned char* key, size_t key_len) {
    if (input == NULL || output == NULL || key == NULL || key_len == 0) {
        return -1;
    }

    for (size_t i = 0; i < input_size; i++) {
        // Usamos int para manejar resta negativa antes del modulo
        int val = (int)input[i] - (int)key[i % key_len];
        
        // Corrección de módulo para números negativos
        if (val < 0) {
            val += 256;
        }
        
        output[i] = (unsigned char)(val % 256);
    }

    return input_size;
}
