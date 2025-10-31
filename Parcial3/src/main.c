#include "args_parser.h"
#include "file_manager.h"
#include "concurrency.h"
#include "utils/error_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @file main.c
 * @brief Punto de entrada principal de GSEA
 * 
 * TODO: Implementar la lógica completa de procesamiento
 * TODO: Integrar con módulos de compresión y encriptación
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

    // TODO: Implementar lógica de procesamiento
    printf("Funcionalidad en desarrollo...\n");
    
    // Liberar memoria de argumentos
    free_arguments(&args);

    return 0;
}
