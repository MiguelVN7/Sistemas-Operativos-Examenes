#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <stddef.h>

/**
 * @file encryption.h
 * @brief Interfaz común para algoritmos de encriptación
 */

/**
 * @brief Tipo de algoritmo de encriptación
 */
typedef enum {
    ENCRYPTION_VIGENERE,
    ENCRYPTION_DES,
    ENCRYPTION_AES
} EncryptionType;

/**
 * @brief Resultado de operación de encriptación
 */
typedef struct {
    unsigned char* data;      // Datos encriptados/desencriptados
    size_t size;             // Tamaño de datos
    int success;             // 1 si exitoso, 0 si falló
    char error_msg[256];     // Mensaje de error si falló
} EncryptionResult;

/**
 * @brief Encripta datos usando el algoritmo especificado
 *
 * @param input Datos de entrada
 * @param input_size Tamaño de datos de entrada
 * @param key Clave de encriptación
 * @param key_size Tamaño de la clave
 * @param type Tipo de encriptación a usar
 * @return EncryptionResult con datos encriptados o error
 */
EncryptionResult encrypt_data(const unsigned char* input, size_t input_size,
                              const unsigned char* key, size_t key_size,
                              EncryptionType type);

/**
 * @brief Desencripta datos usando el algoritmo especificado
 *
 * @param input Datos encriptados
 * @param input_size Tamaño de datos encriptados
 * @param key Clave de desencriptación
 * @param key_size Tamaño de la clave
 * @param type Tipo de encriptación usado
 * @return EncryptionResult con datos desencriptados o error
 */
EncryptionResult decrypt_data(const unsigned char* input, size_t input_size,
                              const unsigned char* key, size_t key_size,
                              EncryptionType type);

/**
 * @brief Libera memoria de un EncryptionResult
 *
 * @param result Resultado a liberar
 */
void free_encryption_result(EncryptionResult* result);

#endif // ENCRYPTION_H
