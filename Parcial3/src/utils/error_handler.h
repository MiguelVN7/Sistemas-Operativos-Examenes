#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

/**
 * @file error_handler.h
 * @brief Sistema centralizado de manejo de errores
 */

/**
 * @brief Códigos de error del sistema
 */
typedef enum {
    ERROR_NONE = 0,
    ERROR_INVALID_ARGUMENT,
    ERROR_MEMORY_ALLOCATION,
    ERROR_FILE_NOT_FOUND,
    ERROR_FILE_READ,
    ERROR_FILE_WRITE,
    ERROR_COMPRESSION_FAILED,
    ERROR_DECOMPRESSION_FAILED,
    ERROR_ENCRYPTION_FAILED,
    ERROR_DECRYPTION_FAILED,
    ERROR_THREAD_CREATION,
    ERROR_MUTEX_INIT,
    ERROR_UNKNOWN
} ErrorCode;

/**
 * @brief Severidad del error
 */
typedef enum {
    SEVERITY_INFO,
    SEVERITY_WARNING,
    SEVERITY_ERROR,
    SEVERITY_CRITICAL
} ErrorSeverity;

/**
 * @brief Registra un error en el sistema
 *
 * @param code Código de error
 * @param severity Severidad del error
 * @param file Archivo donde ocurrió el error
 * @param line Línea donde ocurrió el error
 * @param message Mensaje descriptivo del error
 */
void log_error(ErrorCode code, ErrorSeverity severity,
               const char* file, int line, const char* message);

/**
 * @brief Obtiene mensaje descriptivo para un código de error
 *
 * @param code Código de error
 * @return Mensaje descriptivo
 */
const char* error_to_string(ErrorCode code);

/**
 * @brief Obtiene el último error registrado
 *
 * @return Código del último error
 */
ErrorCode get_last_error(void);

/**
 * @brief Limpia el estado de error
 */
void clear_error(void);

/**
 * @brief Macro para facilitar el registro de errores
 */
#define LOG_ERROR(code, severity, msg) \
    log_error(code, severity, __FILE__, __LINE__, msg)

#endif // ERROR_HANDLER_H
