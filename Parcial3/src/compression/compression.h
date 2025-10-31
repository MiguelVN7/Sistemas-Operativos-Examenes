#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <stddef.h>

/**
 * @file compression.h
 * @brief Interfaz común para algoritmos de compresión
 */

/**
 * @brief Tipo de algoritmo de compresión
 */
typedef enum {
    COMPRESSION_HUFFMAN,
    COMPRESSION_RLE,
    COMPRESSION_LZW
} CompressionType;

/**
 * @brief Resultado de operación de compresión
 */
typedef struct {
    unsigned char* data;      // Datos comprimidos
    size_t size;             // Tamaño de datos comprimidos
    int success;             // 1 si exitoso, 0 si falló
    char error_msg[256];     // Mensaje de error si falló
} CompressionResult;

/**
 * @brief Comprime datos usando el algoritmo especificado
 *
 * @param input Datos de entrada
 * @param input_size Tamaño de datos de entrada
 * @param type Tipo de compresión a usar
 * @return CompressionResult con datos comprimidos o error
 */
CompressionResult compress_data(const unsigned char* input, size_t input_size, CompressionType type);

/**
 * @brief Descomprime datos usando el algoritmo especificado
 *
 * @param input Datos comprimidos
 * @param input_size Tamaño de datos comprimidos
 * @param type Tipo de compresión usado
 * @return CompressionResult con datos descomprimidos o error
 */
CompressionResult decompress_data(const unsigned char* input, size_t input_size, CompressionType type);

/**
 * @brief Libera memoria de un CompressionResult
 *
 * @param result Resultado a liberar
 */
void free_compression_result(CompressionResult* result);

#endif // COMPRESSION_H
