#include "aes_simple.h"
#include <string.h>

/**
 * AES (Advanced Encryption Standard)
 * Cifrado por bloques de 128 bits
 * 
 * TODO: Implementar las rondas de AES
 * TODO: Implementar S-box (SubBytes)
 * TODO: Implementar ShiftRows
 * TODO: Implementar MixColumns
 * TODO: Implementar AddRoundKey
 * TODO: Implementar Key Expansion
 * 
 * Referencia: FIPS 197
 */

ssize_t aes_encrypt(const unsigned char* input, size_t input_size,
                   unsigned char* output, const unsigned char* key, size_t key_len) {
    if (input == NULL || output == NULL || key == NULL || key_len == 0) {
        return -1;
    }

    // TODO: Implementar AES encryption
    // - Procesar en bloques de 16 bytes
    // - Aplicar 10/12/14 rondas según tamaño de clave
    // - Implementar SubBytes, ShiftRows, MixColumns, AddRoundKey
    
    return -1;
}

ssize_t aes_decrypt(const unsigned char* input, size_t input_size,
                   unsigned char* output, const unsigned char* key, size_t key_len) {
    if (input == NULL || output == NULL || key == NULL || key_len == 0) {
        return -1;
    }

    // TODO: Implementar AES decryption
    // - Usar transformaciones inversas
    // - InvSubBytes, InvShiftRows, InvMixColumns
    
    return -1;
}
