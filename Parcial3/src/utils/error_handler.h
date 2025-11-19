#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

typedef enum {
    SEVERITY_INFO,
    SEVERITY_WARNING,
    SEVERITY_ERROR,
    SEVERITY_FATAL
} ErrorSeverity;

typedef enum {
    ERROR_NONE,
    ERROR_INVALID_ARGUMENT,
    ERROR_FILE_OPEN,
    ERROR_FILE_READ,
    ERROR_FILE_WRITE,
    ERROR_DIRECTORY_ACCESS,
    ERROR_MEMORY_ALLOCATION,
    ERROR_THREAD_CREATION,
    ERROR_COMPRESSION_FAILED,
    ERROR_DECOMPRESSION_FAILED,
    ERROR_ENCRYPTION_FAILED,
    ERROR_DECRYPTION_FAILED,
    ERROR_UNKNOWN
} ErrorCode;

void log_error(ErrorCode code, ErrorSeverity severity, const char* message, const char* file, int line);

#define LOG_ERROR(code, severity, msg) log_error(code, severity, msg, __FILE__, __LINE__)

#endif // ERROR_HANDLER_H

