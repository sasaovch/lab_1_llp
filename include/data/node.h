#pragma once

#include "../managers/page_manager.h"
#include "../data/entity.h"
#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint32_t id;
    uint32_t name_length;
    char type[NAME_TYPE_LENGTH + 1];
    char *name;
} Node;

void print_node(const Node *node);

void write_node_to_file(Cursor *cursor, Page *page, Entity *entity, const void *nd, const uint32_t *id);

void *read_node_from_file(const Cursor *cursor, Page *page, const uint64_t *offset_);

uint64_t get_size_of_node(const void *nd);

bool compare_nodes(const void *nd_1, const void *nd_2);

bool compare_id_node(const void *nd_1, const void *nd_2);

bool greater_id_node(const void *nd_1, const void *nd_2);

bool compare_name_node(const void *nd_1, const void *nd_2);

void memcpy_node(const void *element, char *stack, uint64_t *offset);

void *memget_node(const char *stack, uint64_t *offset);

uint32_t get_node_id(const void *nd);

void free_node(void *nd);
