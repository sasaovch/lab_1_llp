#pragma once

#include "../data/function_helper.h"
#include "../managers/file_manager.h"
#include "../data/entity.h"

typedef struct {
    bool (*condition)(const void*, const void*);
    uint64_t (*get_size_of_element)(const void*);
    uint64_t (*get_min_size_of_element)(void*);
    void (*write_element_to_file)(Cursor*, Page*, Entity*, const void*, const uint32_t*);
    void *(*read_element_from_file)(const Cursor*, Page*, const uint64_t *);
    void (*memcpy_element)(const void*, char*, uint64_t*);
    void (*free_element)(void*);
    uint32_t (*get_id)(const void*);
} FunctionHelper;
