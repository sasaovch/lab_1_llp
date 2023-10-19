#ifndef LAB_1_COMMON
#define LAB_1_COMMON

#include "./type.h"

bool return_true(void* d, void* n);

bool is_property_for_node(void* rel, void* nd);

bool is_relationship_for_node(void* rel, void* nd) ;

uint32_t* create_element(
    Cursor* cursor, void* element,
    TypeOfElement type, const char* name_type,
    uint64_t (*get_size_of_element)(void*),
    void (*write_element_to_file)(Cursor*, PageHeader*, Entity*, void*, const uint64_t*)
);

void* find_element(
    Cursor* cursor, Entity* meta_page, 
    Page* page, uint64_t size_of_element, 
    void* find_elem, uint64_t* offset_,
    bool (*comparator)(void*, void*),
    uint64_t (*get_size_of_element)(void *),
    void* (*read_big_element)(Cursor* cursor, Entity* meta_page, PageHeader* page_header, void* element, const uint64_t* offset_, char* body, uint32_t* read_block)
);

void* get_elements_by_condition(
    Cursor* cursor, Entity* meta_page, 
    uint64_t size_of_element, 
    void* helper, 
    void* (*read_big_element)(Cursor* , Entity* , PageHeader* , void* , const uint64_t* , char* , uint32_t* ), 
    bool (*condition)(void*, void*), uint64_t (*get_size_of_element)(void*),
    void (*write_element)(void*, Entity*, char*, uint64_t*)
);

void delete_element(
    Cursor* cursor, void* element, 
        uint64_t (*get_size_of_element)(void *), 
        uint64_t size_of_sturcture, void* name_type, TypeOfElement type, 
        bool (*comparator)(void*, void*),
        void* (*read_big_element)(Cursor* cursor, Entity* meta_page, PageHeader* page_header, void* element, const uint64_t* offset_, char* body, uint32_t* read_block)
);

void update_element(
    Cursor* cursor, void* old_element, 
        void* new_element, uint64_t (*get_size_of_element)(void *), 
        uint64_t size_of_sturcture, void *name_type, TypeOfElement type, 
        void *(*read_big_element)(Cursor* cursor, Entity* meta_page, PageHeader* page_header, void* element, const uint64_t* offset_, char* body, uint32_t* read_block), 
        bool (*comparator)(void*, void*), 
        void (*write_element_to_file)(Cursor*, PageHeader*, Entity*, void*, const uint64_t*)
);

#endif
