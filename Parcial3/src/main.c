#include "args_parser.h"
#include "file_manager.h"
#include "concurrency.h"
#include "utils/error_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

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
        printf("======================================================\n\n");
    }

    // Verificar si entrada existe
    if (!file_exists(args.input_path)) {
        LOG_ERROR(ERROR_FILE_OPEN, SEVERITY_FATAL, "Input path does not exist");
        free_arguments(&args);
        return 1;
    }

    int input_is_dir = is_directory(args.input_path);
    if (input_is_dir < 0) {
        LOG_ERROR(ERROR_FILE_READ, SEVERITY_FATAL, "Failed to stat input");
        free_arguments(&args);
        return 1;
    }

    int ret = 0;

    if (input_is_dir) {
        // Procesamiento de Directorio
        
        // Crear directorio de salida si no existe
        create_directory(args.output_path);

        if (args.verbose) printf("Leyendo directorio: %s\n", args.input_path);

        FileList* list = create_file_list();
        if (!list) {
            LOG_ERROR(ERROR_MEMORY_ALLOCATION, SEVERITY_FATAL, "Failed to create file list");
            free_arguments(&args);
            return 1;
        }

        if (read_directory(args.input_path, list) != 0) {
            LOG_ERROR(ERROR_DIRECTORY_ACCESS, SEVERITY_FATAL, "Failed to read directory");
            free_file_list(list);
            free_arguments(&args);
            return 1;
        }

        if (args.verbose) printf("Archivos encontrados: %zu. Iniciando procesamiento concurrente...\n", list->count);

        if (process_files_concurrent(list, &args) != 0) {
            LOG_ERROR(ERROR_UNKNOWN, SEVERITY_ERROR, "Some files failed to process");
            ret = 1;
        }

        free_file_list(list);

    } else {
        // Procesamiento de Archivo Único
        if (args.verbose) printf("Procesando archivo único...\n");
        
        // Si output es un directorio existente, agregar nombre de archivo
        if (is_directory(args.output_path) == 1) {
            char actual_output[MAX_PATH_LENGTH];
            if (build_output_path(args.input_path, args.output_path, actual_output, MAX_PATH_LENGTH) == 0) {
                if (process_single_file(args.input_path, actual_output, &args) != 0) {
                    ret = 1;
                }
            } else {
                LOG_ERROR(ERROR_INVALID_ARGUMENT, SEVERITY_ERROR, "Output path too long");
                ret = 1;
            }
        } else {
            if (process_single_file(args.input_path, args.output_path, &args) != 0) {
                ret = 1;
            }
        }
    }
    
    if (args.verbose) {
        printf("\nProceso completado.\n");
    }

    // Liberar memoria de argumentos
    free_arguments(&args);

    return ret;
}
