#ifndef RLE_H
#define RLE_H

#include <stddef.h>

/**
 * @file rle.h
 * @brief Implementación del algoritmo Run-Length Encoding (RLE)
 */

/**
 * @brief Comprime datos usando RLE
 *
 * @param input Datos de entrada
 * @param input_size Tamaño de entrada
 * @param output_size Puntero donde se guardará el tamaño de salida
 * @return Puntero a datos comprimidos (debe ser liberado por el llamador)
 */
unsigned char* rle_compress(const unsigned char* input, size_t input_size, size_t* output_size);

/**
 * @brief Descomprime datos codificados con RLE
 *
 * @param input Datos comprimidos
 * @param input_size Tamaño de entrada comprimida
 * @param output_size Puntero donde se guardará el tamaño de salida
 * @return Puntero a datos descomprimidos (debe ser liberado por el llamador)
 */
unsigned char* rle_decompress(const unsigned char* input, size_t input_size, size_t* output_size);

#endif // RLE_H
