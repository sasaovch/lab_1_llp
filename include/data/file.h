#pragma once

#ifdef _WIN32
#include <io.h>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif

#include <stdint.h>
#include <stdio.h>

typedef struct {
    FILE *file;
    uint64_t file_length;
} File;
