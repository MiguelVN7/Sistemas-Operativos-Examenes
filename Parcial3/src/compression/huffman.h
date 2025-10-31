#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stddef.h>

/**
 * @file huffman.h
 * @brief Implementación del algoritmo de compresión Huffman
 */

/**
 * @brief Comprime datos usando codificación Huffman
 *
 * @param input Datos de entrada
 * @param input_size Tamaño de entrada
 * @param output_size Puntero donde se guardará el tamaño de salida
 * @return Puntero a datos comprimidos (debe ser liberado por el llamador)
 */
unsigned char* huffman_compress(const unsigned char* input, size_t input_size, size_t* output_size);

/**
 * @brief Descomprime datos codificados con Huffman
 *
 * @param input Datos comprimidos
 * @param input_size Tamaño de entrada comprimida
 * @param output_size Puntero donde se guardará el tamaño de salida
 * @return Puntero a datos descomprimidos (debe ser liberado por el llamador)
 */
unsigned char* huffman_decompress(const unsigned char* input, size_t input_size, size_t* output_size);

#endif // HUFFMAN_H
