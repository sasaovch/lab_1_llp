#include "../../include/include.h"
#include "data/data.h"
#include "data/node.h"
#include <stdint.h>
void print_node(Node* node) {
    println("Print node");
    println("Id - %llu", node->id);
    println("Type - %s", node->name_type);
    println("Length - %i", node->name_length);
    println("Name - %s", node->name);
}
void write_uint_32_to_file(Cursor* cursor, uint32_t number) {
    write_to_file(cursor->file, &(number), UINT32_T_SIZE);
}
void write_type_to_file(Cursor* cursor, char* type) {
    write_to_file(cursor->file, type, NAME_TYPE_SIZE + 1);
}
void write_string_to_file(Cursor* cursor, char* string, uint32_t length) {
    write_to_file(cursor->file, string, CHAR_SIZE * length);
}
void write_minimum_node(Cursor* cursor, void* nd) {
    Node* node = (Node*) nd;
    write_uint_32_to_file(cursor, node->id);
    write_uint_32_to_file(cursor, node->name_length);
}
void write_node_to_file(Cursor* cursor, void* nd) {
    Node* node = (Node*) nd;

    write_to_file(cursor->file, &(node->id), UINT32_T_SIZE);
    write_to_file(cursor->file, &(node->name_length), UINT32_T_SIZE);
    write_to_file(cursor->file, node->name, CHAR_SIZE * node->name_length);
}
//FIXME: looks good needs to test
void write_node_to_file_big(Cursor* cursor, PageHeader* page_header, Entity* entity, void* nd, const uint64_t* pointer) {
    Node* node = (Node*) nd;

    set_node_id(node, entity->next_id);
    entity->next_id += 1;
    page_header->offset += UINT32_T_SIZE * 2;

    write_uint_32_to_file(cursor, node->id);
    write_uint_32_to_file(cursor, node->name_length);
    write_big_string_to_file(cursor, page_header, entity, node->name_length, global_offset, page_offset, node->name);
}
void read_big_string_from_file(Cursor* cursor, PageHeader* page_header, char* body, char* string, uint32_t length, uint64_t* offset, uint32_t* read_block) {
    uint32_t to_read_length = 0;
    
    while (length != 0) {
        if (*offset == PAGE_BODY_SIZE) {
            *read_block = page_header->next_block;
            uint64_t page_offset = *read_block * PAGE_SIZE;

            set_pointer_offset_file(cursor->file, page_offset);
            read_from_file(cursor->file, page_header, PAGE_HEADER_SIZE);  
            read_from_file(cursor->file, body, PAGE_BODY_SIZE);
            offset = 0;
        }

        if (length <= PAGE_BODY_SIZE - *offset) {
            to_read_length = length;
        } else {
            to_read_length = PAGE_BODY_SIZE - *offset;
        }
    
        memcpy(string, body + *offset, to_read_length);
        *offset += to_read_length;
        length -= to_read_length;
    }
}
//FIXME: need to test
void* read_big_element(Cursor* cursor, Entity* meta_page, PageHeader* page_header, void* element, const uint64_t* offset_, char* body, uint32_t* read_block) {
        Node* node = (Node*) element;
        strcpy(node->name_type, meta_page->name_type);

        uint32_t id = 0;
        uint32_t name_length = 0;

        uint64_t offset = *offset_;
        
        memcpy(&(id), body + offset, UINT32_T_SIZE);
        offset += UINT32_T_SIZE;
        memcpy(&(name_length), body + offset, UINT32_T_SIZE);
        offset += UINT32_T_SIZE;
        
        char* name = malloc(CHAR_SIZE * name_length);
        read_big_string_from_file(cursor, page_header, body, name, name_length, &(offset), read_block);
            
        node->id = id;
        node->name_length = name_length;
        node->name = name;
        return node;
}

uint64_t get_size_of_node(void* nd) {
    Node* node = (Node*) nd;
    return UINT32_T_SIZE + UINT32_T_SIZE + CHAR_SIZE * node->name_length;
}

bool compare_nodes(void* nd_1, void* nd_2) {
    Node* node_1 = (Node*) nd_1;
    Node* node_2 = (Node*) nd_2;
    return (strcmp(node_1->name, node_2->name) == 0) && (node_1->name_type == node_2->name_type) == 0;
}

bool compare_id_node(void* nd_1, void* nd_2) {
    Node* node_1 = (Node*) nd_1;
    Node* node_2 = (Node*) nd_2;
    return node_1->id == node_2->id;
}

bool greater_id_node(void* nd_1, void* nd_2) {
    Node* node_1 = (Node*) nd_1;
    Node* node_2 = (Node*) nd_2;
    return node_1->id >= node_2->id;
}

bool compare_name_node(void* nd_1, void* nd_2) {
    Node* node_1 = (Node*) nd_1;
    Node* node_2 = (Node*) nd_2;
    return (strcmp(node_1->name, node_2->name) == 0);
}

void* read_node(void* element, Entity* metaPage, char* body, uint64_t* offset) {
    Node* node = (Node*) element;
    strcpy(node->name_type, metaPage->name_type);

    uint32_t name_length = 0;
    uint32_t id = 0;
    
    memcpy(&(id), body + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;
    memcpy(&(name_length), body + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;
    
    char* name = malloc(CHAR_SIZE * name_length);
    memcpy(name, body + *offset, CHAR_SIZE * name_length);
    *offset += CHAR_SIZE * name_length;
    
    node->id = id;
    node->name_length = name_length;
    node->name = name;
    
    return node;
}
//FIXME: think about meta_page
void memcpy_node(void* element, Entity* metaPage, char* stack, uint64_t* offset) {
    Node* node = (Node*) element;
    
    
    strcpy(node->name_type, metaPage->name_type);

    memcpy(stack + *offset, &(node->id), UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(stack + *offset, &(node->name_length), UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(stack + *offset, node->name, CHAR_SIZE * node->name_length);
    *offset += CHAR_SIZE * node->name_length;
}

void* memget_node(void* element, Entity* meta_page, char* stack, uint64_t* offset) {
    Node* node = (Node*) element;
    uint32_t name_length = 0;
    uint32_t id = 0;
    strcpy(node->name_type, meta_page->name_type);

    memcpy(&(id), stack + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(&(name_length), stack + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    void* name = malloc(CHAR_SIZE * name_length);
    memcpy(name, stack + *offset, CHAR_SIZE * name_length);
    *offset += CHAR_SIZE * name_length;

    node->id = id;
    node->name_length = name_length;
    node->name = name;

    return node;
}


void set_node_id(void* nd, uint32_t id) {
    Node* node = (Node*) nd;
    node->id = id;
}
