#include "error_handler.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

/**
 * @file error_handler.c
 * @brief Implementación del sistema de manejo de errores
 */

static ErrorCode last_error = ERROR_NONE;

const char* error_to_string(ErrorCode code) {
    switch (code) {
        case ERROR_NONE:
            return "No error";
        case ERROR_INVALID_ARGUMENT:
            return "Invalid argument";
        case ERROR_MEMORY_ALLOCATION:
            return "Memory allocation failed";
        case ERROR_FILE_NOT_FOUND:
            return "File not found";
        case ERROR_FILE_READ:
            return "File read error";
        case ERROR_FILE_WRITE:
            return "File write error";
        case ERROR_COMPRESSION_FAILED:
            return "Compression failed";
        case ERROR_DECOMPRESSION_FAILED:
            return "Decompression failed";
        case ERROR_ENCRYPTION_FAILED:
            return "Encryption failed";
        case ERROR_DECRYPTION_FAILED:
            return "Decryption failed";
        case ERROR_THREAD_CREATION:
            return "Thread creation failed";
        case ERROR_MUTEX_INIT:
            return "Mutex initialization failed";
        case ERROR_DIRECTORY_ACCESS:
            return "Directory access failed";
        case ERROR_UNKNOWN:
        default:
            return "Unknown error";
    }
}

static const char* severity_to_string(ErrorSeverity severity) {
    switch (severity) {
        case SEVERITY_INFO:
            return "INFO";
        case SEVERITY_WARNING:
            return "WARNING";
        case SEVERITY_ERROR:
            return "ERROR";
        case SEVERITY_CRITICAL:
            return "CRITICAL";
        default:
            return "UNKNOWN";
    }
}

void log_error(ErrorCode code, ErrorSeverity severity,
               const char* file, int line, const char* message) {
    last_error = code;

    // Obtener timestamp
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

    // Imprimir error en stderr
    fprintf(stderr, "[%s] [%s] %s:%d - %s: %s\n",
            timestamp,
            severity_to_string(severity),
            file, line,
            error_to_string(code),
            message);

    // TODO: Opcionalmente escribir a archivo de log
}

ErrorCode get_last_error(void) {
    return last_error;
}

void clear_error(void) {
    last_error = ERROR_NONE;
}
