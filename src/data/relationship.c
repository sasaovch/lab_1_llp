#include "../../include/include.h"
#include "data/data.h"
#include <stdint.h>
void print_relationship(Relationship* relationship) {
    println("Print relationship");
    println("Id - %llu", relationship->id);
    println("Type - %s", relationship->name_type);
    println("Parent length - %i", relationship->parent_length);
    println("Parent - %s", relationship->parent);
    println("Child length - %i", relationship->child_length);
    println("Child - %s", relationship->child);
}

void write_relationship_to_file(Cursor* cursor, void* rl) {
    Relationship* relationship = (Relationship*) rl;

    write_to_file(cursor->file, &(relationship->id), UINT32_T_SIZE);
    write_to_file(cursor->file, &(relationship->parent_length), UINT32_T_SIZE);
    write_to_file(cursor->file, relationship->parent, CHAR_SIZE * relationship->parent_length);
    write_to_file(cursor->file, &(relationship->child_length), UINT32_T_SIZE);
    write_to_file(cursor->file, relationship->child, CHAR_SIZE * relationship->child_length);
}

uint64_t get_size_of_relationship(void* rl) {
    (void) rl;
    return RELATIONSHIP_SIZE - CHAR_SIZE * (NAME_TYPE_SIZE + 1);
}

void write_uint_32_to_file(Cursor* cursor, uint32_t number) {
    write_to_file(cursor->file, &(number), UINT32_T_SIZE);
}
//FIXME: good
void write_relationship_to_file_big(Cursor* cursor, PageHeader* page_header, Entity* entity, void* rel, const uint64_t* pointer) {
    Relationship2* relationship = (Relationship2*) rel;

    set_node_id(relationship, entity->next_id);
    entity->next_id += 1;
    page_header->offset += get_size_of_relationship(relationship);

    write_uint_32_to_file(cursor, relationship->id);
    write_uint_32_to_file(cursor, relationship->parent_id);
    write_uint_32_to_file(cursor, relationship->child_id);

    uint64_t header_offset = page_offset + UINT32_T_SIZE * 2;
    set_pointer_offset_file(cursor->file, header_offset);
    write_to_file(cursor->file, &(page_header->offset), UINT32_T_SIZE);
}
//FIXME: good
void* read_big_element(Cursor* cursor, Entity* meta_page, PageHeader* page_header, void* element, uint64_t* offset, char* body, uint32_t* read_block) {
        (void) cursor;
        (void) page_header;
        (void) read_block;

        Relationship2* relationship = (Relationship2*) element;
        strcpy(relationship->name_type, meta_page->name_type);

        uint32_t id = 0;
        uint32_t parent_id = 0;
        uint32_t child_id = 0;
        
        memcpy(&(id), body + *offset, UINT32_T_SIZE);
        *offset += UINT32_T_SIZE;
        memcpy(&(parent_id), body + *offset, UINT32_T_SIZE);
        *offset += UINT32_T_SIZE;
        memcpy(&(child_id), body + *offset, UINT32_T_SIZE);
        *offset += UINT32_T_SIZE;
        
        relationship->id = id;
        relationship->parent_id = parent_id;
        relationship->child_id = child_id;
        return relationship;
}

bool compare_relationship(void* rl_1, void* rl_2) {
    Relationship* relationship_1 = (Relationship*) rl_1;
    Relationship* relationship_2 = (Relationship*) rl_2;
    return (strcmp(relationship_1->parent, relationship_2->parent) == 0) && (
        relationship_1->name_type == relationship_2->name_type) == 0 && 
        (strcmp(relationship_1->child, relationship_2->child) == 0);
}

bool compare_parent_relationship(void* rl_1, void* rl_2) {
    Relationship* relationship_1 = (Relationship*) rl_1;
    Relationship* relationship_2 = (Relationship*) rl_2;
    return (strcmp(relationship_1->parent, relationship_2->parent) == 0);
}

bool compare_child_relationship(void* rl_1, void* rl_2) {
    Relationship* relationship_1 = (Relationship*) rl_1;
    Relationship* relationship_2 = (Relationship*) rl_2;
    return (strcmp(relationship_1->child, relationship_2->child) == 0);
}

bool compare_id_relationship(void* rl_1, void* rl_2) {
    Relationship* relationship_1 = (Relationship*) rl_1;
    Relationship* relationship_2 = (Relationship*) rl_2;
    return relationship_1->id == relationship_2->id;
}

void* read_relationship(void* element, Entity* meta_page, char* body, uint64_t* offset) {
    Relationship* relationship = (Relationship*) element;
    strcpy(relationship->name_type, meta_page->name_type);

    uint32_t id = 0;
    uint32_t parent_length = 0;
    uint32_t child_length = 0;

    memcpy(&(id), body + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(&(parent_length), body + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    char* parent = malloc(CHAR_SIZE * parent_length);
    memcpy(parent, body + *offset, CHAR_SIZE * parent_length);
    *offset += CHAR_SIZE * parent_length;
    
    memcpy(&(child_length), body + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    char* child = malloc(CHAR_SIZE * child_length);
    memcpy(child, body + *offset, CHAR_SIZE * child_length);
    *offset += CHAR_SIZE * child_length;
    
    relationship->id = id;
    relationship->parent_length = parent_length;
    relationship->parent = parent;
    relationship->child_length = child_length;
    relationship->child = child;

    return relationship;
}

void memcpy_relationship(void* element, Entity* meta_page, char* stack, uint64_t* offset) {
    Relationship* relationship = (Relationship*) element;
    (void) meta_page;

    uint32_t parent_length = relationship->parent_length;
    uint32_t child_length = relationship->child_length;

    memcpy(stack + *offset, &(relationship->id), UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(stack + *offset, &(parent_length), UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(stack + *offset, relationship->parent, CHAR_SIZE * parent_length);
    *offset += CHAR_SIZE * parent_length;
    
    memcpy(stack + *offset, &(child_length), UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(stack + *offset, relationship->child, CHAR_SIZE * child_length);
    *offset += CHAR_SIZE * child_length;
}

void* memget_relationship(void* element, Entity* meta_page, char* stack, uint64_t* offset) {
    Relationship* relationship = (Relationship*) element;
    strcpy(relationship->name_type, meta_page->name_type);

    uint32_t id = 0;
    uint32_t parent_length = 0;
    uint32_t child_length = 0;

    memcpy(&(id), stack + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(&(parent_length), stack + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    char* parent = malloc(CHAR_SIZE * parent_length);
    memcpy(parent, stack + *offset, CHAR_SIZE * parent_length);
    *offset += CHAR_SIZE * parent_length;
    
    memcpy(&(child_length), stack + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    char* child = malloc(CHAR_SIZE * child_length);
    memcpy(child, stack + *offset, CHAR_SIZE * child_length);
    *offset += CHAR_SIZE * child_length;
    
    relationship->id = id;
    relationship->parent_length = parent_length;
    relationship->parent = parent;
    relationship->child_length = child_length;
    relationship->child = child;
    print_relationship(relationship);

    return relationship;
}

void set_relation_id(void* nd, uint32_t id) {
    Relationship* node = (Relationship*) nd;
    node->id = id;
}
