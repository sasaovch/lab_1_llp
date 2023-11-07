#pragma once

#include "../managers/page_manager.h"
#include "../data/entity.h"
#include "../data/constants.h"

typedef struct __attribute__((packed)) {
    uint32_t id;
    uint32_t parent_id;
    uint32_t child_id;
    char type[NAME_TYPE_LENGTH + 1];
    char parent_type[NAME_TYPE_LENGTH + 1];
    char child_type[NAME_TYPE_LENGTH + 1];
} Relationship;

void print_relationship(const Relationship *relationship);

void write_relationship_to_file(Cursor *cursor, Page *page, Entity *entity, const void *rl, const uint32_t *id);

uint64_t get_size_of_relationship(const void *rl);

uint32_t get_relationship_id(const void *rl);

void *read_relationship_from_file(const Cursor *cursor, Page *page, const uint64_t *offset_);

bool compare_relationship(const void *rl_1, const void *rl_2);

bool compare_parent_relationship(const void *rl_1, const void *rl_2);

bool compare_child_relationship(const void *rl_1, const void *rl_2);

bool compare_id_relationship(const void *rl_1, const void *rl_2);

bool compare_relationship_by_node(const void *rl_1, const void *rl_2);

void memcpy_relationship(const void *element, char *stack, uint64_t *offset);

void *memget_relationship(const char *stack, uint64_t *offset);

void free_relationship(void *rl);
