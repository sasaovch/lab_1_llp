#include "../../include/utils/logger.h"

#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

extern uint32_t log_level;

const char *log_level_names[] = {
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
};

void LOG(LogLevel level, const char *file, int line, const char* format, ...) {
    va_list args;
    va_start (args, format);

    if (log_level > level) {
        return;
    }
    FILE *out = stdout;

    if (level >= WARN) {
        out = stderr;
    }

    fprintf(out, "[%s] [%s:%d]: ", log_level_names[level], file, line);
    vfprintf(out, format, args);
    fputc('\n', out);
    fflush(out);
    va_end (args);
}
