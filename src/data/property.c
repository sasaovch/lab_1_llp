#include "../../include/data/property.h"

#include "../../include/utils/io_utils.h"
#include "../../include/utils/page_utils.h"
#include "utils/checker.h"
#include "utils/logger.h"

#include <string.h>

void print_property(const Property *property) {
    if (check_is_null_arg(property, "property")) return;
    println("Print property");
    println("Value type - %i", property->value_type);
    println("Subject id - %llu", property->subject_id);
    println("Value length - %i", property->value_length);
    println("Type - %s", property->type);
    println("Subject type - %s", property->subject_type);

    switch (property->value_type) {
        case INT: 
            println("Value - %i", *((uint32_t*) property->value));
            break;
        case BOOL: 
            println("Value - %i", property->value);
            break;
        case FLOAT: 
            println("Value - %f", *((float*)property->value));
            break;
        case STRING: 
            println("Value - %s", property->value);
            break;
        case VOID: 
            println("Value - %s", property->value);
            break;
    }
}

/*
    записывает объект в файл и обновляет offset на страницах
    page->page_header хранит информацию о последнем записанном блоке
*/
void write_property_to_file(Cursor *cursor, Page *page, Entity *entity, const void *pr, const uint32_t *id) {
    LOG_DEBUG("In write_property_to_file", "");
    if (check_is_null_arg(cursor, "cursor") || 
        check_is_null_arg(page, "page") ||
        check_is_null_arg(cursor->file, "cursor->file") ||
        check_is_null_arg(entity, "entity")) {
        return;
    }
    (void) id;
    Property *property = (Property*) pr;

    write_uint_32_to_file(cursor->file, property->value_type);
    write_uint_32_to_file(cursor->file, property->subject_id);
    write_uint_32_to_file(cursor->file, property->value_length);
    write_string_to_file(cursor->file, property->type, NAME_TYPE_WITH_TERM_LENGTH);
    write_string_to_file(cursor->file, property->subject_type, NAME_TYPE_WITH_TERM_LENGTH);
    
    page->page_header->offset += VALUE_TYPE_SIZE + UINT32_T_SIZE * 2 + NAME_TYPE_SIZE * 2;

    switch (property->value_type) {
        case INT: {
            write_uint_32_to_file(cursor->file, *((uint32_t*)(property->value)));
            page->page_header->offset += UINT32_T_SIZE;
            update_page_header_offset(cursor, page);
            break;
        }
        case BOOL: {
            write_to_file(cursor->file, property->value, sizeof(bool));
            page->page_header->offset += sizeof(bool);
            update_page_header_offset(cursor, page);
            break;
        }
        case FLOAT: {
            write_to_file(cursor->file, property->value, sizeof(float));
            page->page_header->offset += sizeof(float);
            update_page_header_offset(cursor, page);
            break;
        }
        case STRING: 
            write_big_string_to_file(cursor, page, entity, property->value_length, property->value);
            break;
        case VOID: 
            break;
    }
}


void read_value_from_file(
        const Cursor *cursor, Page *page,
        Property *property, uint32_t value_length,
        const char *body, uint64_t offset
    ) {
    switch (property->value_type) {
        case INT: {
            uint32_t *value = malloc(UINT32_T_SIZE);
            memcpy(value, body + offset, UINT32_T_SIZE);
            property->value = value;
            break;
        }
        case BOOL: {
            bool *value = malloc(sizeof(bool));
            memcpy(value, body + offset, sizeof(bool));
            property->value = value;
        }
        case FLOAT: {
            float *value = malloc(sizeof(float));
            memcpy(value, body + offset, sizeof(float));
            property->value = value;
            break;
        }
        case STRING: {
            char *value = malloc(CHAR_SIZE * value_length);
            read_big_string_from_file(cursor, page, value, value_length, &(offset));
            property->value = value;
            property->value_length = value_length;
            break;
        }
        case VOID: 
            break;
    }
}

/*
    page хранит полностью последний прочитанный блок
*/
void *read_property_from_file(const Cursor *cursor, Page *page, const uint64_t *offset_) {
    LOG_DEBUG("In read_property_from_file", "");
    if (check_is_null_arg(cursor, "cursor") || 
        check_is_null_arg(page, "page") ||
        check_is_null_arg(cursor->file, "cursor->file") ||
        check_is_null_arg(offset_, "offset_")) {
        return NULL;
    }
    Property *property = (Property*) malloc(PROPERTY_SIZE);
    char* body = page->page_body;

    property->value_length = 0;
    uint32_t value_length = 0;
    uint64_t offset = *offset_;
    
    memcpy(&(property->value_type), body + offset, VALUE_TYPE_SIZE);
    offset += VALUE_TYPE_SIZE;
    
    memcpy(&(property->subject_id), body + offset, UINT32_T_SIZE);
    offset += UINT32_T_SIZE;
    memcpy(&(value_length), body + offset, UINT32_T_SIZE);
    offset += UINT32_T_SIZE;
    
    memcpy(property->type, body + offset, NAME_TYPE_SIZE);
    offset += NAME_TYPE_SIZE;
    memcpy(property->subject_type, body + offset, NAME_TYPE_SIZE);
    offset += NAME_TYPE_SIZE;
    
    read_value_from_file(cursor, page, property, value_length, body, offset);

    return property;
}

uint64_t get_size_of_property(const void *pr) {
    Property *property = (Property*) pr;
    uint64_t size = VALUE_TYPE_SIZE + UINT32_T_SIZE * 2 + NAME_TYPE_SIZE * 2;

    switch (property->value_type) {
        case INT: 
            size += UINT32_T_SIZE;
            break;
        case BOOL: 
            size += sizeof(bool);
            break;
        case FLOAT: 
            size += sizeof(float);
            break;
        case STRING: 
            size += CHAR_SIZE * property->value_length;
            break;
        case VOID: 
            break;
    }

    return size;
}

bool compare_property_by_subject(const void *pr_1, const void *pr_2) {
    Property *property_1 = (Property*) pr_1;
    Property *property_2 = (Property*) pr_2;
    return property_1->subject_id == property_2->subject_id;
}

bool compare_subject_property(const void *pr_1, const void *pr_2) {
    Property *property_1 = (Property*) pr_1;
    Property *property_2 = (Property*) pr_2;
    return property_1->subject_id == property_2->subject_id &&
        strcmp(property_1->subject_type, property_2->subject_type) == 0;
}

bool compare_key_property(const void *pr_1, const void *pr_2) {
    Property *property_1 = (Property*) pr_1;
    Property *property_2 = (Property*) pr_2;
    return property_1->subject_id == property_2->subject_id;
}

void memcpy_property(const void *element, char *stack, uint64_t *offset) {
    LOG_DEBUG("In memecpy_property", "");
    if (check_is_null_arg(element, "element") || 
        check_is_null_arg(stack, "stack") ||
        check_is_null_arg(offset, "offset")) {
        return;
    }
    Property *property = (Property*) element;

    uint32_t value_length = property->value_length;

    memcpy(stack + *offset, &(property->value_type), VALUE_TYPE_SIZE);
    *offset += VALUE_TYPE_SIZE;

    memcpy(stack + *offset, &(property->subject_id), UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(stack + *offset, &(value_length), UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(stack + *offset, property->type, NAME_TYPE_SIZE);
    *offset += NAME_TYPE_SIZE;
    
    memcpy(stack + *offset, property->subject_type, NAME_TYPE_SIZE);
    *offset += NAME_TYPE_SIZE;

    memcpy(stack + *offset, property->value, CHAR_SIZE * value_length);
    *offset += CHAR_SIZE * value_length;
}

void *memget_property(const char *stack, uint64_t *offset) {
    LOG_DEBUG("In memeget_property", "");
    if (check_is_null_arg(stack, "stack") ||
        check_is_null_arg(offset, "offset")) {
        return NULL;
    }

    Property *property = (Property*) malloc(PROPERTY_SIZE);

    uint32_t subject_id = 0;
    uint32_t value_length = 0;

    memcpy(&(property->value_type), stack + *offset, VALUE_TYPE_SIZE);
    *offset += VALUE_TYPE_SIZE;

    memcpy(&(subject_id), stack + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(&(value_length), stack + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(property->type, stack + *offset, NAME_TYPE_SIZE);
    *offset += NAME_TYPE_SIZE;

    memcpy(property->subject_type, stack + *offset, NAME_TYPE_SIZE);
    *offset += NAME_TYPE_SIZE;

    char *value = malloc(CHAR_SIZE * value_length);
    memcpy(value, stack + *offset, CHAR_SIZE * value_length);
    *offset += CHAR_SIZE * value_length;
    
    property->subject_id = subject_id;
    property->value_length = value_length;
    property->value = value;

    return property;
}

uint32_t get_property_id(const void *pr) {
    Property *property = (Property *) pr;
    return property->subject_id;
}

void free_property(void *pr) {
    Property *property = (Property *) pr;
    free(property->value);
    free(property);
}
