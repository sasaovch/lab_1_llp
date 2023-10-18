#ifndef LAB_1_TYPE
#define LAB_1_TYPE

#include "./data.h"
#include "./page_utils.h"

void print_type(Entity* meta_page);

void save_meta_page_and_create_new(Cursor* cursor);

void create_page_for_type(Cursor* cursor);

Entity* get_entity(Cursor* cursor, TypeOfElement type, const char* name, uint64_t* pointer, Entity* meta_page);

bool create_type(Cursor* cursor, Entity* meta_page);

bool delete_type(Cursor* cursor, Entity* meta_page);

bool update_type(Cursor* cursor, Entity* old_meta_page, Entity* new_meta_page);

#endif
