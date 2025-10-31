#ifndef DES_H
#define DES_H

#include <stddef.h>

/**
 * @file des.h
 * @brief Implementación simplificada del algoritmo DES
 */

/**
 * @brief Encripta datos usando DES
 *
 * @param input Datos de entrada
 * @param input_size Tamaño de entrada (debe ser múltiplo de 8)
 * @param key Clave de encriptación (8 bytes)
 * @param output_size Puntero donde se guardará el tamaño de salida
 * @return Puntero a datos encriptados (debe ser liberado por el llamador)
 */
unsigned char* des_encrypt(const unsigned char* input, size_t input_size,
                          const unsigned char* key, size_t* output_size);

/**
 * @brief Desencripta datos usando DES
 *
 * @param input Datos encriptados
 * @param input_size Tamaño de entrada encriptada
 * @param key Clave de desencriptación (8 bytes)
 * @param output_size Puntero donde se guardará el tamaño de salida
 * @return Puntero a datos desencriptados (debe ser liberado por el llamador)
 */
unsigned char* des_decrypt(const unsigned char* input, size_t input_size,
                          const unsigned char* key, size_t* output_size);

#endif // DES_H
