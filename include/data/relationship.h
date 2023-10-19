#ifndef LAB_1_RELATIONSHIP
#define LAB_1_RELATIONSHIP

#include "../io/io.h"

// void print_relationship(Relationship* relationship);

// void write_relationship_to_file(Cursor* cursor, void* rl);

// uint64_t get_size_of_relationship(void* rl);

// bool compare_relationship(void* rl_1, void* rl_2);

// bool compare_parent_relationship(void* rl_1, void* rl_2);

// bool compare_child_relationship(void* rl_1, void* rl_2);

// bool compare_id_relationship(void* rl_1, void* rl_2);

// void* read_relationship(void* element, Entity* meta_page, char* body, uint64_t* offset);

// void memcpy_relationship(void* element, Entity* meta_page, char* stack, uint64_t* offset);

// void* memget_relationship(void* element, Entity* meta_page, char* stack, uint64_t* offset);

// void set_relation_id(void* nd, uint32_t id);

// void print_relationship2(Relationship2* relationship);




// bool compare_parent_relationship2(void* rl_1, void* rl_2);

// bool compare_child_relationship2(void* rl_1, void* rl_2);

// bool compare_id_relationship2(void* rl_1, void* rl_2);





void print_relationship2(Relationship2* relationship);
void write_relationship2_to_file(Cursor* cursor, void* rl);
void write_relationship2_to_file_big(Cursor* cursor, PageHeader* page_header, Entity* entity, void* rel, const uint64_t* pointer);
uint64_t get_size_of_relationship2(void* rl);
void set_relation2_id(void* nd, uint32_t id);
void* read_relationship2(Cursor* cursor, Entity* meta_page, PageHeader* page_header, void* element, const uint64_t* offset, char* body, uint32_t* read_block);

bool compare_relationship2(void* rl_1, void* rl_2);
bool compare_parent_relationship2(void* rl_1, void* rl_2);
bool compare_child_relationship2(void* rl_1, void* rl_2);
bool compare_id_relationship2(void* rl_1, void* rl_2);
void memcpy_relationship2(void* element, Entity* meta_page, char* stack, uint64_t* offset);
void* memget_relationship2(void* element, Entity* meta_page, char* stack, uint64_t* offset);

#endif
