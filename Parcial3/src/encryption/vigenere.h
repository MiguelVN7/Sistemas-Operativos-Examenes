#ifndef VIGENERE_H
#define VIGENERE_H

#include <stddef.h>

/**
 * @file vigenere.h
 * @brief Implementación del cifrado Vigenère
 */

/**
 * @brief Encripta datos usando cifrado Vigenère
 *
 * @param input Datos de entrada (texto)
 * @param input_size Tamaño de entrada
 * @param key Clave de encriptación
 * @param key_size Tamaño de la clave
 * @param output_size Puntero donde se guardará el tamaño de salida
 * @return Puntero a datos encriptados (debe ser liberado por el llamador)
 */
unsigned char* vigenere_encrypt(const unsigned char* input, size_t input_size,
                                const unsigned char* key, size_t key_size,
                                size_t* output_size);

/**
 * @brief Desencripta datos usando cifrado Vigenère
 *
 * @param input Datos encriptados
 * @param input_size Tamaño de entrada encriptada
 * @param key Clave de desencriptación
 * @param key_size Tamaño de la clave
 * @param output_size Puntero donde se guardará el tamaño de salida
 * @return Puntero a datos desencriptados (debe ser liberado por el llamador)
 */
unsigned char* vigenere_decrypt(const unsigned char* input, size_t input_size,
                                const unsigned char* key, size_t key_size,
                                size_t* output_size);

#endif // VIGENERE_H
