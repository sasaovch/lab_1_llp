#ifndef LAB_1_SPEC
#define LAB_1_SPEC

#include "common.h"

void print_property_by_node(Cursor* cursor, Node* node);

void print_relationship_by_node(Cursor* cursor, Node* node);

// void read_all_elements_by_type(Cursor* cursor, TypeOfElement type);

void print_property2_by_node(Cursor* cursor, Node* node);

void print_relationship2_by_node(Cursor* cursor, Node* node);

void read_all_elements_by_type(Cursor* cursor, TypeOfElement type);

void write_big_string_to_file(Cursor* cursor, PageHeader* page_header, Entity* entity, uint32_t length, char* string);
void read_big_string_from_file(Cursor* cursor, PageHeader* page_header, char* body, char* string, uint32_t length, uint64_t* offset, uint32_t* read_block) ;

#endif
