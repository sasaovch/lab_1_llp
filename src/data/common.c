#include "../../include/data/common.h"
#include "data/data.h"
#include "io/io.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


bool return_true(void* d, void* n) {
    (void) d;
    (void) n;
    return true;
}

// bool is_property_for_node(void* rel, void* nd) {
//     Property* prop = (Property*) rel;
//     Node* node = (Node*) nd;
//     return strcmp(prop->subject, node->name) == 0;
// }

// bool is_property2_for_node(void* rel, void* nd) {
//     Property2* prop = (Property2*) rel;
//     Node* node = (Node*) nd;
//     return prop->subject_id == node->id;
// }

// bool is_relationship_for_node(void* rel, void* nd) {
//     Relationship* prop = (Relationship*) rel;
//     Node* node = (Node*) nd;
//     return strcmp(prop->parent, node->name) == 0;
// }

// bool is_relationship2_for_node(void* rel, void* nd) {
//     Relationship2* prop = (Relationship2*) rel;
//     Node* node = (Node*) nd;
//     return prop->parent_id == node->id;
// }

// void write_uint_32_to_file(Cursor* cursor, uint32_t number) {
//     write_to_file(cursor->file, &(number), UINT32_T_SIZE);
// }
// void write_type_to_file(Cursor* cursor, char* type) {
//     write_to_file(cursor->file, type, NAME_TYPE_SIZE + 1);
// }
// void write_string_to_file(Cursor* cursor, char* string, uint32_t length) {
//     write_to_file(cursor->file, string, CHAR_SIZE * length);
// }
// void write_minimum_node(Cursor* cursor, void* nd) {
//     Node* node = (Node*) nd;
//     write_uint_32_to_file(cursor, node->id);
//     write_uint_32_to_file(cursor, node->name_length);
// }
// void write_minimum_relationship(Cursor* cursor, void* rel) {
//     Relationship2* relationship = (Relationship2*) rel;
//     write_uint_32_to_file(cursor, relationship->id);
//     write_uint_32_to_file(cursor, relationship->parent_id);
//     write_uint_32_to_file(cursor, relationship->child_id);
// }
// void write_minimum_property(Cursor* cursor, void* prop) {
//     Property2* property = (Property2*) prop;
//     write_uint_32_to_file(cursor, property->id);
//     write_uint_32_to_file(cursor, property->value_type);
//     write_uint_32_to_file(cursor, property->subject_id);
//     write_uint_32_to_file(cursor, property->key_length);
//     write_uint_32_to_file(cursor, property->value_length);
// }
//FIXME: free element
uint32_t* create_element(
    Cursor* cursor, void* element,
    TypeOfElement type, const char* name_type,
    uint64_t (*get_size_of_element)(void*),
    void (*write_element_to_file)(Cursor*, PageHeader*, Entity*, void*, const uint64_t*)
) {
    uint64_t pointer = 0;

    Entity* entity = (Entity*) malloc(ENTITY_SIZE);
    PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    
    entity = get_entity(cursor, type, name_type, &(pointer), entity);
    if (entity == NULL) {
        println("Error to find Node with name %s", name_type);
        return NULL;
    }

    uint32_t page_num = entity->last_block;
    uint64_t page_offset = page_num * PAGE_SIZE;
    uint64_t global_offset = page_offset + page_header->offset + PAGE_HEADER_SIZE;

    set_pointer_offset_file(cursor->file, page_offset);
    read_from_file(cursor->file, page_header, PAGE_HEADER_SIZE);

    if (page_header->offset + get_size_of_element(element) > PAGE_BODY_SIZE) {
        create_new_page(cursor, page_header, page_offset);
        
        entity->last_block = cursor->number_of_pages;
        global_offset = cursor->number_of_pages * PAGE_SIZE + PAGE_HEADER_SIZE;
    }
    set_pointer_offset_file(cursor->file, global_offset);
    write_element_to_file(cursor, page_header, entity, element, &(pointer));

    if (get_size_of_element(element) > PAGE_BODY_SIZE) {
        create_new_page(cursor, page_header, page_offset);
        entity->last_block = cursor->number_of_pages;
    }

    set_pointer_offset_file(cursor->file, pointer + TYPE_OF_ELEMENT_SIZE + UINT32_T_SIZE);
    write_to_file(cursor->file, &(entity->last_block), UINT32_T_SIZE);
    write_to_file(cursor->file, &(entity->next_id), UINT32_T_SIZE);

    free(entity);
    free(page_header);
    //FIXME: check free 
    uint32_t* return_id = malloc(UINT32_T_SIZE);
    *return_id = entity->next_id - 1;
    return return_id;
}

//FIXME: good
void* get_elements_by_condition(
        Cursor* cursor, Entity* meta_page, 
        uint64_t size_of_element, 
        void* helper, 
        void* (*read_big_element)(Cursor*, Entity*, PageHeader*, void*, uint64_t*, char*, uint32_t*), 
        bool (*condition)(void*, void*), uint64_t (*get_size_of_element)(void*),
        void (*write_element)(void*, Entity*, char*, uint64_t*)
    ) {
    uint32_t read_block = meta_page->first_block;
    uint64_t stack_offset = UINT32_T_SIZE;
    uint32_t find_number = 0;
    uint64_t stack_size = PAGE_BODY_SIZE;

    PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    char* body = (char*) malloc(PAGE_BODY_SIZE);
    void* stack = malloc(PAGE_BODY_SIZE);
    void* element = (void*) malloc(size_of_element);

    while (read_block != 0) {
        uint64_t offset = 0;
        uint64_t page_offset = read_block * PAGE_SIZE;

        set_pointer_offset_file(cursor->file, page_offset);
        read_from_file(cursor->file, page_header, PAGE_HEADER_SIZE);  
        read_from_file(cursor->file, body, PAGE_BODY_SIZE);

        while (offset < page_header->offset) {

            element = read_big_element(cursor, meta_page, page_header, element, &(offset), body, &(read_block));
            if (condition(element, helper)) {
                find_number++;
            
                if (stack_offset + get_size_of_element(element) >= stack_size) {
                    stack_size += (get_size_of_element(element) / PAGE_BODY_SIZE + 1) * PAGE_BODY_SIZE;
                    stack = realloc(stack, stack_size);
                }
                write_element(element, meta_page, stack, &(stack_offset));
            }
            offset = (offset + get_size_of_element(element)) % PAGE_BODY_SIZE;
        }
        read_block = page_header->next_block;
    }
    memcpy(stack, &(find_number), UINT32_T_SIZE);

    free(element);
    free(page_header);
    free(body);
    
    return stack;
}

void* find_element(
        Cursor* cursor, Entity* meta_page, 
        Page* page, uint64_t size_of_element, 
        void* find_elem, uint64_t* offset_,
        bool (*comparator)(void*, void*),
        uint64_t (*get_size_of_element)(void *),
        void* (*read_big_element)(Cursor*, Entity*, PageHeader*, void*, const uint64_t*, char*, uint32_t*)
    ) {
    PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);

    PageHeader* const_page_header = page->page_header;
    uint32_t read_block = meta_page->first_block;
    memcpy(page_header, page->page_header, PAGE_HEADER_SIZE);
    
    void* body = page->body;
    void* element = (void*) malloc(size_of_element);

    while (read_block != 0) {
        uint64_t offset = 0;
        uint64_t page_offset = read_block * PAGE_SIZE;
        
        set_pointer_offset_file(cursor->file, page_offset);
        read_from_file(cursor->file, page_header, PAGE_HEADER_SIZE);  
        read_from_file(cursor->file, body, PAGE_BODY_SIZE);
        memcpy(const_page_header, page_header, PAGE_HEADER_SIZE);

        while (offset < page_header->offset) {
            element = read_big_element(cursor, meta_page, page_header, element, &(offset), body, &(read_block));

            if (comparator(element, find_elem)) {
                page->body = body;
                page->page_header = const_page_header;
                *offset_ = offset;
                free(page_header);
                return element;
            }
            offset = (offset + get_size_of_element(element)) % PAGE_BODY_SIZE;
        }
        read_block = page_header->next_block;
    }
    free(element);
    free(page_header);
    return NULL;
}

void cut_node_blocks(Cursor* cursor, void* new_body, Page* page, uint64_t offset, uint64_t pointer, Entity* entity, void* element) {
    Node* nd = (Node*) element;
    uint32_t lenght = nd->name_length;
    PageHeader* new_page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    char* copy_body = (char*) malloc(PAGE_BODY_SIZE);
    char* empty_body = (char*) calloc(1, PAGE_BODY_SIZE);

    memcpy(new_body, page->body, offset);
    // memcpy(new_body + offset, page->body + offset + get_size_of_element(find_el), PAGE_BODY_SIZE - offset - get_size_of_element(find_el));
    set_pointer_offset_file(cursor->file, page->page_header->block_number * PAGE_SIZE + PAGE_HEADER_SIZE - UINT32_T_SIZE);
    write_to_file(cursor->file, &(offset), UINT32_T_SIZE);
    write_to_file(cursor->file, new_body, PAGE_BODY_SIZE);
    uint32_t* stack = (uint32_t*) malloc(lenght);
    lenght -= (PAGE_BODY_SIZE - offset - UINT32_T_SIZE * 2);
    memcpy(new_page_header, page->page_header, PAGE_HEADER_SIZE);
    int i = 0;
    while (lenght > 0) {
        set_pointer_offset_file(cursor->file, new_page_header->next_block * PAGE_SIZE);
        read_from_file(cursor->file, new_page_header, PAGE_HEADER_SIZE);
        read_from_file(cursor->file, copy_body, PAGE_BODY_SIZE);
        if (lenght >= PAGE_BODY_SIZE) {
            uint32_t zero = 0;
            set_pointer_offset_file(cursor->file, new_page_header->block_number * PAGE_SIZE + PAGE_HEADER_SIZE - UINT32_T_SIZE);
            write_to_file(cursor->file, &(zero), UINT32_T_SIZE);
            write_to_file(cursor->file, empty_body, PAGE_BODY_SIZE);
            stack[i] = new_page_header->block_number;
            cut_blocks(cursor, page->page_header, &(pointer), entity);
        } else {
            free(new_body);
            void* n_body = calloc(1, PAGE_BODY_SIZE);
            memcpy(n_body, copy_body + lenght, lenght);
            set_pointer_offset_file(cursor->file, new_page_header->block_number * PAGE_SIZE + PAGE_HEADER_SIZE - UINT32_T_SIZE);
            uint32_t new_offset = new_page_header->offset - lenght;
            write_to_file(cursor->file, &(new_offset), UINT32_T_SIZE);
            write_to_file(cursor->file, n_body, PAGE_BODY_SIZE);
            if (new_offset == 0) {
                cut_blocks(cursor, page->page_header, &(pointer), entity);
            }
        }
        lenght -= PAGE_BODY_SIZE;
    }

}

void cut_not_big_element(Cursor* cursor, Page* page, uint64_t offset) {
    char* new_body = malloc(PAGE_BODY_SIZE);
    void* empty_body = calloc(1, PAGE_BODY_SIZE);
    memcpy(new_body, page->body, offset);
    // memcpy(new_body + offset, page->body + offset + get_size_of_element(find_el), PAGE_BODY_SIZE - offset - get_size_of_element(find_el));
    set_pointer_offset_file(cursor->file, page->page_header->block_number * PAGE_SIZE + PAGE_HEADER_SIZE - UINT32_T_SIZE);
    write_to_file(cursor->file, &(offset), UINT32_T_SIZE);
    write_to_file(cursor->file, new_body, PAGE_BODY_SIZE);
    PageHeader* next_header = malloc(PAGE_HEADER_SIZE);
    set_pointer_offset_file(cursor->file, page->page_header->next_block * PAGE_SIZE);
    read_from_file(cursor->file, next_header, PAGE_HEADER_SIZE);
    read_from_file(cursor->file, new_body, PAGE_BODY_SIZE);
    memcpy(empty_body, new_body + offset, PAGE_BODY_SIZE - offset);
    set_pointer_offset_file(cursor->file, next_header->block_number * PAGE_SIZE + PAGE_HEADER_SIZE - UINT32_T_SIZE);
    uint32_t off = next_header->offset - offset;
    write_to_file(cursor->file, &(off), UINT32_T_SIZE);
    write_to_file(cursor->file, &(off), UINT32_T_SIZE);
    cut_blocks(cursor, page->page_header, &(pointer), table);
}

void delete_element(
        Cursor* cursor, void* element, 
        uint64_t (*get_size_of_element)(void *), 
        uint64_t size_of_sturcture, void* name_type, TypeOfElement type, 
        bool (*comparator)(void*, void*),
        void* (*read_big_element)(Cursor*, Entity*, PageHeader*, void*, uint64_t*, char*, uint32_t*)
    ) {
    uint64_t pointer = 0;
    uint64_t offset = 0;

    Entity* entity = (Entity*) malloc(ENTITY_SIZE);
    Page* page = (Page*) malloc(sizeof(Page));
    PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    
    char* new_body = (char*) calloc(1, PAGE_BODY_SIZE);
    void* body = malloc(PAGE_BODY_SIZE);

    entity = get_entity(cursor, type, name_type, &(pointer), entity);
    if (entity == NULL) {
        println("Error to find Node with name %s", name_type);
        return;
    }

    page->page_header = page_header;
    page->body = body;
    //FIXME: offset - начало элемента
    void* find_el = find_element(cursor, entity, page, size_of_sturcture, element, &(offset),  comparator, read_big_element);
    
    while (find_el != NULL) {
        if (get_size_of_element(element) <= PAGE_BODY_SIZE) {
            memcpy(new_body, page->body, offset);
            memcpy(new_body + offset, page->body + offset + get_size_of_element(find_el), PAGE_BODY_SIZE - offset - get_size_of_element(find_el));

            uint64_t new_offset = page->page_header->offset - get_size_of_element(find_el);
            
            set_pointer_offset_file(cursor->file, page->page_header->block_number * PAGE_SIZE + PAGE_HEADER_SIZE - UINT32_T_SIZE);
            write_to_file(cursor->file, &(new_offset), UINT32_T_SIZE);
            write_to_file(cursor->file, new_body, PAGE_BODY_SIZE);
            
            if (new_offset == 0) {
                cut_blocks(cursor, page->page_header, &(pointer), entity);
            }
        } else {
            //offset по идеи 0
            // нужно просто удалить все блоки

            // memcpy(new_body, page->body, offset);
            // memcpy(new_body + offset, page->body + offset + get_size_of_element(find_el), PAGE_BODY_SIZE - offset - get_size_of_element(find_el));

            //нужно сделать стек из массива и циклического указателя. Пройти по блокам после удаленных и скопировать их повыше, а пото просто удалить лишние
            set_pointer_offset_file(cursor->file, page->page_header->block_number * PAGE_SIZE + PAGE_HEADER_SIZE - UINT32_T_SIZE);
            write_to_file(cursor->file, &(offset), UINT32_T_SIZE);
            write_to_file(cursor->file, new_body, PAGE_BODY_SIZE);
            set_pointer_offset_file(cursor->file, page->page_header->next_block * PAGE_SIZE + PAGE_HEADER_SIZE - UINT32_T_SIZE);
            
            uint32_t zero = 0;
            write_to_file(cursor->file, &(zero), UINT32_T_SIZE);
            cut_blocks(cursor, page->page_header, &(pointer), entity);
        }
        free(find_el);
        find_el = find_element(cursor, entity, page, size_of_sturcture, element, &(offset),  comparator, read_big_element);

    }

    free(page->page_header);
    free(page->body);
    free(page);
    free(entity);
    free(new_body);
}

// uint32_t* create_big_element(
//     Cursor* cursor, void* element, 
//         void (*write_to_file_element)(Cursor*, void*), const char* name_type, 
//         TypeOfElement type, uint64_t (*get_size_of_element)(void *), 
//         void (*set_id)(void*, uint32_t)
// ) {
//     (void) write_to_file_element;
//     (void) get_size_of_element;

//     Node* nd = (Node*) element;
    
//     uint64_t pointer = 0;
//     Entity* table = (Entity*) malloc(ENTITY_SIZE);
//     table = get_entity(cursor, type, name_type, &(pointer), table);

//     if (table == NULL) {
//         println("Error to find Node with name %s", name_type);
//         return NULL;
//     }

//     uint32_t page_num = table->last_block;
//     uint64_t page_offset = page_num * PAGE_SIZE;

//     PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);

//     set_pointer_offset_file(cursor->file, page_offset);
//     read_from_file(cursor->file, page_header, PAGE_HEADER_SIZE);

//     uint64_t global_offset = page_offset + page_header->offset + PAGE_HEADER_SIZE;
//     // until now the same
    
//     // difference condition
//     if (page_header->offset + UINT32_T_SIZE * 2 > PAGE_BODY_SIZE) {
//         create_new_page(cursor, page_header, page_offset);
        
//         table->last_block = cursor->number_of_pages;
//         global_offset = cursor->number_of_pages * PAGE_SIZE + PAGE_HEADER_SIZE;
//     }

//     set_id(element, table->next_id);
//     table->next_id += 1;

//     set_pointer_offset_file(cursor->file, global_offset);
//     //записываю разные данные
//     write_uint_32_to_file(cursor, nd->id);
//     write_uint_32_to_file(cursor, nd->name_length);

//     uint32_t name_length = nd->name_length;
//     uint32_t string_offset = 0;

//     page_header->offset += UINT32_T_SIZE * 2;
//     char* string_to_write = (char*) malloc(PAGE_BODY_SIZE);
    
//     while (name_length != 0) {
//         if (page_header->offset == PAGE_BODY_SIZE) {
//             create_new_page(cursor, page_header, page_offset);
        
//             table->last_block = cursor->number_of_pages;
//             page_offset = table->last_block * PAGE_SIZE;
//             global_offset = page_offset + PAGE_HEADER_SIZE;

//             set_pointer_offset_file(cursor->file, global_offset);
//         }
//         uint32_t write_length = 0;
//         if (name_length <= PAGE_BODY_SIZE - page_header->offset) {
//             write_length = name_length;
//         } else {
//             write_length = PAGE_BODY_SIZE - page_header->offset;
//             debug(102, "%llu %llu", write_length, PAGE_BODY_SIZE);
//         }
//         memcpy(string_to_write, nd->name + string_offset, write_length);
//         //обнвить page_header offset and table last block
//         write_string_to_file(cursor, string_to_write, write_length);

//         name_length -= write_length;
//         string_offset += write_length;
//         page_header->offset += write_length;
        
//         uint64_t header_offset = page_offset + UINT32_T_SIZE * 2;
//         //записал отступ для блока
//         set_pointer_offset_file(cursor->file, header_offset);
//         write_to_file(cursor->file, &(page_header->offset), UINT32_T_SIZE);
//         set_pointer_offset_file(cursor->file, pointer + TYPE_OF_ELEMENT_SIZE + UINT32_T_SIZE);
//         //записал данные для таблицы
//         write_to_file(cursor->file, &(table->last_block), UINT32_T_SIZE);
//         write_to_file(cursor->file, &(table->next_id), sizeof(uint32_t));
//     }

//     free(table);
//     free(page_header);
//     uint32_t* return_id = malloc(UINT32_T_SIZE);
//     *return_id = table->next_id - 1;
//     return return_id;
// }

// uint32_t* create_element(
//         Cursor* cursor, void* element, 
//         void (*write_to_file_element)(Cursor*, void*), const char* name_type, 
//         TypeOfElement type, uint64_t (*get_size_of_element)(void *), 
//         void (*set_id)(void*, uint32_t)
//     ) {

//     if (get_size_of_element(element) > PAGE_BODY_SIZE) {
//         println("Element is too big");
//         return NULL;
//     }
    
//     uint64_t pointer = 0;
//     Entity* table = (Entity*) malloc(ENTITY_SIZE);
//     // читаю данные о таблице
//     table = get_entity(cursor, type, name_type, &(pointer), table);

//     if (table == NULL) {
//         println("Error to find Node with name %s", name_type);
//         return NULL;
//     }
//     //из данных по таблице получаю адрес нужного блока
//     uint32_t page_num = table->last_block;
//     uint64_t page_offset = page_num * PAGE_SIZE;

//     PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);

//     set_pointer_offset_file(cursor->file, page_offset);
//     // получаю заголовок
//     read_from_file(cursor->file, page_header, PAGE_HEADER_SIZE);
//     // получаю адрес куда можно записать
//     uint64_t global_offset = page_offset + page_header->offset + PAGE_HEADER_SIZE;
//     //создаю новый блок, если недостаточно места
//     if (page_header->offset + get_size_of_element(element) > PAGE_BODY_SIZE) {
//         create_new_page(cursor, page_header, page_offset);
        
//         table->last_block = cursor->number_of_pages;
//         global_offset = cursor->number_of_pages * PAGE_SIZE + PAGE_HEADER_SIZE;
//     }

//     set_id(element, table->next_id);
//     table->next_id += 1;
//     //записа элемент
//     set_pointer_offset_file(cursor->file, global_offset);
//     write_to_file_element(cursor, element);

//     uint64_t header_offset = global_offset - page_header->offset - PAGE_HEADER_SIZE + UINT32_T_SIZE * 2;
//     page_header->offset += get_size_of_element(element);
//     //записал отступ для блока
//     set_pointer_offset_file(cursor->file, header_offset);
//     write_to_file(cursor->file, &(page_header->offset), UINT32_T_SIZE);

//     set_pointer_offset_file(cursor->file, pointer + TYPE_OF_ELEMENT_SIZE + UINT32_T_SIZE);
//     //записал данные для таблицы
//     write_to_file(cursor->file, &(table->last_block), UINT32_T_SIZE);
//     write_to_file(cursor->file, &(table->next_id), sizeof(uint32_t));

//     free(table);
//     free(page_header);
//     uint32_t* return_id = malloc(UINT32_T_SIZE);
//     *return_id = table->next_id - 1;
//     return return_id;
// }

// void* find_element(
//         Cursor* cursor, Entity* meta_page, 
//         Page* page, uint64_t size_of_element, 
//         void* find_elem, void* (*read_element)(void*, Entity*, char*, uint64_t*), 
//         bool (*comparator)(void*, void*), uint64_t* offset_
//     ) {
//     uint32_t read_block = meta_page->first_block;
//     PageHeader* page_header = page->page_header;
//     void* body = page->body;
//     void* element = (void*) malloc(size_of_element);

//     while (read_block != 0) {
//         uint64_t offset = 0;
//         uint64_t page_offset = read_block * PAGE_SIZE;
        
//         set_pointer_offset_file(cursor->file, page_offset);
//         read_from_file(cursor->file, page_header, PAGE_HEADER_SIZE);  
//         read_from_file(cursor->file, body, PAGE_BODY_SIZE);
//         while (offset < page_header->offset) {
//             element = read_element(element, meta_page, body, &(offset));
//             if (comparator(element, find_elem)) {
//                 page->body = body;
//                 page->page_header = page_header;
//                 *offset_ = offset;
//                 return element;
//             }
//         }
//         read_block = page_header->next_block;
//     }
//     free(element);
//     return NULL;
// }


// void* read_big_node(void* element, Entity* metaPage, char* body, uint64_t* offset) {
//     Node* node = (Node*) element;
//     strcpy(node->name_type, metaPage->name_type);

//     uint32_t name_length = 0;
//     uint32_t id = 0;
    
//     memcpy(&(id), body + *offset, UINT32_T_SIZE);
//     *offset += UINT32_T_SIZE;
//     memcpy(&(name_length), body + *offset, UINT32_T_SIZE);
//     *offset += UINT32_T_SIZE;
    
//     char* name = malloc(CHAR_SIZE * name_length);


//     memcpy(name, body + *offset, CHAR_SIZE * name_length);
//     *offset += CHAR_SIZE * name_length;
    
//     node->id = id;
//     node->name_length = name_length;
//     node->name = name;
    
//     return node;
// }

// void* get_elements_by_condition(
//         Cursor* cursor, Entity* meta_page, 
//         uint64_t size_of_element, 
//         void* helper, void* (*read_element)(void*, Entity*, char*, uint64_t*), 
//         bool (*condition)(void*, void*), uint64_t (*get_size_of_element)(void*),
//         void (*write_element)(void*, Entity*, char*, uint64_t*)
//     ) {
//     uint32_t read_block = meta_page->first_block;
//     uint64_t stack_offset = UINT32_T_SIZE;
//     uint32_t find_number = 0;

//     PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);
//     uint64_t stack_size = PAGE_BODY_SIZE;
//     void* body = malloc(PAGE_BODY_SIZE);
//     void* stack = malloc(PAGE_BODY_SIZE);
//     void* element = (void*) malloc(size_of_element);

//     while (read_block != 0) {
//         uint64_t offset = 0;
//         uint64_t page_offset = read_block * PAGE_SIZE;

//         set_pointer_offset_file(cursor->file, page_offset);
//         read_from_file(cursor->file, page_header, PAGE_HEADER_SIZE);  
//         read_from_file(cursor->file, body, PAGE_BODY_SIZE);

//         while (offset < page_header->offset) {
//             element = read_element(element, meta_page, body, &(offset));
//             if (condition(element, helper)) {
//                 find_number++;
//                 // uint64_t size = get_size_of_element(element);

//                 if (stack_offset + get_size_of_element(element) >= stack_size) {
//                     stack_size += PAGE_BODY_SIZE;
//                     stack = realloc(stack, stack_size);
//                 }
//                 write_element(element, meta_page, stack, &(stack_offset));
//             }
//         }
//         read_block = page_header->next_block;
//     }
//     memcpy(stack, &(find_number), UINT32_T_SIZE);

//     free(element);
//     free(page_header);
//     free(body);
//     return stack;
// }

// void delete_element(
//         Cursor* cursor, void* element, 
//         uint64_t (*get_size_of_element)(void *), 
//         uint64_t size_of_sturcture, void* name_type, TypeOfElement type, 
//         void* (*read_element)(void*, Entity*, char*, uint64_t*), 
//         bool (*comparator)(void*, void*)
//     ) {
//     uint64_t pointer = 0;
//     uint64_t offset = 0;

//     Entity* table = (Entity*) malloc(ENTITY_SIZE);
//     table = get_entity(cursor, type, name_type, &(pointer), table);
//     if (table == NULL) {
//         println("Error to find Node with name %s", name_type);
//         return;
//     }

//     char* new_body = (char*) malloc(PAGE_BODY_SIZE);
//     Page* page = (Page*) malloc(sizeof(Page));
//     PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);
//     void* body = malloc(PAGE_BODY_SIZE);
//     page->page_header = page_header;
//     page->body = body;
//     void* find_el = find_element(cursor, table, page, size_of_sturcture, element, read_element, comparator, &(offset));
//     while (find_el != NULL) {
//         if (find_el == NULL) {
//             println("Can't find element %s", name_type);
//             return;
//         }
//         offset -= get_size_of_element(find_el);

//         memcpy(new_body, page->body, offset);
//         memcpy(new_body + offset, page->body + offset + get_size_of_element(find_el), PAGE_BODY_SIZE - offset - get_size_of_element(find_el));

//         uint64_t new_offset = page->page_header->offset - get_size_of_element(find_el);
        
//         set_pointer_offset_file(cursor->file, page->page_header->block_number * PAGE_SIZE + PAGE_HEADER_SIZE - UINT32_T_SIZE);
//         write_to_file(cursor->file, &(new_offset), UINT32_T_SIZE);
//         write_to_file(cursor->file, new_body, PAGE_BODY_SIZE);
        
//         if (new_offset == 0) {
//             cut_blocks(cursor, page->page_header, &(pointer), table);
//         }
//         free(find_el);
//         find_el = find_element(cursor, table, page, size_of_sturcture, element, read_element, comparator, &(offset));
//     }

//     free(page->page_header);
//     free(page->body);
//     free(page);
//     free(new_body);
// }

void update_element(
        Cursor* cursor, void* old_element, 
        void* new_element, uint64_t (*get_size_of_element)(void *), 
        uint64_t size_of_sturcture, void *name_type, TypeOfElement type, 
        void *(*read_element)(void *, Entity *, char*, uint64_t *), 
        bool (*comparator)(void*, void*), 
        void (*write_to_file_element)(Cursor *, void*), 
        void (*set_id)(void*, uint32_t)
    ) {
    delete_element(cursor, old_element, get_size_of_element, size_of_sturcture, name_type, type, read_element, comparator);
    create_element(cursor, new_element, write_to_file_element, name_type, type, get_size_of_element, set_id);
}
