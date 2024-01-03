#include "../../include/operations/specific.h"

#include "../../include/operations/crud_methods.h"
#include "data/constants.h"
#include "data/enums.h"
#include "data/node.h"
#include "operations/common.h"
#include "utils/checker.h"
#include "utils/logger.h"

#include <stdint.h>
#include <string.h>

bool check_constraints_create_relationship(
    Cursor *cursor, Relationship *relationship,
    FunctionHelper *function_helper
) {
    if (check_is_null_arg(cursor, "cursor") ||
        check_is_null_arg(relationship, "relationship") ||
        check_is_null_arg(function_helper, "function_helper")) {
            return false;
    }

    Node *nd = (Node*) malloc(NODE_SIZE);
    nd->id = relationship->parent_id;
    strcpy(nd->type, relationship->parent_type);
    
    Iterator *iter = select_node_by_id(cursor, nd);
    if (!has_next(iter)) {
        LOG_INFO("Can't find node element", "");
        return NULL;
    }

    nd->id = relationship->child_id;
    strcpy(nd->type, relationship->child_type);

    iter = select_node_by_id(cursor, nd);
    if (!has_next(iter)) {
        return NULL;
    }

    free(iter);
    free(nd);

    return create_element(cursor, relationship, RELATIONSHIP, relationship->type, function_helper, NULL);
}

bool check_constraints_create_property(
    Cursor *cursor, Property *property,
    FunctionHelper *function_helper
) {
    if (check_is_null_arg(cursor, "cursor") ||
        check_is_null_arg(property, "property") ||
        check_is_null_arg(function_helper, "function_helper")) {
            return false;
    }

    Node *nd = (Node*) malloc(NODE_SIZE);
    nd->id = property->subject_id;
    strcpy(nd->type, property->subject_type);
    
    Iterator *iter = select_node_by_id(cursor, nd);
    if (!has_next(iter)) {
        LOG_INFO("Can't find node element", "");
        return NULL;
    }

    iter = select_property_by_subject(cursor, property);
    if (has_next(iter)) {
        return NULL;
    }

    PageHeader *page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    uint64_t pointer = 0;
    
    Entity *entity = get_entity(cursor, PROPERTY, property->type, &(pointer));
    if (entity == NULL) {
        free(entity);
        free(page_header);
        LOG_INFO("Error to find Entity with name %s", property->type);
        return NULL;
    }
    if (entity->value_type != property->value_type) {
        return NULL;
    }
    free(page_header);
    free(entity);
    free(iter);
    free(nd);

    return create_element(cursor, property, PROPERTY, property->type, function_helper, NULL);
}

void delete_relationship_iter(EntityIterator *entity_iterator) {
    Iterator *iterator = entity_iterator->iterator;
    uint64_t offset = *(iterator->offset_) - RELATIONSHIP_SIZE;
    uint32_t read_block = *(iterator->read_block_);

    Cursor *cursor = iterator->cursor;
    Page *page = read_page_from_file(cursor, read_block);
    uint32_t next_page = page->page_header->next_block;
    uint64_t pointer = (*(entity_iterator->offset_) - 1) * ENTITY_SIZE + entity_iterator->page->page_header->block_number * PAGE_SIZE;

    remove_small_element(cursor, page, iterator->entity, offset, RELATIONSHIP_SIZE, &(pointer));

    if (page->page_header->block_number != read_block && page->page_header->block_number != 0) {
        *(entity_iterator->iterator->offset_) = 0;
        *(entity_iterator->iterator->read_block_) = next_page;
    } else {
        *(entity_iterator->iterator->offset_) = offset;
        *(entity_iterator->iterator->read_block_) = read_block;
    }
}

void delete_property_iter(EntityIterator *entity_iterator) {
    Property *delete_prop = entity_iterator->iterator->element;
    Iterator *iterator = entity_iterator->iterator;
    Cursor *cursor = iterator->cursor;

    delete_element(cursor, delete_prop, PROPERTY_SIZE, delete_prop->type, PROPERTY, iterator->function_helper);
}

bool delete_node_with_others(
        Cursor *cursor, Node *node) {

    EntityIterator *entity_iterator_rel = select_relationships_by_node(cursor, node);
    while (entity_iterator_rel != NULL && entity_has_next(entity_iterator_rel)) {
        delete_relationship_iter(entity_iterator_rel);
    }

    EntityIterator *entity_iterator_prop = select_properties_by_node(cursor, node);
    while (entity_iterator_prop != NULL && entity_has_next(entity_iterator_prop)) {
        delete_property_iter(entity_iterator_prop);
    }
    free_entity_iter(entity_iterator_rel);
    free_entity_iter(entity_iterator_prop);
    
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_id_node;
    function_helper->get_size_of_element = get_size_of_node;
    function_helper->write_element_to_file = write_node_to_file;
    function_helper->read_element_from_file = read_node_from_file;
    
    bool result = delete_element(cursor, node, NODE_SIZE, node->type, NODE, function_helper);
    free(function_helper);
    return result;
}
