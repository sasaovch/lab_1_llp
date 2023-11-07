#pragma once

#include "../managers/page_manager.h"
#include "../data/function_helper.h"
#include "../data/enums.h"
#include "../data/entity.h"
#include "../data/iterator.h"
#include <stdint.h>

bool create_element(
    Cursor *cursor, const void *element,
    TypeOfElement element_type, const char *type,
    const FunctionHelper *function_helper,
    const uint32_t *id
);

void *find_element(
    const Cursor *cursor, const Page *page, 
    uint64_t size_of_element_malloc, 
    const void *find_elem, uint64_t *offset_,
    const FunctionHelper *function_helper
);

void *get_elements_by_condition(
    Cursor *cursor, Entity *entity, 
    uint64_t size_of_element_malloc, 
    void *helper, 
    FunctionHelper *function_helper
);

bool delete_element(
    const Cursor *cursor, const void *element, 
    uint64_t size_of_sturcture, 
    const void *type, TypeOfElement element_type, 
    const FunctionHelper *function_helper
);

bool update_element(
    Cursor *cursor, const void *old_element, 
    const void *new_element, uint64_t size_of_sturcture, 
    const void *type, TypeOfElement element_type, 
    const FunctionHelper *function_helper
);

void remove_bid_element(
    const Cursor *cursor, const Page *page, 
    
    Entity *entity, uint64_t size_of_element,
    const uint64_t *pointer
);

void remove_small_element(
        const Cursor *cursor, const Page *page,
        Entity *entity, uint32_t offset, 
        uint64_t size_of_element, const uint64_t *pointer
);

Iterator *select_element(
    const Cursor *cursor,
    TypeOfElement element_type, const char *type,
    uint64_t size_of_element_malloc, 
    const void *helper, 
    const FunctionHelper *function_helper
);

uint32_t *replace_element(
    const Cursor *cursor, const Page *page, 
    uint64_t* offset,
    const void *find_el, const void *new_element, 
    const FunctionHelper *function_helper
);

uint32_t *delete_and_create_element(
    Cursor *cursor, Page *page, 
    Entity *entity, uint64_t pointer,
    uint64_t* offset, 
    const void *old_element, const void *new_element, 
    const FunctionHelper *function_helper
);
