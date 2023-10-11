#include "common.c"
#include "data.c"
#include "property.c"
#include "node.c"
#include "relationship.c"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void print_property_by_node(Cursor* cursor, Node* node) {
    uint64_t pointer = 0;
    uint64_t table_offset = 0;

    Entity* table = (Entity*) malloc(ENTITY_SIZE);
    table = get_entity(cursor, PROPERTY, "proper", &(table_offset), table);
    if (table == NULL) {
        println("Error to find Node with name %s", node->name_type);
        return;
    }

    void* stack = get_elements_by_condition(
                        cursor, table, PROPERTY_SIZE, node, 
                        read_property, is_property_for_node, get_size_of_property, memcpy_property);
    uint32_t number = 0;
    memcpy(&(number), stack, UINT32_T_SIZE);
    Property* no = (Property*) malloc(PROPERTY_SIZE);
    uint64_t offset = UINT32_T_SIZE;
    strcpy(table->name_type, "proper");
    for (uint32_t i = 0; i < number; i++) {
        no = memget_property(no, table, stack, &(offset));
        print_property(no);
    }
}

void print_relationship_by_node(Cursor* cursor, Node* node) {
    PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    Entity* table = (Entity*) malloc(ENTITY_SIZE);
    Entity* entity = (Entity*) malloc(ENTITY_SIZE);
    
    void* page_body = malloc(PAGE_BODY_SIZE);
    int page_num = 0;
    uint64_t init_size = NAME_TYPE_SIZE * 100;
    void* table_stack = malloc(init_size);
    uint64_t offset = 0;

    off_t file_length = lseek(cursor->file->file_descriptor, 0, SEEK_END); // find end of file
    error_exit(file_length, "Failed to find end of file");
    do {
        set_pointer_offset_file(cursor->file, page_num * PAGE_SIZE);

        read_from_file(cursor->file, page_header, PAGE_HEADER_SIZE);        
        read_from_file(cursor->file, page_body, PAGE_BODY_SIZE);

        
        for (uint64_t i = 0; i < (page_header->offset / ENTITY_SIZE); i++) {
            memcpy(table, page_body + i * ENTITY_SIZE, ENTITY_SIZE);
            if (RELATIONSHIP == table->type) {
                uint64_t table_offset = 0;
                entity = get_entity(cursor, RELATIONSHIP, table->name_type, &(table_offset), entity);
                if (entity == NULL) {
                    println("Error to find Entity with name %s", table->name_type);
                }

                void* stack = get_elements_by_condition(
                                    cursor, entity, RELATIONSHIP_SIZE, node, 
                                    read_relationship, is_relationship_for_node, get_size_of_relationship, memcpy_relationship);
                uint32_t number = 0;
                memcpy(&(number), stack, UINT32_T_SIZE);
                Relationship* no = (Relationship*) malloc(RELATIONSHIP_SIZE);
                uint64_t offset_ = UINT32_T_SIZE;
                strcpy(no->name_type, entity->name_type);
                for (uint32_t i = 0; i < number; i++) {
                    no = memget_relationship(no, table, stack, &(offset_));
                    print_relationship(no);
                }
            }
        }
        page_num = page_header->next_block;
    } while (page_num != 0);
    
    free(page_header);
    free(page_body);
    // uint64_t pointer = 0;


    // table = get_entity(cursor, RELATIONSHIP, "proper", &(table_offset), table);
    // if (table == NULL) {
    //     println("Error to find Node with name %s", node->name_type);
    //     return;
    // }

    // void* stack = get_elements_by_condition(
    //                     cursor, table, PROPERTY_SIZE, node, 
    //                     read_property, is_property_for_node, get_size_of_property, memcpy_property);
    // uint32_t number = 0;
    // memcpy(&(number), stack, UINT32_T_SIZE);
    // Property* no = (Property*) malloc(PROPERTY_SIZE);
    // uint64_t offset = UINT32_T_SIZE;
    // strcpy(table->name_type, "proper");
    // for (uint32_t i = 0; i < number; i++) {
    //     no = memget_property(no, table, stack, &(offset));
    //     print_property(no);
    // }
}

void read_all_elements_by_type(Cursor* cursor, TypeOfElement type) {
    Entity* metaPage = (Entity*) malloc(ENTITY_SIZE);
    PageHeader* pageHeader = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    
    void* page_body = malloc(PAGE_BODY_SIZE);
    set_pointer_offset_file(cursor->file, 0);
    read_from_file(cursor->file, pageHeader, PAGE_HEADER_SIZE);
    uint32_t page_num = find_block_before(cursor, pageHeader, metaPage);

    off_t file_length = lseek(cursor->file->file_descriptor, 0, SEEK_END);
    error_exit(file_length, "Failed to find end of file");
    
    do {
        set_pointer_offset_file(cursor->file, page_num * PAGE_SIZE);

        read_from_file(cursor->file, pageHeader, PAGE_HEADER_SIZE);        
        read_from_file(cursor->file, page_body, PAGE_BODY_SIZE);

        
        
        for (int i = 0; i < (pageHeader->offset / ENTITY_SIZE); i++) {
            memcpy(metaPage, page_body, ENTITY_SIZE);
            uint32_t i = find_block_before(cursor, pageHeader, metaPage);
            if (type == META) print_type(metaPage);
            switch (type) {
                case NODE: {
                    void* stack = get_elements_by_condition(
                        cursor, metaPage, NODE_SIZE, NULL, 
                        read_node, return_true, get_size_of_node, memcpy_node);
                    uint32_t number = 0;
                    memcpy(&(number), stack, UINT32_T_SIZE);
                    Node* no = (Node*) malloc(NODE_SIZE);
                    uint64_t offset = UINT32_T_SIZE;
                    for (uint32_t i = 0; i < number; i++) {
                        no = memget_node(no, metaPage, stack, &(offset));
                        // print_node(no);
                    }
                    break;
                }
                case RELATIONSHIP: {
                    void* stack = get_elements_by_condition(
                        cursor, metaPage, RELATIONSHIP_SIZE, NULL, 
                        read_relationship, return_true, get_size_of_relationship, memcpy_relationship);
                    uint32_t number = 0;
                    memcpy(&(number), stack, UINT32_T_SIZE);
                    Relationship* no = (Relationship*) malloc(RELATIONSHIP_SIZE);
                    uint64_t offset = UINT32_T_SIZE;
                    for (uint32_t i = 0; i < number; i++) {
                        no = memget_relationship(no, metaPage, stack, &(offset));
                        print_relationship(no);
                    }
                    break;
                }
                case PROPERTY: {
                    void* stack = get_elements_by_condition(
                        cursor, metaPage, PROPERTY_SIZE, NULL, 
                        read_property, return_true, get_size_of_property, memcpy_property);
                    uint32_t number = 0;
                    memcpy(&(number), stack, UINT32_T_SIZE);
                    Property* no = (Property*) malloc(PROPERTY_SIZE);
                    uint64_t offset = UINT32_T_SIZE;
                    for (uint32_t i = 0; i < number; i++) {
                        no = memget_property(no, metaPage, stack, &(offset));
                        print_property(no);
                    }
                    break;
                }
                case META:
                    print_type(metaPage);
                }
            }
        page_num = pageHeader->next_block;
    } while (page_num != 0);
}