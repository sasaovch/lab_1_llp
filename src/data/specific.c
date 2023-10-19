#include "../../include/include.h"
#include "data/data.h"
#include "data/specific.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// void print_property_by_node(Cursor* cursor, Node* node) {
//     // uint64_t pointer = 0;
//     uint64_t table_offset = 0;

//     Entity* table = (Entity*) malloc(ENTITY_SIZE);
//     table = get_entity(cursor, PROPERTY, "proper", &(table_offset), table);
//     if (table == NULL) {
//         println("Error to find Node with name %s", node->name_type);
//         return;
//     }

//     void* stack = get_elements_by_condition(
//                         cursor, table, PROPERTY_SIZE, node, 
//                         read_property2, is_property_for_node, get_size_of_property, memcpy_property);
//     uint32_t number = 0;
//     memcpy(&(number), stack, UINT32_T_SIZE);
//     Property* no = (Property*) malloc(PROPERTY_SIZE);
//     uint64_t offset = UINT32_T_SIZE;
//     strcpy(table->name_type, "proper");
//     for (uint32_t i = 0; i < number; i++) {
//         no = memget_property(no, table, stack, &(offset));
//         print_property(no);
//     }
// }

// void print_relationship_by_node(Cursor* cursor, Node* node) {
//     PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);
//     Entity* table = (Entity*) malloc(ENTITY_SIZE);
//     Entity* entity = (Entity*) malloc(ENTITY_SIZE);
    
//     char* page_body = (char*) malloc(PAGE_BODY_SIZE);
//     int page_num = 0;
//     // uint64_t init_size = NAME_TYPE_SIZE * 100;
//     // void* table_stack = malloc(init_size);
//     // uint64_t offset = 0;

//     off_t file_length = lseek(cursor->file->file_descriptor, 0, SEEK_END); // find end of file
//     error_exit(file_length, "Failed to find end of file");
//     do {
//         set_pointer_offset_file(cursor->file, page_num * PAGE_SIZE);

//         read_from_file(cursor->file, page_header, PAGE_HEADER_SIZE);        
//         read_from_file(cursor->file, page_body, PAGE_BODY_SIZE);

        
//         for (uint64_t i = 0; i < (page_header->offset / ENTITY_SIZE); i++) {
//             memcpy(table, page_body + i * ENTITY_SIZE, ENTITY_SIZE);
//             if (RELATIONSHIP == table->type) {
//                 uint64_t table_offset = 0;
//                 entity = get_entity(cursor, RELATIONSHIP, table->name_type, &(table_offset), entity);
//                 if (entity == NULL) {
//                     println("Error to find Entity with name %s", table->name_type);
//                 }

//                 void* stack = get_elements_by_condition(
//                                     cursor, entity, RELATIONSHIP_SIZE, node, 
//                                     read_relationship, is_relationship_for_node, get_size_of_relationship, memcpy_relationship);
//                 uint32_t number = 0;
//                 memcpy(&(number), stack, UINT32_T_SIZE);
//                 Relationship* no = (Relationship*) malloc(RELATIONSHIP_SIZE);
//                 uint64_t offset_ = UINT32_T_SIZE;
//                 strcpy(no->name_type, entity->name_type);
//                 for (uint32_t i = 0; i < number; i++) {
//                     no = memget_relationship(no, table, stack, &(offset_));
//                     print_relationship(no);
//                 }
//             }
//         }
//         page_num = page_header->next_block;
//     } while (page_num != 0);
    
//     free(page_header);
//     free(page_body);
//     // uint64_t pointer = 0;


//     // table = get_entity(cursor, RELATIONSHIP, "proper", &(table_offset), table);
//     // if (table == NULL) {
//     //     println("Error to find Node with name %s", node->name_type);
//     //     return;
//     // }

//     // void* stack = get_elements_by_condition(
//     //                     cursor, table, PROPERTY_SIZE, node, 
//     //                     read_property, is_property_for_node, get_size_of_property, memcpy_property);
//     // uint32_t number = 0;
//     // memcpy(&(number), stack, UINT32_T_SIZE);
//     // Property* no = (Property*) malloc(PROPERTY_SIZE);
//     // uint64_t offset = UINT32_T_SIZE;
//     // strcpy(table->name_type, "proper");
//     // for (uint32_t i = 0; i < number; i++) {
//     //     no = memget_property(no, table, stack, &(offset));
//     //     print_property(no);
//     // }
// }

void read_all_elements_by_type(Cursor* cursor, TypeOfElement type) {
    Entity* entity = (Entity*) malloc(ENTITY_SIZE);
    PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    
    char* page_body = malloc(PAGE_BODY_SIZE);
    set_pointer_offset_file(cursor->file, 0);
    read_from_file(cursor->file, page_header, PAGE_HEADER_SIZE);
    uint32_t page_num = find_block_before(cursor, page_header, entity);

    off_t file_length = lseek(cursor->file->file_descriptor, 0, SEEK_END);
    error_exit(file_length, "Failed to find end of file");
    
    do {
        set_pointer_offset_file(cursor->file, page_num * PAGE_SIZE);

        read_from_file(cursor->file, page_header, PAGE_HEADER_SIZE);        
        read_from_file(cursor->file, page_body, PAGE_BODY_SIZE);


        for (uint32_t i = 0; i < (page_header->offset / ENTITY_SIZE); i++) {
            memcpy(entity, page_body + i * ENTITY_SIZE, ENTITY_SIZE);
            // uint32_t block_before = find_block_before(cursor, pageHeader, metaPage);
            if (type == META) print_type(entity);
            if (entity->type != type) continue;
            switch (type) {
                case NODE: {
                    void* stack = get_elements_by_condition(
                        cursor, entity, NODE_SIZE, NULL, 
                        read_node, return_true, get_size_of_node, memcpy_node);
                    uint32_t number = 0;
                    memcpy(&(number), stack, UINT32_T_SIZE);
                    Node* no = (Node*) malloc(NODE_SIZE);
                    uint64_t offset = UINT32_T_SIZE;
                    for (uint32_t j = 0; j < number; j++) {
                        no = memget_node(no, entity, stack, &(offset));
                        print_node(no);
                    }
                    break;
                }
                case RELATIONSHIP: {
                    void* stack = get_elements_by_condition(
                        cursor, entity, RELATIONSHIP_SIZE, NULL, 
                        read_relationship2, return_true, get_size_of_relationship2, memcpy_relationship2);
                    uint32_t number = 0;
                    memcpy(&(number), stack, UINT32_T_SIZE);
                    Relationship2* no = (Relationship2*) malloc(RELATIONSHIP_SIZE);
                    uint64_t offset = UINT32_T_SIZE;
                    for (uint32_t j = 0; j < number; j++) {
                        no = memget_relationship2(no, entity, stack, &(offset));
                        print_relationship2(no);
                    }
                    break;
                }
                case PROPERTY: {
                    void* stack = get_elements_by_condition(
                        cursor, entity, PROPERTY_SIZE, NULL, 
                        read_property2, return_true, get_size_of_property2, memcpy_property2);
                    uint32_t number = 0;
                    memcpy(&(number), stack, UINT32_T_SIZE);
                    Property2* no = (Property2*) malloc(PROPERTY_SIZE);
                    uint64_t offset = UINT32_T_SIZE;
                    for (uint32_t j = 0; j < number; j++) {
                        no = memget_property2(no, entity, stack, &(offset));
                        print_property2(no);
                    }
                    break;
                }
                case META:
                    print_type(entity);
                }
            }
        page_num = page_header->next_block;
    } while (page_num != 0);
}

void write_big_string_to_file(Cursor* cursor, PageHeader* page_header, Entity* entity, uint32_t length, char* string) {
    set_pointer_offset_file(cursor->file, page_header->block_number * PAGE_SIZE + page_header->offset + PAGE_HEADER_SIZE);
    uint32_t string_offset = 0;
    char* string_to_write = (char*) calloc(1, PAGE_BODY_SIZE);
    uint32_t page_offset = page_header->block_number * PAGE_SIZE;
    
    while (length != 0) {
        uint32_t write_length = 0;
        if (length <= PAGE_BODY_SIZE - page_header->offset) {
            write_length = length;
        } else {
            write_length = PAGE_BODY_SIZE - page_header->offset;
        }
        memcpy(string_to_write, string + string_offset, write_length);

        write_string_to_file(cursor, string_to_write, write_length);

        length -= write_length;
        string_offset += write_length;
        page_header->offset += write_length;

        if (page_header->offset == PAGE_BODY_SIZE && length != 0) {
            uint64_t header_offset = page_header->block_number * PAGE_SIZE + UINT32_T_SIZE * 2;
            set_pointer_offset_file(cursor->file, header_offset);
            write_to_file(cursor->file, &(page_header->offset), UINT32_T_SIZE);

            create_new_page(cursor, page_header, page_header->block_number * PAGE_SIZE);
        
            entity->last_block = cursor->number_of_pages;
            page_offset = entity->last_block * PAGE_SIZE;
            uint32_t global_offset = page_offset + PAGE_HEADER_SIZE;

            set_pointer_offset_file(cursor->file, global_offset);
        }
    }
    uint64_t header_offset = page_header->block_number * PAGE_SIZE + UINT32_T_SIZE * 2;
    set_pointer_offset_file(cursor->file, header_offset);
    write_to_file(cursor->file, &(page_header->offset), UINT32_T_SIZE);
    free(string_to_write);
}

void read_big_string_from_file(Cursor* cursor, PageHeader* page_header, char* body, char* string, uint32_t length, uint64_t* offset, uint32_t* read_block) {
    uint32_t to_read_length = 0;
    
    while (length != 0) {
        if (length <= PAGE_BODY_SIZE - *offset) {
            to_read_length = length;
        } else {
            to_read_length = PAGE_BODY_SIZE - *offset;
        }
    
        memcpy(string, body + *offset, to_read_length);
        *offset += to_read_length;
        length -= to_read_length;

        if (*offset == PAGE_BODY_SIZE) {
            *read_block = page_header->next_block;
            uint64_t page_offset = *read_block * PAGE_SIZE;

            set_pointer_offset_file(cursor->file, page_offset);
            read_from_file(cursor->file, page_header, PAGE_HEADER_SIZE);  
            read_from_file(cursor->file, body, PAGE_BODY_SIZE);
            *offset = 0;
        }
    }
}
