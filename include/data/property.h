#ifndef LAB_1_PROPERTY
#define LAB_1_PROPERTY

#include "../io/io.h"

// void print_property(Property* property);

// void write_property_to_file(Cursor* cursor, void* pr);

// uint64_t get_size_of_property(void* pr);

// bool compare_property(void* pr_1, void* pr_2) ;

// bool compare_subject_property(void* pr_1, void* pr_2);

// bool compare_key_property(void* pr_1, void* pr_2);

// void* read_property(void* element, Entity* metaPage, char* body, uint64_t* offset);

// void memcpy_property(void* element, Entity* metaPage, char* stack, uint64_t* offset);

// void* memget_property(void* element, Entity* metaPage, char* stack, uint64_t* offset);

// void set_property_id(void* nd, uint32_t id);

// void print_property2(Property2* property);

// void write_property2_to_file(Cursor* cursor, void* pr);

// uint64_t get_size_of_property2(void* pr);

// bool compare_property2(void* pr_1, void* pr_2) ;

// bool compare_subject_property2(void* pr_1, void* pr_2);

// bool compare_key_property2(void* pr_1, void* pr_2);

// void* read_property2(Cursor* cursor, Entity* meta_page, PageHeader* page_header, void* element, const uint64_t* offset_, char* body, uint32_t* read_block);

// void memcpy_property2(void* element, Entity* metaPage, char* stack, uint64_t* offset);

// void* memget_property2(void* element, Entity* metaPage, char* stack, uint64_t* offset);

// void set_property2_id(void* nd, uint32_t id);

void print_property2(Property2* property);
void write_property2_to_file(Cursor* cursor, void* pr);
void write_property2_to_file_big(Cursor* cursor, PageHeader* page_header, Entity* entity, void* pr, const uint64_t* pointer);
void* read_property2(Cursor* cursor, Entity* meta_page, PageHeader* page_header, void* element, const uint64_t* offset_, char* body, uint32_t* read_block);
uint64_t get_size_of_property2(void* pr);
void memcpy_property2(void* element, Entity* metaPage, char* stack, uint64_t* offset);
void* memget_property2(void* element, Entity* meta_page, char* stack, uint64_t* offset);
void set_property2_id(void* nd, uint32_t id);

bool compare_property2(void* pr_1, void* pr_2);

bool compare_subject_property2(void* pr_1, void* pr_2) ;
bool compare_key_property2(void* pr_1, void* pr_2) ;

#endif
