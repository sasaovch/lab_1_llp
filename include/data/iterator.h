#pragma once

#include "../managers/file_manager.h"
#include "../data/entity.h"
#include "../data/function_helper.h"

typedef struct {
    const Cursor *cursor;
    Entity *entity;
    const FunctionHelper *function_helper;

    uint32_t *read_block_;
    uint64_t *offset_;
    
    void *element;
    const void *helper;
} Iterator;

typedef struct{
    const Cursor *cursor;
    Entity *entity;
    Page *page;
    uint64_t *offset_;
    Iterator *iterator;
} EntityIterator;

void *next(const Iterator *iterator);

bool has_next(Iterator *iterator);

void *entity_next(const EntityIterator *entity_iterator);

bool entity_has_next(EntityIterator *entity_iterator);

void free_iter(Iterator *iterator);

void free_entity_iter(EntityIterator *entity_iterator);

EntityIterator *get_entity_iterator(Cursor *cursor, TypeOfElement element_type);
