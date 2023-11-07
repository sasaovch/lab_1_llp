#pragma once

#include "../data/enums.h"
#include "../data/constants.h"
#include "../managers/page_manager.h"
#include "./entity.h"
#include <stdint.h>

typedef struct __attribute__((packed)) {
    ValueType value_type;
    uint32_t subject_id;
    uint32_t value_length;
    char type[NAME_TYPE_LENGTH + 1];
    char subject_type[NAME_TYPE_LENGTH + 1];
    void *value;
} Property;

void write_property_to_file(Cursor *cursor, Page *page, Entity *entity, const void *pr, const uint32_t *id);

void print_property(const Property *property);

void *read_property_from_file(const Cursor *cursor, Page *page, const uint64_t *offset_);

uint64_t get_size_of_property(const void *pr);

void memcpy_property(const void *element, char *stack, uint64_t *offset);

void *memget_property(const char *stack, uint64_t *offset);

uint32_t get_property_id(const void *pr);

bool compare_property_by_subject(const void *pr_1, const void *pr_2);

bool compare_subject_property(const void *pr_1, const void *pr_2);

bool compare_key_property(const void *pr_1, const void *pr_2);

void free_property(void *pr);

void read_value_from_file(
        const Cursor *cursor, Page *page,
        Property *property, uint32_t value_length,
        const char *body, uint64_t offset
);
