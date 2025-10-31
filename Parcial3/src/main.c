#include "args_parser.h"
#include "file_manager.h"
#include "concurrency.h"
#include "utils/error_handler.h"
#include "utils/memory_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @file main.c
 * @brief Punto de entrada principal de GSEA
 */

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    // Parsear argumentos
    Arguments args = parse_arguments(argc, argv);

    // Validar argumentos
    if (validate_arguments(&args) != 0) {
        free_arguments(&args);
        return 1;
    }

    if (args.verbose) {
        printf("=== GSEA - Gestión Segura y Eficiente de Archivos ===\n");
        printf("Entrada: %s\n", args.input_path);
        printf("Salida: %s\n", args.output_path);
        
        if (args.operations & OP_COMPRESS) {
            printf("Operación: Compresión (%s)\n",
                   args.compression_alg == COMP_RLE ? "RLE" :
                   args.compression_alg == COMP_HUFFMAN ? "Huffman" :
                   args.compression_alg == COMP_LZW ? "LZW" : "Desconocido");
        }
        if (args.operations & OP_DECOMPRESS) {
            printf("Operación: Descompresión (%s)\n",
                   args.compression_alg == COMP_RLE ? "RLE" :
                   args.compression_alg == COMP_HUFFMAN ? "Huffman" :
                   args.compression_alg == COMP_LZW ? "LZW" : "Desconocido");
        }
        if (args.operations & OP_ENCRYPT) {
            printf("Operación: Encriptación (%s)\n",
                   args.encryption_alg == ENC_VIGENERE ? "Vigenere" :
                   args.encryption_alg == ENC_DES ? "DES" :
                   args.encryption_alg == ENC_AES ? "AES" : "Desconocido");
        }
        if (args.operations & OP_DECRYPT) {
            printf("Operación: Desencriptación (%s)\n",
                   args.encryption_alg == ENC_VIGENERE ? "Vigenere" :
                   args.encryption_alg == ENC_DES ? "DES" :
                   args.encryption_alg == ENC_AES ? "AES" : "Desconocido");
        }
        printf("======================================================\n\n");
    }

    int result = 0;

    // Verificar si la entrada es un directorio o un archivo
    int is_dir = is_directory(args.input_path);

    if (is_dir == 1) {
        // Procesar directorio con concurrencia
        if (args.verbose) {
            printf("Procesando directorio: %s\n", args.input_path);
        }

        // Crear directorio de salida si no existe
        create_directory(args.output_path);

        // Leer archivos del directorio
        FileList* file_list = create_file_list();
        if (file_list == NULL) {
            LOG_ERROR(ERROR_MEMORY_ALLOCATION, SEVERITY_CRITICAL,
                     "Failed to create file list");
            free_arguments(&args);
            return 1;
        }

        if (read_directory(args.input_path, file_list) < 0) {
            LOG_ERROR(ERROR_DIRECTORY_ACCESS, SEVERITY_ERROR,
                     "Failed to read directory");
            free_file_list(file_list);
            free_arguments(&args);
            return 1;
        }

        if (args.verbose) {
            printf("Archivos encontrados: %zu\n", file_list->count);
        }

        // Procesar archivos concurrentemente
        result = process_files_concurrent(file_list, &args);

        free_file_list(file_list);

    } else if (is_dir == 0) {
        // Procesar archivo único
        if (args.verbose) {
            printf("Procesando archivo: %s\n", args.input_path);
        }

        result = process_single_file(args.input_path, args.output_path, &args);

    } else {
        LOG_ERROR(ERROR_FILE_NOT_FOUND, SEVERITY_ERROR,
                 "Input path not found or inaccessible");
        free_arguments(&args);
        return 1;
    }

    // Liberar memoria de argumentos
    free_arguments(&args);

    if (result == 0) {
        if (args.verbose) {
            printf("\n✓ Procesamiento completado exitosamente\n");
        }
    } else {
        fprintf(stderr, "\n✗ Error durante el procesamiento\n");
    }

#ifdef DEBUG_MEMORY
    print_memory_leaks();
#endif

    return result;
}
