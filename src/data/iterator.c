#include "../../include/data/iterator.h"

#include "data/constants.h"
#include "data/page.h"
#include "managers/page_manager.h"
#include "utils/checker.h"
#include "utils/io_utils.h"
#include "utils/logger.h"

#include <string.h>
////FIXME: good
void *next(const Iterator *iterator) {
    return iterator->element;
}

/* 
    offset_ указывает на следующие элемент
    read_block указывает на блок, в котором находится следующий элемент
*/
bool has_next(Iterator *iterator) {
    LOG_DEBUG("In has_next", "");
    if (check_is_null_arg(iterator, "iterator")) {
        return false;
    }
    uint64_t offset = *(iterator->offset_);
    uint32_t read_block = *(iterator->read_block_);

    const Cursor *cursor = iterator->cursor;

    while (read_block != 0) {
        Page* page = read_page_from_file(cursor, read_block);
        PageHeader *page_header = page->page_header;

        while (offset < page_header->offset) {
            void *element = iterator->function_helper->read_element_from_file(cursor, page, &(offset));

            if (iterator->function_helper->condition(element, iterator->helper)) {

                *(iterator->offset_) = (offset + iterator->function_helper->get_size_of_element(element)) % PAGE_BODY_SIZE;
                *(iterator->read_block_) = page->page_header->block_number;
                iterator->element = element;
                
                free_page(page);
                return true;
            }
            offset = offset + iterator->function_helper->get_size_of_element(element) % PAGE_BODY_SIZE;
            iterator->function_helper->free_element(element);
        }
        read_block = page_header->next_block;
        free_page(page);
        offset = 0;
    }
    return false;
}

void *entity_next(const EntityIterator *entity_iterator) {
        return entity_iterator->iterator->element;
}

/* 
    offset_ указывает на следующие элемент
    entity_iterator->page указывает на блок, в котором находится следующий элемент
*/
bool entity_has_next(EntityIterator *entity_iterator) {
    LOG_DEBUG("In entity_has_next", "");
    if (check_is_null_arg(entity_iterator, "entity_iterator")) {
        return false;
    }
    
    Iterator *iterator = entity_iterator->iterator;
    bool has_next_element = has_next(iterator);
    if (has_next_element) return true;
    
    const Cursor *cursor = entity_iterator->cursor;
    Entity *old_entity = entity_iterator->entity;
    Entity *new_entity = (Entity*) malloc(ENTITY_SIZE);
    uint32_t page_num = 0;

    do {
        Page *entity_page = entity_iterator->page;

        for (uint64_t i = *(entity_iterator->offset_) / ENTITY_SIZE; i < (entity_page->page_header->offset / ENTITY_SIZE); i++) {
            memcpy(new_entity, entity_page->page_body + i * ENTITY_SIZE, ENTITY_SIZE);
            
            if (new_entity->element_type == old_entity->element_type) {

                *(entity_iterator->offset_) = (i + 1) * ENTITY_SIZE;
                entity_iterator->entity = new_entity;

                *(iterator->read_block_) = new_entity->first_page;
                *(iterator->offset_) = 0;
                iterator->entity = new_entity;
                
                return entity_has_next(entity_iterator);
            }
        }
        page_num = entity_page->page_header->next_block;
        *(entity_iterator->offset_) = 0;

        free_page(entity_page);
        entity_iterator->page = read_page_from_file(cursor, page_num);
    } while (page_num != 0);

    return false;
}

void free_iter(Iterator *iterator) {
    if (check_is_null_arg(iterator, "iterator")) return ;
    free(iterator->entity);
    free(iterator->read_block_);
    free(iterator->offset_);
    free(iterator->element);
    free(iterator);
}

void free_entity_iter(EntityIterator *entity_iterator) {
    if (check_is_null_arg(entity_iterator, "entity_iterator")) return ;
    free_iter(entity_iterator->iterator);
    free_page(entity_iterator->page);
    free(entity_iterator->offset_);
    free(entity_iterator);
}

EntityIterator *get_entity_iterator(Cursor *cursor, TypeOfElement element_type) {

    Entity *entity = malloc(ENTITY_SIZE);
    uint64_t *offset_ = malloc(UINT64_T_SIZE);
    EntityIterator *entity_iterator = (EntityIterator*) malloc(EMTITY_ITERATOR_SIZE);
    
    int page_num = START_PAGE;

    do {
        Page *page = read_page_from_file(cursor, page_num);

        for (uint64_t i = 0; i < (page->page_header->offset / ENTITY_SIZE); i++) {
            memcpy(entity, page->page_body + i * ENTITY_SIZE, ENTITY_SIZE);
            if (element_type == entity->element_type) {

               *offset_ = (i + 1) * ENTITY_SIZE;

                entity_iterator->cursor = cursor;
                entity_iterator->entity = entity;
                entity_iterator->page = page;
                entity_iterator->offset_ = offset_;
                
                return entity_iterator;
            }
        }
        page_num = page->page_header->next_block;
        free_page(page);
    } while (page_num != 0);
    
    free(entity);

    free(offset_);
    free(entity_iterator);

    return NULL;
}
