#include "vigenere.h"
#include <string.h>

/**
 * Cifrado Vigenère - Algoritmo de cifrado por sustitución polialfabética
 * Usa una clave para desplazar cada byte del texto
 */

ssize_t vigenere_encrypt(const unsigned char* input, size_t input_size,
                        unsigned char* output, const unsigned char* key, size_t key_len) {
    if (input == NULL || output == NULL || key == NULL || key_len == 0) {
        return -1;
    }

    for (size_t i = 0; i < input_size; i++) {
        unsigned char key_byte = key[i % key_len];
        output[i] = input[i] ^ key_byte;  // XOR con la clave
    }

    return input_size;
}

ssize_t vigenere_decrypt(const unsigned char* input, size_t input_size,
                        unsigned char* output, const unsigned char* key, size_t key_len) {
    // Vigenère con XOR es simétrico
    return vigenere_encrypt(input, input_size, output, key, key_len);
}
