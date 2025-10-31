#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <stddef.h>

/**
 * @file encryption.h
 * @brief Interfaz com�n para algoritmos de encriptaci�n
 */

/**
 * @brief Tipo de algoritmo de encriptaci�n
 */
typedef enum {
    ENCRYPTION_VIGENERE,
    ENCRYPTION_DES,
    ENCRYPTION_AES
} EncryptionType;

/**
 * @brief Resultado de operaci�n de encriptaci�n
 */
typedef struct {
    unsigned char* data;      // Datos encriptados/desencriptados
    size_t size;             // Tama�o de datos
    int success;             // 1 si exitoso, 0 si fall�
    char error_msg[256];     // Mensaje de error si fall�
} EncryptionResult;

/**
 * @brief Encripta datos usando el algoritmo especificado
 *
 * @param input Datos de entrada
 * @param input_size Tama�o de datos de entrada
 * @param key Clave de encriptaci�n
 * @param key_size Tama�o de la clave
 * @param type Tipo de encriptaci�n a usar
 * @return EncryptionResult con datos encriptados o error
 */
EncryptionResult encrypt_data(const unsigned char* input, size_t input_size,
                              const unsigned char* key, size_t key_size,
                              EncryptionType type);

/**
 * @brief Desencripta datos usando el algoritmo especificado
 *
 * @param input Datos encriptados
 * @param input_size Tama�o de datos encriptados
 * @param key Clave de desencriptaci�n
 * @param key_size Tama�o de la clave
 * @param type Tipo de encriptaci�n usado
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
