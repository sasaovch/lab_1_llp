#include "../../include/utils/io_utils.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void println(const char *line, ...) {
    va_list args;
    va_start(args, line);
    vprintf(line, args);
    printf("\n");
    va_end(args);
}

void error_exit(int var, const char *line, ...) {
    if (var == -1) {
        va_list args;
        va_start(args, line);
        vprintf(line, args);
        printf("\n");
        va_end(args);
        exit(EXIT_FAILURE);
    }
}

void debug(int num, const char *line, ...) {
    println("Debug %i", num);
    va_list args;
    va_start(args, line);
    vprintf(line, args);
    printf("\n");
    va_end(args);
}

void print_test_format(const char *line, ...) {
    println("---------------------------");
    va_list args;
    va_start(args, line);
    vprintf(line, args);
    printf("\n");
    va_end(args);
    println("---------------------------");
}
