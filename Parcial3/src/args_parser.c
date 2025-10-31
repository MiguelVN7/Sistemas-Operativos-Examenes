#define _POSIX_C_SOURCE 200809L
#include "args_parser.h"
#include "utils/error_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

void print_usage(const char* program_name) {
    printf("Uso: %s [OPCIONES]\n\n", program_name);
    printf("GSEA - Gestión Segura y Eficiente de Archivos\n\n");
    printf("Operaciones:\n");
    printf("  -c              Comprimir\n");
    printf("  -d              Descomprimir\n");
    printf("  -e              Encriptar\n");
    printf("  -u              Desencriptar\n");
    printf("  (Se pueden combinar: -ce para comprimir y encriptar)\n\n");
    printf("Algoritmos:\n");
    printf("  --comp-alg ALG  Algoritmo de compresión (rle, huffman, lzw)\n");
    printf("  --enc-alg ALG   Algoritmo de encriptación (vigenere, des, aes)\n\n");
    printf("Archivos:\n");
    printf("  -i PATH         Archivo o directorio de entrada\n");
    printf("  -o PATH         Archivo o directorio de salida\n");
    printf("  -k KEY          Clave de encriptación (requerida para -e/-u)\n\n");
    printf("Opciones:\n");
    printf("  -v              Modo verbose\n");
    printf("  -h, --help      Mostrar esta ayuda\n\n");
    printf("Ejemplos:\n");
    printf("  %s -c --comp-alg lzw -i input.txt -o output.lzw\n", program_name);
    printf("  %s -ce --comp-alg huffman --enc-alg des -i dir/ -o output.enc -k mykey\n", program_name);
}

CompressionAlgorithm string_to_compression_alg(const char* str) {
    if (strcmp(str, "rle") == 0) return COMP_RLE;
    if (strcmp(str, "huffman") == 0) return COMP_HUFFMAN;
    if (strcmp(str, "lzw") == 0) return COMP_LZW;
    return COMP_NONE;
}

EncryptionAlgorithm string_to_encryption_alg(const char* str) {
    if (strcmp(str, "vigenere") == 0) return ENC_VIGENERE;
    if (strcmp(str, "des") == 0) return ENC_DES;
    if (strcmp(str, "aes") == 0) return ENC_AES;
    return ENC_NONE;
}

Arguments parse_arguments(int argc, char* argv[]) {
    Arguments args = {
        .operations = OP_NONE,
        .compression_alg = COMP_NONE,
        .encryption_alg = ENC_NONE,
        .input_path = NULL,
        .output_path = NULL,
        .key = NULL,
        .verbose = 0
    };

    static struct option long_options[] = {
        {"comp-alg", required_argument, 0, 'C'},
        {"enc-alg",  required_argument, 0, 'E'},
        {"help",     no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "cdeui:o:k:vhC:E:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'c': args.operations |= OP_COMPRESS; break;
            case 'd': args.operations |= OP_DECOMPRESS; break;
            case 'e': args.operations |= OP_ENCRYPT; break;
            case 'u': args.operations |= OP_DECRYPT; break;
            case 'i': args.input_path = strdup(optarg); break;
            case 'o': args.output_path = strdup(optarg); break;
            case 'k': args.key = strdup(optarg); break;
            case 'v': args.verbose = 1; break;
            case 'C':
                args.compression_alg = string_to_compression_alg(optarg);
                if (args.compression_alg == COMP_NONE) {
                    fprintf(stderr, "Algoritmo de compresión inválido: %s\n", optarg);
                }
                break;
            case 'E':
                args.encryption_alg = string_to_encryption_alg(optarg);
                if (args.encryption_alg == ENC_NONE) {
                    fprintf(stderr, "Algoritmo de encriptación inválido: %s\n", optarg);
                }
                break;
            case 'h':
            case '?':
            default:
                print_usage(argv[0]);
                exit(opt == 'h' ? 0 : 1);
        }
    }

    return args;
}

int validate_arguments(const Arguments* args) {
    if (args->operations == OP_NONE) {
        fprintf(stderr, "Error: Debe especificar al menos una operación (-c, -d, -e, -u)\n");
        return -1;
    }

    if ((args->operations & OP_COMPRESS) && (args->operations & OP_DECOMPRESS)) {
        fprintf(stderr, "Error: No se puede comprimir y descomprimir al mismo tiempo\n");
        return -1;
    }

    if ((args->operations & OP_ENCRYPT) && (args->operations & OP_DECRYPT)) {
        fprintf(stderr, "Error: No se puede encriptar y desencriptar al mismo tiempo\n");
        return -1;
    }

    if (args->input_path == NULL) {
        fprintf(stderr, "Error: Debe especificar ruta de entrada (-i)\n");
        return -1;
    }

    if (args->output_path == NULL) {
        fprintf(stderr, "Error: Debe especificar ruta de salida (-o)\n");
        return -1;
    }

    if ((args->operations & (OP_COMPRESS | OP_DECOMPRESS)) && args->compression_alg == COMP_NONE) {
        fprintf(stderr, "Error: Debe especificar algoritmo de compresión (--comp-alg)\n");
        return -1;
    }

    if ((args->operations & (OP_ENCRYPT | OP_DECRYPT)) && args->encryption_alg == ENC_NONE) {
        fprintf(stderr, "Error: Debe especificar algoritmo de encriptación (--enc-alg)\n");
        return -1;
    }

    if ((args->operations & (OP_ENCRYPT | OP_DECRYPT)) && args->key == NULL) {
        fprintf(stderr, "Error: Debe especificar clave de encriptación (-k)\n");
        return -1;
    }

    return 0;
}

void free_arguments(Arguments* args) {
    if (args->input_path) {
        free(args->input_path);
        args->input_path = NULL;
    }
    if (args->output_path) {
        free(args->output_path);
        args->output_path = NULL;
    }
    if (args->key) {
        memset(args->key, 0, strlen(args->key));
        free(args->key);
        args->key = NULL;
    }
}
