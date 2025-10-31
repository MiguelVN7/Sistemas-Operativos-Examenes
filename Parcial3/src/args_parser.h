#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

/**
 * @file args_parser.h
 * @brief Parser de argumentos de línea de comandos para GSEA
 */

typedef enum {
    OP_NONE = 0,
    OP_COMPRESS = 1 << 0,      // -c
    OP_DECOMPRESS = 1 << 1,    // -d
    OP_ENCRYPT = 1 << 2,       // -e
    OP_DECRYPT = 1 << 3        // -u
} Operation;

typedef enum {
    COMP_NONE,
    COMP_RLE,
    COMP_HUFFMAN,
    COMP_LZW
} CompressionAlgorithm;

typedef enum {
    ENC_NONE,
    ENC_VIGENERE,
    ENC_DES,
    ENC_AES
} EncryptionAlgorithm;

/**
 * @brief Estructura que contiene todos los argumentos parseados
 */
typedef struct {
    Operation operations;                    // Operaciones a realizar (pueden combinarse)
    CompressionAlgorithm compression_alg;    // Algoritmo de compresión
    EncryptionAlgorithm encryption_alg;      // Algoritmo de encriptación
    char* input_path;                        // Ruta de entrada (-i)
    char* output_path;                       // Ruta de salida (-o)
    char* key;                               // Clave de encriptación (-k)
    int verbose;                             // Modo verbose
} Arguments;

/**
 * @brief Parsea los argumentos de línea de comandos
 */
Arguments parse_arguments(int argc, char* argv[]);

/**
 * @brief Valida que los argumentos sean correctos
 */
int validate_arguments(const Arguments* args);

/**
 * @brief Libera memoria de la estructura Arguments
 */
void free_arguments(Arguments* args);

/**
 * @brief Imprime la ayuda de uso del programa
 */
void print_usage(const char* program_name);

/**
 * @brief Convierte nombre de algoritmo de compresión a enum
 */
CompressionAlgorithm string_to_compression_alg(const char* str);

/**
 * @brief Convierte nombre de algoritmo de encriptación a enum
 */
EncryptionAlgorithm string_to_encryption_alg(const char* str);

#endif // ARGS_PARSER_H
