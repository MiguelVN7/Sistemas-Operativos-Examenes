#ifndef AES_SIMPLE_H
#define AES_SIMPLE_H

#include <stddef.h>

/**
 * @file aes_simple.h
 * @brief Implementación simplificada del algoritmo AES
 */

/**
 * @brief Encripta datos usando AES-128
 *
 * @param input Datos de entrada
 * @param input_size Tamaño de entrada (debe ser múltiplo de 16)
 * @param key Clave de encriptación (16 bytes para AES-128)
 * @param output_size Puntero donde se guardará el tamaño de salida
 * @return Puntero a datos encriptados (debe ser liberado por el llamador)
 */
unsigned char* aes_encrypt(const unsigned char* input, size_t input_size,
                          const unsigned char* key, size_t* output_size);

/**
 * @brief Desencripta datos usando AES-128
 *
 * @param input Datos encriptados
 * @param input_size Tamaño de entrada encriptada
 * @param key Clave de desencriptación (16 bytes para AES-128)
 * @param output_size Puntero donde se guardará el tamaño de salida
 * @return Puntero a datos desencriptados (debe ser liberado por el llamador)
 */
unsigned char* aes_decrypt(const unsigned char* input, size_t input_size,
                          const unsigned char* key, size_t* output_size);

#endif // AES_SIMPLE_H
