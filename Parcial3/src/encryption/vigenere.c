#include "vigenere.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * @file vigenere.c
 * @brief Implementación del cifrado Vigenère
 *
 * El cifrado Vigenère usa una clave repetida para desplazar caracteres
 * alfabéticos. Es un cifrado de sustitución polialfabética.
 */

unsigned char* vigenere_encrypt(const unsigned char* input, size_t input_size,
                                const unsigned char* key, size_t key_size,
                                size_t* output_size) {
    if (!input || !key || input_size == 0 || key_size == 0 || !output_size) {
        return NULL;
    }

    unsigned char* output = (unsigned char*)malloc(input_size);
    if (!output) {
        return NULL;
    }

    for (size_t i = 0; i < input_size; i++) {
        unsigned char c = input[i];

        if (isalpha(c)) {
            // Determinar si es mayúscula o minúscula
            char base = isupper(c) ? 'A' : 'a';

            // Obtener desplazamiento de la clave (solo caracteres alfabéticos)
            unsigned char key_char = toupper(key[i % key_size]);
            int shift = key_char - 'A';

            // Aplicar cifrado Vigenère
            output[i] = ((c - base + shift) % 26) + base;
        } else {
            // No encriptar caracteres no alfabéticos
            output[i] = c;
        }
    }

    *output_size = input_size;
    return output;
}

unsigned char* vigenere_decrypt(const unsigned char* input, size_t input_size,
                                const unsigned char* key, size_t key_size,
                                size_t* output_size) {
    if (!input || !key || input_size == 0 || key_size == 0 || !output_size) {
        return NULL;
    }

    unsigned char* output = (unsigned char*)malloc(input_size);
    if (!output) {
        return NULL;
    }

    for (size_t i = 0; i < input_size; i++) {
        unsigned char c = input[i];

        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';

            unsigned char key_char = toupper(key[i % key_size]);
            int shift = key_char - 'A';

            // Aplicar descifrado Vigenère (desplazamiento inverso)
            output[i] = ((c - base - shift + 26) % 26) + base;
        } else {
            output[i] = c;
        }
    }

    *output_size = input_size;
    return output;
}
