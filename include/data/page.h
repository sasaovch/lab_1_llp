#pragma once

#include "./file.h"
#include "./stack.h"

#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint32_t block_number;
    uint32_t next_block;
    uint32_t offset;
} PageHeader;

typedef struct {
    PageHeader *page_header;
    char *page_body;
} Page;

typedef struct {
    File *file;
    uint32_t last_entity_block;
    uint32_t number_of_pages;
    Stack *empty_pages;
} Cursor;

Page *new_page(void);

void print_page(const PageHeader *page_header);

void free_page(Page *page);
