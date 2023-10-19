#ifndef LAB_1_NODE
#define LAB_1_NODE

#include "../io/io.h"

void print_node(Node* node);

void write_node_to_file_big(Cursor* cursor, PageHeader* page_header, Entity* entity, void* nd, const uint64_t* pointer);

void* read_node(Cursor* cursor, Entity* meta_page, PageHeader* page_header, void* element, const uint64_t* offset_, char* body, uint32_t* read_block);

uint64_t get_size_of_node(void* nd);

bool compare_nodes(void* nd_1, void* nd_2);

bool compare_id_node(void* nd_1, void* nd_2);

bool greater_id_node(void* nd_1, void* nd_2);

bool compare_name_node(void* nd_1, void* nd_2);

void memcpy_node(void* element, Entity* meta_page, char* stack, uint64_t* offset);

void* memget_node(void* element, Entity* meta_page, char* stack, uint64_t* offset);

void set_node_id(void* nd, uint32_t id);

#endif
