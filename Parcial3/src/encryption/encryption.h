#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include "../file_manager.h"
#include "../args_parser.h"
#include <stddef.h>
#include <sys/types.h>

/**
 * @file encryption.h
 * @brief Interfaz unificada para algoritmos de encriptación
 */

/**
 * @brief Encripta datos usando el algoritmo especificado
 */
ssize_t encrypt_data(const unsigned char* input, size_t input_size,
                    FileBuffer* output, EncryptionAlgorithm algorithm,
                    const unsigned char* key, size_t key_len);

/**
 * @brief Desencripta datos usando el algoritmo especificado
 */
ssize_t decrypt_data(const unsigned char* input, size_t input_size,
                    FileBuffer* output, EncryptionAlgorithm algorithm,
                    const unsigned char* key, size_t key_len);

// Vigenere Cipher
ssize_t vigenere_encrypt(const unsigned char* input, size_t input_size,
                        unsigned char* output, const unsigned char* key, size_t key_len);
ssize_t vigenere_decrypt(const unsigned char* input, size_t input_size,
                        unsigned char* output, const unsigned char* key, size_t key_len);

// DES (Data Encryption Standard - simplified)
ssize_t des_encrypt(const unsigned char* input, size_t input_size,
                   unsigned char* output, const unsigned char* key, size_t key_len);
ssize_t des_decrypt(const unsigned char* input, size_t input_size,
                   unsigned char* output, const unsigned char* key, size_t key_len);

// AES (Advanced Encryption Standard - simplified)
ssize_t aes_encrypt(const unsigned char* input, size_t input_size,
                   unsigned char* output, const unsigned char* key, size_t key_len);
ssize_t aes_decrypt(const unsigned char* input, size_t input_size,
                   unsigned char* output, const unsigned char* key, size_t key_len);

#endif // ENCRYPTION_H
