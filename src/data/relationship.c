#include "../../include/data/relationship.h"

#include "../../include/utils/io_utils.h"
#include "utils/checker.h"
#include "utils/logger.h"

#include <string.h>

void print_relationship(const Relationship *relationship) {
    if (check_is_null_arg(relationship, "relationship")) return;
    println("Print relationship");
    println("Id - %llu", relationship->id);
    println("Parent - %llu", relationship->parent_id);
    println("Child - %llu", relationship->child_id);
    println("Type - %s", relationship->type);
    println("Parent Type - %s", relationship->parent_type);
    println("Child Type - %s", relationship->child_type);
}

uint64_t get_size_of_relationship(const void *rl) {
    (void) rl;
    return RELATIONSHIP_SIZE;
}

/*
    записывает объект в файл и обновляет offset на страницах
    page->page_header хранит информацию о последнем записанном блоке
*/
void write_relationship_to_file(Cursor *cursor, Page *page, Entity *entity, const void *rl, const uint32_t *id) {
    LOG_DEBUG("In write_relationship_to_file", "");
    if (check_is_null_arg(cursor, "cursor") || 
        check_is_null_arg(page, "page") ||
        check_is_null_arg(cursor->file, "cursor->file") ||
        check_is_null_arg(entity, "entity")) {
        return;
    }

    (void) entity;
    Relationship *relationship = (Relationship*) rl;

    if (id == NULL) {
        relationship->id = entity->next_id;
        entity->next_id++;
    } else {
        relationship->id = *id;
    }

    write_uint_32_to_file(cursor->file, relationship->id);
    write_uint_32_to_file(cursor->file, relationship->parent_id);
    write_uint_32_to_file(cursor->file, relationship->child_id);
    write_string_to_file(cursor->file, relationship->type, NAME_TYPE_WITH_TERM_LENGTH);
    write_string_to_file(cursor->file, relationship->parent_type, NAME_TYPE_WITH_TERM_LENGTH);
    write_string_to_file(cursor->file, relationship->child_type, NAME_TYPE_WITH_TERM_LENGTH);
    
    page->page_header->offset += RELATIONSHIP_SIZE;

    update_page_header_offset(cursor, page);
}

/*
    page хранит полностью последний прочитанный блок
*/
void *read_relationship_from_file(const Cursor *cursor, Page *page, const uint64_t *offset_) {
    LOG_DEBUG("In read_relationship_from_file", "");
    if (check_is_null_arg(cursor, "cursor") || 
        check_is_null_arg(page, "page") ||
        check_is_null_arg(cursor->file, "cursor->file") ||
        check_is_null_arg(offset_, "offset_")) {
        return NULL;
    }

    (void) cursor;
    
    char *body = page->page_body;
    Relationship *relationship = (Relationship*) malloc(RELATIONSHIP_SIZE);
    uint64_t offset = *offset_;
    
    memcpy(&(relationship->id), body + offset, UINT32_T_SIZE);
    offset += UINT32_T_SIZE;

    memcpy(&(relationship->parent_id), body + offset, UINT32_T_SIZE);
    offset += UINT32_T_SIZE;

    memcpy(&(relationship->child_id), body + offset, UINT32_T_SIZE);
    offset += UINT32_T_SIZE;
    
    memcpy(relationship->type, body + offset, NAME_TYPE_SIZE);
    offset += NAME_TYPE_SIZE;
    
    memcpy(relationship->parent_type, body + offset, NAME_TYPE_SIZE);
    offset += NAME_TYPE_SIZE;
    
    memcpy(relationship->child_type, body + offset, NAME_TYPE_SIZE);
    offset += NAME_TYPE_SIZE;
    
    return relationship;
}

bool compare_relationship(const void *rl_1, const void *rl_2) {
    Relationship *relationship_1 = (Relationship*) rl_1;
    Relationship *relationship_2 = (Relationship*) rl_2;
    return relationship_1->parent_id == relationship_2->parent_id && 
        relationship_1->id == relationship_2->id && relationship_1->child_id == relationship_2->id
        && strcmp(relationship_1->type, relationship_2->type) == 0;
}

bool compare_parent_relationship(const void *rl_1, const void *rl_2) {
    Relationship *relationship_1 = (Relationship*) rl_1;
    Relationship *relationship_2 = (Relationship*) rl_2;
    return relationship_1->parent_id == relationship_2->parent_id &&
        strcmp(relationship_1->parent_type, relationship_2->parent_type) == 0;
}

bool compare_relationship_by_node(const void *rl_1, const void *rl_2) {
    Relationship *relationship_1 = (Relationship*) rl_1;
    Relationship *relationship_2 = (Relationship*) rl_2;
    return (strcmp(relationship_1->parent_type, relationship_2->parent_type) == 0 &&
        relationship_1->parent_id == relationship_2->parent_id) ||
        (strcmp(relationship_1->child_type, relationship_2->child_type) == 0 &&
        relationship_1->child_id == relationship_2->child_id);
}

bool compare_child_relationship(const void *rl_1, const void *rl_2) {
    Relationship *relationship_1 = (Relationship*) rl_1;
    Relationship *relationship_2 = (Relationship*) rl_2;
    return relationship_1->child_id == relationship_2->child_id &&
        strcmp(relationship_1->child_type, relationship_2->child_type) == 0;
}

bool compare_id_relationship(const void *rl_1, const void *rl_2) {
    Relationship *relationship_1 = (Relationship*) rl_1;
    Relationship *relationship_2 = (Relationship*) rl_2;
    return relationship_1->id == relationship_2->id &&
        strcmp(relationship_1->type, relationship_2->type) == 0;
}

void memcpy_relationship(const void *element, char *stack, uint64_t *offset) {
    LOG_DEBUG("In memecpy_relationship", "");
    if (check_is_null_arg(element, "element") || 
        check_is_null_arg(stack, "stack") ||
        check_is_null_arg(offset, "offset")) {
        return;
    }    
    Relationship *relationship = (Relationship*) element;
    
    memcpy(stack + *offset, &(relationship->id), UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(stack + *offset, &(relationship->parent_id), UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;
    
    memcpy(stack + *offset, &(relationship->child_id), UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;
    
    memcpy(stack + *offset, relationship->type, NAME_TYPE_SIZE);
    *offset += NAME_TYPE_SIZE;
    
    memcpy(stack + *offset, relationship->parent_type, NAME_TYPE_SIZE);
    *offset += NAME_TYPE_SIZE;
    
    memcpy(stack + *offset, relationship->child_type, NAME_TYPE_SIZE);
    *offset += NAME_TYPE_SIZE;
}

void *memget_relationship(const char *stack, uint64_t *offset) {
    LOG_DEBUG("In memeget_property", "");
    if (check_is_null_arg(stack, "stack") ||
        check_is_null_arg(offset, "offset")) {
        return NULL;
    }
    Relationship *relationship = (Relationship*) malloc(RELATIONSHIP_SIZE);

    memcpy(&(relationship->id), stack + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(&(relationship->parent_id), stack + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(&(relationship->child_id), stack + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(relationship->type, stack + *offset, NAME_TYPE_SIZE);
    *offset += NAME_TYPE_SIZE;
    
    memcpy(relationship->parent_type, stack + *offset, NAME_TYPE_SIZE);
    *offset += NAME_TYPE_SIZE;
    
    memcpy(relationship->child_type, stack + *offset, NAME_TYPE_SIZE);
    *offset += NAME_TYPE_SIZE;

    return relationship;
}

uint32_t get_relationship_id(const void *rl) {
    Relationship *relationship = (Relationship *) rl;
    return relationship->id;
}

void free_relationship(void *rl) {
    Relationship *relationship = (Relationship *) rl;
    free(relationship);
}
