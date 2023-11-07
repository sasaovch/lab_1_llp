#pragma once

typedef enum {
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3,
} LogLevel;

enum ErrorCodes {
    NULL_POINTER_IN_ARGS = 0,
    NULL_POINTER_RETURNED = 1,
};

void LOG(LogLevel level, const char *file, int line, const char* format, ...);

#define LOG_DEBUG(format, ...) LOG(DEBUG, __FILE__, __LINE__, format, __VA_ARGS__)

#define LOG_INFO(format, ...) LOG(INFO, __FILE__, __LINE__, format, __VA_ARGS__)

#define LOG_WARN(format, ...) LOG(WARN, __FILE__, __LINE__, format, __VA_ARGS__)

#define LOG_ERROR(format, ...) LOG(ERROR, __FILE__, __LINE__, format, __VA_ARGS__)
