#pragma once

#include "./file.h"
#include "./stack.h"

#include <stdint.h>

typedef struct {
    File *file;
    uint32_t last_stack_page;
    uint32_t number_of_pages;
    Stack *empty_pages;
} Cursor;
