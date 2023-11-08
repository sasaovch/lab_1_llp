#include "../../include/data/entity.h"

#include "../../include/utils/io_utils.h"
#include "../../include/utils/page_utils.h"
#include "../../include/utils/stack_utils.h"
#include "../../include/utils/checker.h"
#include "data/constants.h"
#include "managers/file_manager.h"
#include "managers/page_manager.h"
#include "utils/logger.h"

#include <stdint.h>
#include <string.h>
//FIXME: good

void print_entity(const Entity *entity) {
    if (check_is_null_arg(entity, "entity")) {
        return;
    }
    println("Print");
    println("Type - %i", entity->type);
    println("First - %i", entity->first_page);
    println("Last - %i", entity->last_page);
    println("Next Id - %i", entity->next_id);
    println("Name - %s", entity->type);
}

/*
    pointer - указывает на начало найденного элемента
*/
Entity *get_entity(const Cursor *cursor, TypeOfElement element_type, const char *name, uint64_t *pointer) {
    LOG_DEBUG("In get_entity", "");
    if (check_is_null_arg(cursor, "cursor") || 
        check_is_null_arg(name, "name") ||
        check_is_null_arg(cursor->file, "cursor->file") ||
        check_is_null_arg(pointer, "pointer")) {
        return NULL;
    }
    
    Entity *entity = (Entity*) malloc(ENTITY_SIZE);
    int page_num = START_PAGE;

    do {
        Page *page = read_page_from_file(cursor, page_num);
        if (check_is_null_returned(page, "read_page_from_file")) return NULL;
        PageHeader *page_header = page->page_header;
        char* page_body = page->page_body;

        for (uint64_t i = 0; i < (page_header->offset / ENTITY_SIZE); i++) {
            memcpy(entity, page_body + i * ENTITY_SIZE, ENTITY_SIZE);
            
            if (strcmp(entity->type, name) == 0 && element_type == entity->element_type) {
                *pointer = page_num * BLOCK_SIZE + i * ENTITY_SIZE + PAGE_HEADER_SIZE;
                free_page(page);
                return entity;
            }
        }
        page_num = page_header->next_block;
        free_page(page);
    } while (page_num != 0);
    
    free(entity);
    return NULL;
}

Entity *create_entity(Cursor *cursor, const Entity *new_entity) {
    LOG_DEBUG("In create_entity", "");
    if (check_is_null_arg(cursor, "cursor") ||
        check_is_null_arg(new_entity, "new_entity")) {

        return NULL;
    }

    uint64_t pointer = 0;
    Entity *old_entity = get_entity(cursor, new_entity->element_type, new_entity->type, &(pointer));

    if (old_entity != NULL) {
        LOG_INFO("Entity %s with type %i already exists", new_entity->type, new_entity->element_type);
        return NULL;
    }

    uint32_t last_entity_page = find_last_page(cursor, START_PAGE);
    if (last_entity_page < START_PAGE) last_entity_page = START_PAGE;
    
    Page *page = read_page_from_file(cursor, last_entity_page);
    if (page->page_header->block_number < last_entity_page) page->page_header->block_number = last_entity_page; 

    Page *n_page = page;
    if (page->page_header->offset + ENTITY_SIZE > PAGE_BODY_SIZE) {
        n_page = get_free_page(cursor, page);
        free_page(page);
    }

    Page *objects_page = get_free_page(cursor, NULL);
    Entity *entity = (Entity*) malloc(ENTITY_SIZE);
    
    entity->next_id = 0;
    entity->element_type = new_entity->element_type;
    entity->first_page = objects_page->page_header->block_number;
    entity->last_page = objects_page->page_header->block_number;
    entity->value_type = new_entity->value_type;

    memcpy(entity->type, new_entity->type, NAME_TYPE_SIZE);
    memcpy(n_page->page_body + n_page->page_header->offset, entity, ENTITY_SIZE);
    
    n_page->page_header->offset += ENTITY_SIZE;
    write_page_to_file_flush(cursor, n_page);

    flush(cursor->file);

    free(entity);
    free(old_entity);
    free_page(n_page);
    free_page(objects_page);

    return entity;
}

void erase_entity(Cursor *cursor, const uint64_t *pointer) {
    LOG_DEBUG("In erase_entity", "");
    if (check_is_null_arg(cursor, "cursor") ||
        check_is_null_arg(pointer, "pointer")) {

        return;
    }

    uint64_t page_number = *pointer / BLOCK_SIZE;
    uint64_t offset = *pointer % BLOCK_SIZE - PAGE_HEADER_SIZE;
    
    Page *page = read_page_from_file(cursor, page_number);
    char *new_body = (char*) malloc(PAGE_BODY_SIZE);

    memcpy(new_body, page->page_body, offset);
    memcpy(new_body + offset, page->page_body + offset + ENTITY_SIZE, PAGE_BODY_SIZE - offset - ENTITY_SIZE);

    free(page->page_body);
    page->page_body = new_body;
    page->page_header->offset -= ENTITY_SIZE;
    uint32_t new_offset = page->page_header->offset;

    write_page_to_file_flush(cursor, page);

    if (new_offset == 0) {
        uint32_t delete_page_number = page->page_header->block_number;
        uint32_t prev_page_number = find_page_before(cursor, delete_page_number, START_PAGE);
        uint32_t next_page_number = page->page_header->next_block;

        if (delete_page_number != START_PAGE) {
            set_pointer_offset_file(cursor->file, prev_page_number * BLOCK_SIZE + UINT32_T_SIZE);
            write_uint_32_to_file(cursor->file, next_page_number);
            
            push_in_stack(cursor, delete_page_number);
        
            set_pointer_offset_file(cursor->file, delete_page_number * BLOCK_SIZE + UINT32_T_SIZE);
            write_uint_32_to_file(cursor->file, 0);
        }
    }

    free_page(page);
}

bool delete_entity(Cursor *cursor, const Entity *entity) {
    LOG_DEBUG("In delete_entity", "");
    if (check_is_null_arg(cursor, "cursor") ||
        check_is_null_arg(entity, "entity")) {

        return false;
    }

    uint64_t *pointer = (uint64_t*) malloc(UINT64_T_SIZE);
    *pointer = 0;

    Entity *delete_entity = get_entity(cursor, entity->element_type, entity->type, pointer);
    if (check_is_null_returned(delete_entity, "get_entity")) {
        LOG_INFO("Entity %s with type %i doesn't exists", entity->type, entity->element_type);
        return false;
    }

    Stack *stack = find_all_pages_to_delete(cursor, delete_entity);
    remove_pages(cursor, stack);
    erase_entity(cursor, pointer);

    flush(cursor->file);

    free(pointer);
    free(stack);
    free(delete_entity);
    return true;
}
