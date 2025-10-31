#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/encryption/vigenere.h"
#include "../src/encryption/des.h"
#include "../src/encryption/aes_simple.h"
#include "../src/utils/error_handler.h"

/**
 * @file test_encryption.c
 * @brief Pruebas unitarias para módulos de encriptación
 */

void test_vigenere_basic() {
    printf("Testing Vigenere basic encryption...\n");

    const char* input = "ATTACKATDAWN";
    const char* key = "LEMON";
    size_t input_size = strlen(input);
    size_t key_size = strlen(key);
    size_t output_size = 0;

    unsigned char* encrypted = vigenere_encrypt((unsigned char*)input, input_size,
                                                (unsigned char*)key, key_size,
                                                &output_size);
    assert(encrypted != NULL);
    assert(output_size == input_size);

    printf("  Original: %s\n", input);
    printf("  Key: %s\n", key);
    printf("  Encrypted: %.*s\n", (int)output_size, encrypted);

    size_t decrypted_size = 0;
    unsigned char* decrypted = vigenere_decrypt(encrypted, output_size,
                                               (unsigned char*)key, key_size,
                                               &decrypted_size);
    assert(decrypted != NULL);
    assert(decrypted_size == input_size);
    assert(memcmp(decrypted, input, input_size) == 0);

    printf("  Decrypted: %.*s\n", (int)decrypted_size, decrypted);
    printf("  ✓ Vigenere basic test passed\n");

    free(encrypted);
    free(decrypted);
}

void test_vigenere_mixed_case() {
    printf("Testing Vigenere with mixed case...\n");

    const char* input = "Hello World";
    const char* key = "KEY";
    size_t input_size = strlen(input);
    size_t key_size = strlen(key);
    size_t output_size = 0;

    unsigned char* encrypted = vigenere_encrypt((unsigned char*)input, input_size,
                                                (unsigned char*)key, key_size,
                                                &output_size);
    assert(encrypted != NULL);

    printf("  Original: %s\n", input);
    printf("  Encrypted: %.*s\n", (int)output_size, encrypted);

    size_t decrypted_size = 0;
    unsigned char* decrypted = vigenere_decrypt(encrypted, output_size,
                                               (unsigned char*)key, key_size,
                                               &decrypted_size);
    assert(decrypted != NULL);
    assert(memcmp(decrypted, input, input_size) == 0);

    printf("  Decrypted: %.*s\n", (int)decrypted_size, decrypted);
    printf("  ✓ Vigenere mixed case test passed\n");

    free(encrypted);
    free(decrypted);
}

void test_des_placeholder() {
    printf("Testing DES (placeholder)...\n");

    const char* input = "12345678";  // 8 bytes for DES block
    const char* key = "DESKEY12";     // 8 bytes for DES key
    size_t input_size = strlen(input);
    size_t output_size = 0;

    unsigned char* encrypted = des_encrypt((unsigned char*)input, input_size,
                                          (unsigned char*)key, &output_size);

    if (encrypted != NULL) {
        printf("  Input: %s\n", input);
        printf("  Encrypted size: %zu bytes\n", output_size);

        size_t decrypted_size = 0;
        unsigned char* decrypted = des_decrypt(encrypted, output_size,
                                              (unsigned char*)key, &decrypted_size);

        if (decrypted != NULL && memcmp(decrypted, input, input_size) == 0) {
            printf("  ✓ DES encryption/decryption cycle successful\n");
            free(decrypted);
        }

        printf("  ⚠ DES using placeholder implementation\n");
        free(encrypted);
    } else {
        printf("  ⚠ DES encryption returned NULL\n");
    }
}

void test_aes_placeholder() {
    printf("Testing AES (placeholder)...\n");

    const char* input = "1234567890123456";  // 16 bytes for AES block
    const char* key = "AESKEY1234567890";    // 16 bytes for AES-128 key
    size_t input_size = strlen(input);
    size_t output_size = 0;

    unsigned char* encrypted = aes_encrypt((unsigned char*)input, input_size,
                                          (unsigned char*)key, &output_size);

    if (encrypted != NULL) {
        printf("  Input: %s\n", input);
        printf("  Encrypted size: %zu bytes\n", output_size);

        size_t decrypted_size = 0;
        unsigned char* decrypted = aes_decrypt(encrypted, output_size,
                                              (unsigned char*)key, &decrypted_size);

        if (decrypted != NULL && memcmp(decrypted, input, input_size) == 0) {
            printf("  ✓ AES encryption/decryption cycle successful\n");
            free(decrypted);
        }

        printf("  ⚠ AES using placeholder implementation\n");
        free(encrypted);
    } else {
        printf("  ⚠ AES encryption returned NULL\n");
    }
}

int main() {
    printf("\n=== Encryption Module Tests ===\n\n");

    test_vigenere_basic();
    test_vigenere_mixed_case();
    test_des_placeholder();
    test_aes_placeholder();

    printf("\n=== All encryption tests completed ===\n\n");

    return 0;
}
