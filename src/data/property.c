#include "../../include/include.h"
#include "data/data.h"
#include "data/property.h"
void print_property(Property* property) {
    println("Print property");
    println("Id - %llu", property->id);
    println("Subject length - %i", property->subject_length);
    println("Subject - %s", property->subject);
    println("Key length - %i", property->key_length);
    println("Key - %s", property->key);
    println("Value type - %i", property->value_type);
    println("Value length - %i", property->value_length);
    println("Value - %s", property->value); 
}
void write_string_to_file(Cursor* cursor, char* string, uint32_t length) {
    write_to_file(cursor->file, string, CHAR_SIZE * length);
}
void write_property_to_file(Cursor* cursor, void* pr) {
    Property* property = (Property*) pr;

    write_to_file(cursor->file, &(property->id), UINT32_T_SIZE);
    write_to_file(cursor->file, &(property->subject_length), UINT32_T_SIZE);
    write_to_file(cursor->file, property->subject, CHAR_SIZE * property->subject_length);
    write_to_file(cursor->file, &(property->key_length), UINT32_T_SIZE);
    write_to_file(cursor->file, property->key, CHAR_SIZE * property->key_length);
    write_to_file(cursor->file, &(property->value_type), VALUE_TYPE_SIZE);
    write_to_file(cursor->file, &(property->value_length), UINT32_T_SIZE);
    write_to_file(cursor->file, property->value, CHAR_SIZE * property->value_length);
}
void write_uint_32_to_file(Cursor* cursor, uint32_t number) {
    write_to_file(cursor->file, &(number), UINT32_T_SIZE);
}
//FIXME: looks good needs to test
void write_property_to_file_big(Cursor* cursor, PageHeader* page_header, Entity* entity, void* pr, const uint64_t* pointer) {
    Property2* property = (Property2*) pr;

    set_node_id(property, entity->next_id);
    entity->next_id += 1;
    page_header->offset += UINT32_T_SIZE * 4 + VALUE_TYPE_SIZE;

    write_uint_32_to_file(cursor, property->id);
    write_uint_32_to_file(cursor, property->value_type);
    write_uint_32_to_file(cursor, property->subject_id);
    write_uint_32_to_file(cursor, property->key_length);
    write_uint_32_to_file(cursor, property->value_length);
    write_big_string_to_file(cursor, page_header, entity, property->key_length, global_offset, page_offset, property->key);
    write_big_string_to_file(cursor, page_header, entity, property->value_length, global_offset, page_offset, property->value);
}
//FIXME: need test
void* read_big_element(Cursor* cursor, Entity* meta_page, PageHeader* page_header, void* element, const uint64_t* offset_, char* body, uint32_t* read_block) {
        (void) meta_page;
        Property2* property = (Property2*) element;

        uint32_t id = 0;
        uint32_t subject_id = 0;
        uint32_t key_length = 0;
        uint32_t value_length = 0;

        uint64_t offset = *offset_;
        
        memcpy(&(id), body + offset, UINT32_T_SIZE);
        offset += UINT32_T_SIZE;
        memcpy(&(subject_id), body + offset, UINT32_T_SIZE);
        offset += UINT32_T_SIZE;
        memcpy(&(key_length), body + offset, UINT32_T_SIZE);
        offset += UINT32_T_SIZE;
        memcpy(&(value_length), body + offset, UINT32_T_SIZE);
        offset += UINT32_T_SIZE;
        
        char* key = malloc(CHAR_SIZE * key_length);
        char* value = malloc(CHAR_SIZE * key_length);
        read_bid_string_from_file(cursor, page_header, body, key, key_length, &(offset), read_block);
        read_bid_string_from_file(cursor, page_header, body, value, value_length, &(offset), read_block);
                    
        property->id = id;
        property->subject_id = subject_id;
        property->key_length = key_length;
        property->value_length = value_length;
        property->key = key;
        property->value = value;

        return property;
}

uint64_t get_size_of_property(void* pr) {
    Property* property = (Property*) pr;
    return UINT32_T_SIZE + UINT32_T_SIZE * 3 + CHAR_SIZE * (property->subject_length + property->key_length + property->value_length) + VALUE_TYPE_SIZE;
}

bool compare_property(void* pr_1, void* pr_2) {
    Property* property_1 = (Property*) pr_1;
    Property* property_2 = (Property*) pr_2;
    return (strcmp(property_1->subject, property_2->subject) == 0) && (property_1->key == property_2->key) == 0;
}

bool compare_subject_property(void* pr_1, void* pr_2) {
    Property* property_1 = (Property*) pr_1;
    Property* property_2 = (Property*) pr_2;
    return (strcmp(property_1->subject, property_2->subject) == 0);
}
bool compare_key_property(void* pr_1, void* pr_2) {
    Property* property_1 = (Property*) pr_1;
    Property* property_2 = (Property*) pr_2;
    return (property_1->key == property_2->key) == 0;
}

void* read_property(void* element, Entity* metaPage, char* body, uint64_t* offset) {
    Property* property = (Property*) element;
    (void) metaPage;

    uint32_t id = 0;
    uint32_t subject_length = 0;
    uint32_t key_length = 0;
    uint32_t value_length = 0;
    ValueType value_type;

    memcpy(&(id), body + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(&(subject_length), body + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    char* subject = malloc(CHAR_SIZE * subject_length);
    memcpy(subject, body + *offset, CHAR_SIZE * subject_length);
    *offset += CHAR_SIZE * subject_length;
    
    memcpy(&(key_length), body + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    char* key = malloc(CHAR_SIZE * key_length);
    memcpy(key, body + *offset, CHAR_SIZE * key_length);
    *offset += CHAR_SIZE * key_length;
    
    memcpy(&(value_type), body + *offset, VALUE_TYPE_SIZE);
    *offset += VALUE_TYPE_SIZE;

    memcpy(&(value_length), body+ *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    char* value = malloc(CHAR_SIZE * value_length);
    memcpy(value, body + *offset, CHAR_SIZE * value_length);
    *offset += CHAR_SIZE * value_length;
    
    property->id = id;
    property->subject_length = subject_length;
    property->subject = subject;
    property->key_length = key_length;
    property->key = key;
    property->value_type = value_type;
    property->value_length = value_length;
    property->value = value;

    return property;
}

void memcpy_property(void* element, Entity* metaPage, char* stack, uint64_t* offset) {
    Property* property = (Property*) element;
    (void) metaPage;

    uint32_t subject_length = property->subject_length;
    uint32_t key_length = property->key_length;
    uint32_t value_length = property->value_length;

    memcpy(stack + *offset, &(property->id), UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(stack + *offset, &(subject_length), UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(stack + *offset, property->subject, CHAR_SIZE * subject_length);
    *offset += CHAR_SIZE * subject_length;
    
    memcpy(stack + *offset, &(key_length), UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(stack + *offset, property->key, CHAR_SIZE * key_length);
    *offset += CHAR_SIZE * key_length;

    memcpy(stack + *offset, &(property->value_type), VALUE_TYPE_SIZE);
    *offset += VALUE_TYPE_SIZE;
    
    memcpy(stack + *offset, &(value_length), UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(stack + *offset, property->value, CHAR_SIZE * key_length);
    *offset += CHAR_SIZE * value_length;
}
//FIXME: think about metaPage
void* memget_property(void* element, Entity* metaPage, char* stack, uint64_t* offset) {
    Property* property = (Property*) element;
        (void) metaPage;

    uint32_t id = 0;
    uint32_t subject_length = 0;
    uint32_t key_length = 0;
    uint32_t value_length = 0;
    ValueType value_type;

    memcpy(&(id), stack + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(&(subject_length), stack + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    char* subject = malloc(CHAR_SIZE * subject_length);
    memcpy(subject, stack + *offset, CHAR_SIZE * subject_length);
    *offset += CHAR_SIZE * subject_length;
    
    memcpy(&(key_length), stack + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    char* key = malloc(CHAR_SIZE * key_length);
    memcpy(key, stack + *offset, CHAR_SIZE * key_length);
    *offset += CHAR_SIZE * key_length;
    
    memcpy(&(value_type), stack + *offset, VALUE_TYPE_SIZE);
    *offset += VALUE_TYPE_SIZE;

    memcpy(&(value_length), stack + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    char* value = malloc(CHAR_SIZE * value_length);
    memcpy(value, stack + *offset, CHAR_SIZE * value_length);
    *offset += CHAR_SIZE * value_length;
    
    property->id = id;
    property->subject_length = subject_length;
    property->subject = subject;
    property->key_length = key_length;
    property->key = key;
    property->value_type = value_type;
    property->value_length = value_length;
    property->value = value;

    return property;
}

void set_property_id(void* nd, uint32_t id) {
    Property* node = (Property*) nd;
    node->id = id;
}
