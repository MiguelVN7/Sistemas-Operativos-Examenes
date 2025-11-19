#include "error_handler.h"
#include <stdio.h>
#include <stdlib.h>

void log_error(ErrorCode code, ErrorSeverity severity, const char* message, const char* file, int line) {
    const char* severity_str;
    switch (severity) {
        case SEVERITY_INFO: severity_str = "INFO"; break;
        case SEVERITY_WARNING: severity_str = "WARNING"; break;
        case SEVERITY_ERROR: severity_str = "ERROR"; break;
        case SEVERITY_FATAL: severity_str = "FATAL"; break;
        default: severity_str = "UNKNOWN"; break;
    }

    fprintf(stderr, "[%s] Error %d: %s (at %s:%d)\n", severity_str, code, message, file, line);
    
    if (severity == SEVERITY_FATAL) {
        exit(EXIT_FAILURE);
    }
}

