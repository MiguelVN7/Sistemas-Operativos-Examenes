#ifndef LZW_H
#define LZW_H

#include <stddef.h>

/**
 * @file lzw.h
 * @brief Implementación del algoritmo Lempel-Ziv-Welch (LZW)
 */

/**
 * @brief Comprime datos usando LZW
 *
 * @param input Datos de entrada
 * @param input_size Tamaño de entrada
 * @param output_size Puntero donde se guardará el tamaño de salida
 * @return Puntero a datos comprimidos (debe ser liberado por el llamador)
 */
unsigned char* lzw_compress(const unsigned char* input, size_t input_size, size_t* output_size);

/**
 * @brief Descomprime datos codificados con LZW
 *
 * @param input Datos comprimidos
 * @param input_size Tamaño de entrada comprimida
 * @param output_size Puntero donde se guardará el tamaño de salida
 * @return Puntero a datos descomprimidos (debe ser liberado por el llamador)
 */
unsigned char* lzw_decompress(const unsigned char* input, size_t input_size, size_t* output_size);

#endif // LZW_H
