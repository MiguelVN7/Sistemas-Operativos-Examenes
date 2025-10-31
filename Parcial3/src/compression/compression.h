#ifndef COMPRESSION_H
#define COMPRESSION_H

#include "../file_manager.h"
#include "../args_parser.h"
#include <stddef.h>
#include <sys/types.h>

/**
 * @file compression.h
 * @brief Interfaz unificada para algoritmos de compresión
 */

/**
 * @brief Comprime datos usando el algoritmo especificado
 */
ssize_t compress_data(const unsigned char* input, size_t input_size,
                     FileBuffer* output, CompressionAlgorithm algorithm);

/**
 * @brief Descomprime datos usando el algoritmo especificado
 */
ssize_t decompress_data(const unsigned char* input, size_t input_size,
                       FileBuffer* output, CompressionAlgorithm algorithm);

// RLE (Run-Length Encoding)
ssize_t rle_compress(const unsigned char* input, size_t input_size,
                    unsigned char* output, size_t output_capacity);
ssize_t rle_decompress(const unsigned char* input, size_t input_size,
                      unsigned char* output, size_t output_capacity);

// Huffman Coding
ssize_t huffman_compress(const unsigned char* input, size_t input_size,
                        unsigned char* output, size_t output_capacity);
ssize_t huffman_decompress(const unsigned char* input, size_t input_size,
                          unsigned char* output, size_t output_capacity);

// LZW (Lempel-Ziv-Welch)
ssize_t lzw_compress(const unsigned char* input, size_t input_size,
                    unsigned char* output, size_t output_capacity);
ssize_t lzw_decompress(const unsigned char* input, size_t input_size,
                      unsigned char* output, size_t output_capacity);

#endif // COMPRESSION_H
