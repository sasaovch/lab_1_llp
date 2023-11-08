#pragma once

#include "../managers/page_manager.h"
#include "../data/constants.h"
#include "./enums.h"

#include <stdbool.h>

typedef struct __attribute__((packed)) {
    TypeOfElement element_type;
    ValueType value_type;
    uint32_t first_page;
    uint32_t last_page;
    uint32_t next_id;
    char type[NAME_TYPE_LENGTH + 1];
} Entity;


void print_entity(const Entity *entity);

void erase_entity(Cursor *cursor, const uint64_t *pointer);

void create_page_for_entity(const Cursor *cursor);

Entity *get_entity(const Cursor *cursor, TypeOfElement element_type, const char *name, uint64_t *pointer);

Entity *create_entity(Cursor *cursor, const Entity *new_entity);

bool delete_entity(Cursor *cursor, const Entity *entity);
