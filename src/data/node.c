#include "../../include/data/node.h"

#include "../../include/utils/page_utils.h"
#include "../../include/utils/io_utils.h"
#include "utils/checker.h"
#include "utils/logger.h"

#include <string.h>

//FIXME:"good"
void print_node(const Node *node) {
    if (check_is_null_arg(node, "node")) return;
    println("Print node");
    println("Id - %llu", node->id);
    println("Type - %s", node->type);
    println("Length - %i", node->name_length);
    println("Name - %s", node->name);
}

/*
    записывает объект в файл и обновляет offset на страницах
    page->page_header хранит информацию о последнем записанном блоке
*/
void write_node_to_file(Cursor *cursor, Page *page, Entity *entity, const void *nd, const uint32_t *id) {
    LOG_DEBUG("In write_node_to_file", "");
    if (check_is_null_arg(cursor, "cursor") || 
        check_is_null_arg(page, "page") ||
        check_is_null_arg(cursor->file, "cursor->file") ||
        check_is_null_arg(entity, "entity")) {
        return;
    }
    Node *node = (Node*) nd;

    if (id == NULL) {
        node->id = entity->next_id;
        entity->next_id++;
    } else {
        node->id = *id;
    }

    write_uint_32_to_file(cursor->file, node->id);
    write_uint_32_to_file(cursor->file, node->name_length);
    write_string_to_file(cursor->file, node->type, NAME_TYPE_WITH_TERM_LENGTH);

    page->page_header->offset += UINT32_T_SIZE * 2 + NAME_TYPE_SIZE;

    write_big_string_to_file(cursor, page, entity, node->name_length, node->name);
}

/*
    page хранит полностью последний прочитанный блок
*/
void *read_node_from_file(const Cursor *cursor, Page *page, const uint64_t *offset_) {
    LOG_DEBUG("In read_node_from_file", "");
    if (check_is_null_arg(cursor, "cursor") || 
        check_is_null_arg(page, "page") ||
        check_is_null_arg(cursor->file, "cursor->file") ||
        check_is_null_arg(offset_, "offset_")) {
        return NULL;
    }
    Node *node = (Node*) malloc(NODE_SIZE);
    char* body = page->page_body;
    uint64_t offset = *offset_;
    
    memcpy(&(node->id), body + offset, UINT32_T_SIZE);
    offset += UINT32_T_SIZE;
    
    memcpy(&(node->name_length), body + offset, UINT32_T_SIZE);
    offset += UINT32_T_SIZE;
    
    memcpy(node->type, body + offset, NAME_TYPE_SIZE);
    offset += NAME_TYPE_SIZE;
    
    char *name = (char *) malloc(CHAR_SIZE * node->name_length);
    read_big_string_from_file(cursor, page, name, node->name_length, &(offset));
    node->name = name;

    return node;
}

uint64_t get_size_of_node(const void *nd) {
    Node *node = (Node*) nd;
    return UINT32_T_SIZE + UINT32_T_SIZE + CHAR_SIZE * node->name_length + NAME_TYPE_SIZE;
}

bool compare_nodes(const void *nd_1, const void *nd_2) {
    Node *node_1 = (Node*) nd_1;
    Node *node_2 = (Node*) nd_2;
    return (strcmp(node_1->name, node_2->name) == 0) && (node_1->type == node_2->type) == 0;
}

bool compare_id_node(const void *nd_1, const void *nd_2) {
    Node *node_1 = (Node*) nd_1;
    Node *node_2 = (Node*) nd_2;
    return node_1->id == node_2->id &&
        (strcmp(node_1->type, node_2->type) == 0);
}

bool greater_id_node(const void *nd_1, const void *nd_2) {
    Node *node_1 = (Node*) nd_1;
    Node *node_2 = (Node*) nd_2;
    return node_1->id < node_2->id;
}

bool compare_name_node(const void *nd_1, const void *nd_2) {
    Node *node_1 = (Node*) nd_1;
    Node *node_2 = (Node*) nd_2;
    return (strcmp(node_1->name, node_2->name) == 0);
}

void memcpy_node(const void *element, char *stack, uint64_t *offset) {
    LOG_DEBUG("In memecpy_node", "");
    if (check_is_null_arg(element, "element") || 
        check_is_null_arg(stack, "stack") ||
        check_is_null_arg(offset, "offset")) {
        return;
    }

    const Node *node = (Node*) element;

    memcpy(stack + *offset, &(node->id), UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(stack + *offset, &(node->name_length), UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;
    
    memcpy(stack + *offset, node->type, NAME_TYPE_SIZE);
    *offset += NAME_TYPE_SIZE;

    memcpy(stack + *offset, node->name, CHAR_SIZE  *node->name_length);
    *offset += CHAR_SIZE  *node->name_length;
}

void *memget_node(const char *stack, uint64_t *offset) {
    LOG_DEBUG("In memeget_node", "");
    if (check_is_null_arg(stack, "stack") ||
        check_is_null_arg(offset, "offset")) {
        return NULL;
    }
    Node *node = (Node*) malloc(NODE_SIZE);
    uint32_t name_length = 0;
    uint32_t id = 0;

    memcpy(&(id), stack + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(&(name_length), stack + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(node->type, stack + *offset, NAME_TYPE_SIZE);
    *offset += NAME_TYPE_SIZE;

    void *name = malloc(CHAR_SIZE * name_length);
    memcpy(name, stack + *offset, CHAR_SIZE * name_length);
    *offset += CHAR_SIZE * name_length;

    node->id = id;
    node->name_length = name_length;
    node->name = name;

    return node;
}

uint32_t get_node_id(const void *nd) {
    Node *node = (Node*) nd;
    return node->id;
}

void free_node(void *nd) {
    Node *node = (Node*) nd;
    free(node->name);
    free(node);
}
