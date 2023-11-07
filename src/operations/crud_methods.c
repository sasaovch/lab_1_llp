#include "../../include/operations/crud_methods.h"

#include "../../include/operations/specific.h"
#include "../../include/operations/comparator.h"
#include "data/constants.h"
#include "data/enums.h"

#include <stdlib.h>
#include <string.h>

bool create_node(Cursor *cursor, Node *node) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = return_true;
    function_helper->get_size_of_element = get_size_of_node;
    function_helper->write_element_to_file = write_node_to_file;
    function_helper->read_element_from_file = read_node_from_file;
    function_helper->get_id = get_node_id;
    function_helper->free_element = free_node;
    return create_element(cursor, node, NODE, node->type, function_helper, NULL);
}

bool create_relationship(Cursor *cursor, Relationship *relationship) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->get_size_of_element = get_size_of_relationship;
    function_helper->write_element_to_file = write_relationship_to_file;
    function_helper->get_id = get_relationship_id;
    function_helper->free_element = free_relationship;
    return check_constraints_create_relationship(cursor, relationship, function_helper);
}

bool create_property(Cursor *cursor, Property *property) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->get_size_of_element = get_size_of_property;
    function_helper->free_element = free_property;
    function_helper->write_element_to_file = write_property_to_file;
    function_helper->get_id = get_property_id;
    function_helper->free_element = free_property;
    return check_constraints_create_property(cursor, property, function_helper);
}

bool delete_all_nodes(Cursor *cursor, Node *node) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = return_true;
    function_helper->read_element_from_file = read_node_from_file;
    function_helper->get_id = get_node_id;
    function_helper->get_size_of_element = get_size_of_node;
    function_helper->free_element = free_node;
    return delete_element(cursor, node, NODE_SIZE, node->type, NODE, function_helper);
}

bool delete_node_by_id(Cursor *cursor, Node *node) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_id_node;
    function_helper->get_size_of_element = get_size_of_node;
    function_helper->write_element_to_file = write_node_to_file;
    function_helper->read_element_from_file = read_node_from_file;
    function_helper->get_id = get_node_id;
    function_helper->free_element = free_node;
    return delete_element(cursor, node, NODE_SIZE, node->type, NODE, function_helper);
}

bool delete_node_by_name(Cursor *cursor, Node *node) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_name_node;
    function_helper->get_size_of_element = get_size_of_node;
    function_helper->write_element_to_file = write_node_to_file;
    function_helper->read_element_from_file = read_node_from_file;
    function_helper->get_id = get_node_id;
    function_helper->free_element = free_node;
    return delete_element(cursor, node, NODE_SIZE, node->type, NODE, function_helper);
}

bool delete_nodes_greater_id(Cursor *cursor, Node *node) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = greater_id_node;
    function_helper->get_size_of_element = get_size_of_node;
    function_helper->write_element_to_file = write_node_to_file;
    function_helper->read_element_from_file = read_node_from_file;
    function_helper->get_id = get_node_id;
    function_helper->free_element = free_node;
    return delete_element(cursor, node, NODE_SIZE, node->type, NODE, function_helper);
}

bool delete_all_relationships(Cursor *cursor, Relationship *relationship) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = return_true;
    function_helper->get_size_of_element = get_size_of_relationship;
    function_helper->read_element_from_file = read_relationship_from_file;
    function_helper->free_element = free_relationship;
    return delete_element(cursor, relationship, RELATIONSHIP_SIZE, relationship->type, RELATIONSHIP, function_helper);
}
bool delete_relationship_by_id(Cursor *cursor, Relationship *relationship) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_id_relationship;
    function_helper->get_size_of_element = get_size_of_relationship;
    function_helper->read_element_from_file = read_relationship_from_file;
    function_helper->free_element = free_relationship;
    return delete_element(cursor, relationship, RELATIONSHIP_SIZE, relationship->type, RELATIONSHIP, function_helper);
}
bool delete_relationship_by_parent(Cursor *cursor, Relationship *relationship) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_parent_relationship;
    function_helper->get_size_of_element = get_size_of_relationship;
    function_helper->read_element_from_file = read_relationship_from_file;
    function_helper->free_element = free_relationship;
    return delete_element(cursor, relationship, RELATIONSHIP_SIZE, relationship->type, RELATIONSHIP, function_helper);
}
bool delete_relationship_by_child(Cursor *cursor, Relationship *relationship) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_child_relationship;
    function_helper->get_size_of_element = get_size_of_relationship;
    function_helper->read_element_from_file = read_relationship_from_file;
    function_helper->free_element = free_relationship;
    return delete_element(cursor, relationship, RELATIONSHIP_SIZE, relationship->type, RELATIONSHIP, function_helper);
}

bool delete_all_properties(Cursor *cursor, Property *property) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = return_true;
    function_helper->get_size_of_element = get_size_of_property;
    function_helper->free_element = free_property;
    function_helper->read_element_from_file = read_property_from_file;
    return delete_element(cursor, property, PROPERTY_SIZE, property->type, PROPERTY, function_helper);
}
bool delete_property_by_subject(Cursor *cursor, Property *property) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_subject_property;
    function_helper->get_size_of_element = get_size_of_property;
    function_helper->free_element = free_property;
    function_helper->write_element_to_file = write_property_to_file;
    function_helper->read_element_from_file = read_property_from_file;
    return delete_element(cursor, property, PROPERTY_SIZE, property->type, PROPERTY, function_helper);
}
bool delete_property_by_key(Cursor *cursor, Property *property) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_key_property;
    function_helper->get_size_of_element = get_size_of_property;
    function_helper->free_element = free_property;
    function_helper->write_element_to_file = write_property_to_file;
    function_helper->read_element_from_file = read_property_from_file;
    return delete_element(cursor, property, PROPERTY_SIZE, property->type, PROPERTY, function_helper);
}

bool update_all_nodes(Cursor *cursor, Node *old_node, Node *new_node) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = return_true;
    function_helper->get_size_of_element = get_size_of_node;
    function_helper->write_element_to_file = write_node_to_file;
    function_helper->read_element_from_file = read_node_from_file;
    function_helper->get_id = get_node_id;
    function_helper->free_element = free_node;
    function_helper->memcpy_element = memcpy_node;
    return update_element(cursor, old_node, new_node, NODE_SIZE, old_node->type, NODE, function_helper); 
}
bool update_node_by_id(Cursor *cursor, Node *old_node, Node *new_node) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_id_node;
    function_helper->get_size_of_element = get_size_of_node;
    function_helper->write_element_to_file = write_node_to_file;
    function_helper->read_element_from_file = read_node_from_file;
    function_helper->get_id = get_node_id;
    function_helper->free_element = free_node;
    function_helper->memcpy_element = memcpy_node;
    return update_element(cursor, old_node, new_node, NODE_SIZE, old_node->type, NODE, function_helper); 
}
bool update_node_by_name(Cursor *cursor, Node *old_node, Node *new_node) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_name_node;
    function_helper->get_size_of_element = get_size_of_node;
    function_helper->write_element_to_file = write_node_to_file;
    function_helper->read_element_from_file = read_node_from_file;
    function_helper->get_id = get_node_id;
    function_helper->free_element = free_node;
    function_helper->memcpy_element = memcpy_node;
    return update_element(cursor, old_node, new_node, NODE_SIZE, old_node->type, NODE, function_helper); 
}

bool update_all_relationships(Cursor *cursor, Relationship *old_relationship, Relationship *new_relationship) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = return_true;
    function_helper->get_size_of_element = get_size_of_relationship;
    function_helper->write_element_to_file = write_relationship_to_file;
    function_helper->read_element_from_file = read_relationship_from_file;
    function_helper->get_id = get_relationship_id;
    function_helper->free_element = free_relationship;
    function_helper->memcpy_element = memcpy_relationship;
    return update_element(cursor, old_relationship, new_relationship, RELATIONSHIP_SIZE, old_relationship->type, RELATIONSHIP, function_helper); 
}
bool update_relationship_by_id(Cursor *cursor, Relationship *old_relationship, Relationship *new_relationship) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_id_relationship;
    function_helper->get_size_of_element = get_size_of_relationship;
    function_helper->write_element_to_file = write_relationship_to_file;
    function_helper->read_element_from_file = read_relationship_from_file;
    function_helper->get_id = get_relationship_id;
    function_helper->free_element = free_relationship;
    function_helper->memcpy_element = memcpy_relationship;
    return update_element(cursor, old_relationship, new_relationship, RELATIONSHIP_SIZE, old_relationship->type, RELATIONSHIP, function_helper); 
}
bool update_relationship_by_child(Cursor *cursor, Relationship *old_relationship, Relationship *new_relationship) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_child_relationship;
    function_helper->get_size_of_element = get_size_of_relationship;
    function_helper->write_element_to_file = write_relationship_to_file;
    function_helper->read_element_from_file = read_relationship_from_file;
    function_helper->get_id = get_relationship_id;
    function_helper->free_element = free_relationship;
    function_helper->memcpy_element = memcpy_relationship;
    return update_element(cursor, old_relationship, new_relationship, RELATIONSHIP_SIZE, old_relationship->type, RELATIONSHIP, function_helper); 
}
bool update_relationship_by_parent(Cursor *cursor, Relationship *old_relationship, Relationship *new_relationship) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_parent_relationship;
    function_helper->get_size_of_element = get_size_of_relationship;
    function_helper->write_element_to_file = write_relationship_to_file;
    function_helper->read_element_from_file = read_relationship_from_file;
    function_helper->get_id = get_relationship_id;
    function_helper->free_element = free_relationship;
    function_helper->memcpy_element = memcpy_relationship;
    return update_element(cursor, old_relationship, new_relationship, RELATIONSHIP_SIZE, old_relationship->type, RELATIONSHIP, function_helper); 
}

bool update_all_properties(Cursor *cursor, Property *old_property, Property *new_property) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = return_true;
    function_helper->get_size_of_element = get_size_of_property;
    function_helper->free_element = free_property;
    function_helper->write_element_to_file = write_property_to_file;
    function_helper->read_element_from_file = read_property_from_file;
    function_helper->get_id = get_property_id;
    function_helper->free_element = free_property;
    function_helper->memcpy_element = memcpy_property;
    return update_element(cursor, old_property, new_property, PROPERTY_SIZE, old_property->type, PROPERTY, function_helper); 
}

bool update_property_by_subject(Cursor *cursor, Property *old_property, Property *new_property) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_subject_property;
    function_helper->get_size_of_element = get_size_of_property;
    function_helper->free_element = free_property;
    function_helper->write_element_to_file = write_property_to_file;
    function_helper->read_element_from_file = read_property_from_file;
    function_helper->get_id = get_property_id;
    function_helper->free_element = free_property;
    function_helper->memcpy_element = memcpy_property;
    return update_element(cursor, old_property, new_property, PROPERTY_SIZE, old_property->type, PROPERTY, function_helper); 
}

bool update_property_by_key(Cursor *cursor, Property *old_property, Property *new_property) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_key_property;
    function_helper->get_size_of_element = get_size_of_property;
    function_helper->free_element = free_property;
    function_helper->write_element_to_file = write_property_to_file;
    function_helper->read_element_from_file = read_property_from_file;
    function_helper->get_id = get_property_id;
    function_helper->memcpy_element = memcpy_property;
    return update_element(cursor, old_property, new_property, PROPERTY_SIZE, old_property->type, PROPERTY, function_helper); 
}

Iterator *select_all_nodes_by_type(Cursor *cursor, Node *node) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = return_true;
    function_helper->get_size_of_element = get_size_of_node;
    function_helper->write_element_to_file = write_node_to_file;
    function_helper->read_element_from_file = read_node_from_file;
    function_helper->free_element = free_node;
    return select_element(cursor, NODE, node->type, NODE_SIZE, node, function_helper);
}

Iterator *select_node_by_id(Cursor *cursor, Node *node) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_id_node;
    function_helper->get_size_of_element = get_size_of_node;
    function_helper->write_element_to_file = write_node_to_file;
    function_helper->read_element_from_file = read_node_from_file;
    function_helper->free_element = free_node;
    return select_element(cursor, NODE, node->type, NODE_SIZE, node, function_helper);
}

Iterator *select_node_by_name(Cursor *cursor, Node *node) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_name_node;
    function_helper->get_size_of_element = get_size_of_node;
    function_helper->write_element_to_file = write_node_to_file;
    function_helper->read_element_from_file = read_node_from_file;
    return select_element(cursor, NODE, node->type, NODE_SIZE, node, function_helper);
}

Iterator *select_all_relationships_by_type(Cursor *cursor, Relationship *relationship) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = return_true;
    function_helper->get_size_of_element = get_size_of_relationship;
    function_helper->write_element_to_file = write_relationship_to_file;
    function_helper->read_element_from_file = read_relationship_from_file;
    function_helper->free_element = free_relationship;
    return select_element(cursor, RELATIONSHIP, relationship->type, RELATIONSHIP_SIZE, relationship, function_helper);
}

Iterator *select_relationship_by_id(Cursor *cursor, Relationship *relationship) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_id_relationship;
    function_helper->get_size_of_element = get_size_of_relationship;
    function_helper->write_element_to_file = write_relationship_to_file;
    function_helper->read_element_from_file = read_relationship_from_file;
    function_helper->free_element = free_relationship;
    return select_element(cursor, RELATIONSHIP, relationship->type, RELATIONSHIP_SIZE, relationship, function_helper);
}

Iterator *select_relationship_by_parent(Cursor *cursor, Relationship *relationship) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_parent_relationship;
    function_helper->get_size_of_element = get_size_of_relationship;
    function_helper->write_element_to_file = write_relationship_to_file;
    function_helper->read_element_from_file = read_relationship_from_file;
    function_helper->free_element = free_relationship;
    return select_element(cursor, RELATIONSHIP, relationship->type, RELATIONSHIP_SIZE, relationship, function_helper);
}

Iterator *select_relationship_by_child(Cursor *cursor, Relationship *relationship) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_child_relationship;
    function_helper->get_size_of_element = get_size_of_relationship;
    function_helper->write_element_to_file = write_relationship_to_file;
    function_helper->read_element_from_file = read_relationship_from_file;
    function_helper->free_element = free_relationship;
    return select_element(cursor, RELATIONSHIP, relationship->type, RELATIONSHIP_SIZE, relationship, function_helper);
}

Iterator *select_all_properties_by_type(Cursor *cursor, Property *property) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = return_true;
    function_helper->get_size_of_element = get_size_of_property;
    function_helper->free_element = free_property;
    function_helper->write_element_to_file = write_property_to_file;
    function_helper->read_element_from_file = read_property_from_file;
    return select_element(cursor, PROPERTY, property->type, PROPERTY_SIZE, property, function_helper);
}

Iterator *select_property_by_subject(Cursor *cursor, Property *property) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_subject_property;
    function_helper->get_size_of_element = get_size_of_property;
    function_helper->free_element = free_property;
    function_helper->write_element_to_file = write_property_to_file;
    function_helper->read_element_from_file = read_property_from_file;
    return select_element(cursor, PROPERTY, property->type, PROPERTY_SIZE, property, function_helper);
}

Iterator *select_property_by_key(Cursor *cursor, Property *property) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_key_property;
    function_helper->get_size_of_element = get_size_of_property;
    function_helper->free_element = free_property;
    function_helper->write_element_to_file = write_property_to_file;
    function_helper->read_element_from_file = read_property_from_file;
    return select_element(cursor, PROPERTY, property->type, PROPERTY_SIZE, property, function_helper);
}

EntityIterator *select_all_nodes(Cursor *cursor) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = return_true;
    function_helper->get_size_of_element = get_size_of_node;
    function_helper->write_element_to_file = write_node_to_file;
    function_helper->read_element_from_file = read_node_from_file;
    function_helper->get_id = get_node_id;
    function_helper->free_element = free_node;
    function_helper->memcpy_element = memcpy_node;

    Iterator *iterator = (Iterator*) malloc(ITERATOR_SIZE);
    iterator->cursor = cursor;
    iterator->entity = (Entity *) malloc(ENTITY_SIZE);
    iterator->read_block_ = malloc(UINT32_T_SIZE);
    iterator->offset_ =  malloc(UINT64_T_SIZE);;
    iterator->element = (void *) malloc(NODE_SIZE);
    iterator->helper = (void *) malloc(NODE_SIZE);
    iterator->function_helper = function_helper;

    EntityIterator *enity_iterator = get_entity_iterator(cursor, NODE);
    if (enity_iterator == NULL) {
        free_iter(iterator);
        return NULL;
    }

    memcpy(iterator->entity, enity_iterator->entity, ENTITY_SIZE);
    *(iterator->read_block_) = enity_iterator->entity->first_page;
    *(iterator->offset_) = 0;
    enity_iterator->iterator = iterator;

    return enity_iterator;
}

EntityIterator *select_all_relationships(Cursor *cursor) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = return_true;
    function_helper->get_size_of_element = get_size_of_relationship;
    function_helper->write_element_to_file = write_relationship_to_file;
    function_helper->read_element_from_file = read_relationship_from_file;
    function_helper->get_id = get_relationship_id;
    function_helper->free_element = free_relationship;
    function_helper->memcpy_element = memcpy_relationship;

    Iterator *iterator = (Iterator*) malloc(ITERATOR_SIZE);
    iterator->cursor = cursor;
    iterator->entity = (Entity *) malloc(ENTITY_SIZE);
    iterator->read_block_ = malloc(UINT32_T_SIZE);
    iterator->offset_ =  malloc(UINT64_T_SIZE);;
    iterator->element = (void *) malloc(RELATIONSHIP_SIZE);
    iterator->helper = (void *) malloc(RELATIONSHIP_SIZE);
    iterator->function_helper = function_helper;

    EntityIterator *enity_iterator = get_entity_iterator(cursor, RELATIONSHIP);
    if (enity_iterator == NULL) {
        free_iter(iterator);
        return NULL;
    }

    memcpy(iterator->entity, enity_iterator->entity, ENTITY_SIZE);
    *(iterator->read_block_) = enity_iterator->entity->first_page;
    *(iterator->offset_) = 0;
    enity_iterator->iterator = iterator;

    return enity_iterator;
}

EntityIterator *select_all_properties(Cursor *cursor) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = return_true;
    function_helper->get_size_of_element = get_size_of_property;
    function_helper->free_element = free_property;
    function_helper->write_element_to_file = write_property_to_file;
    function_helper->read_element_from_file = read_property_from_file;
    function_helper->get_id = get_property_id;
    function_helper->free_element = free_property;
    function_helper->memcpy_element = memcpy_property;

    Iterator *iterator = (Iterator*) malloc(ITERATOR_SIZE);
    iterator->cursor = cursor;
    iterator->entity = (Entity *) malloc(ENTITY_SIZE);
    iterator->read_block_ = malloc(UINT32_T_SIZE);
    iterator->offset_ =  malloc(UINT64_T_SIZE);;
    iterator->element = (void *) malloc(PROPERTY_SIZE);
    iterator->helper = (void *) malloc(PROPERTY_SIZE);
    iterator->function_helper = function_helper;

    EntityIterator *enity_iterator = get_entity_iterator(cursor, PROPERTY);
    if (enity_iterator == NULL) {
        free_iter(iterator);
        return NULL;
    }

    memcpy(iterator->entity, enity_iterator->entity, ENTITY_SIZE);
    *(iterator->read_block_) = enity_iterator->entity->first_page;
    *(iterator->offset_) = 0;
    enity_iterator->iterator = iterator;

    return enity_iterator;
}

EntityIterator *get_entity_iter_by_node(Cursor *cursor, Node *node, TypeOfElement element_type) {
    Iterator *iterator = select_node_by_id(cursor, node);
    if (has_next(iterator)) {
        EntityIterator *enity_iterator = get_entity_iterator(cursor, element_type);
        if (enity_iterator == NULL) {
            free_iter(iterator);
            return NULL;
        }

        memcpy(iterator->entity, enity_iterator->entity, ENTITY_SIZE);
        *(iterator->read_block_) = enity_iterator->entity->first_page;
        *(iterator->offset_) = 0;
        enity_iterator->iterator = iterator;

        return enity_iterator;
    } else {
        free_iter(iterator);
        return NULL;
    }
}

EntityIterator *select_properties_by_node(Cursor *cursor, Node *node) {
    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_subject_property;
    function_helper->get_size_of_element = get_size_of_property;
    function_helper->free_element = free_property;
    function_helper->write_element_to_file = write_property_to_file;
    function_helper->read_element_from_file = read_property_from_file;

    EntityIterator *entity_iterator = get_entity_iter_by_node(cursor, node, PROPERTY);
    if (entity_iterator == NULL) {
        return NULL;
    }
    entity_iterator->iterator->function_helper = function_helper;

    Node *nd = (Node*) entity_iterator->iterator->element;
    Property *property = (Property*) malloc(PROPERTY_SIZE);
    property->subject_id = nd->id;
    strcpy(property->subject_type, nd->type);
    entity_iterator->iterator->helper = property;

    return entity_iterator;
}

EntityIterator *select_relationships_by_node(Cursor *cursor, Node *node) {

    FunctionHelper *function_helper = (FunctionHelper*) malloc(FUNCTION_HELPER);
    function_helper->condition = compare_relationship_by_node;
    function_helper->get_size_of_element = get_size_of_relationship;
    function_helper->write_element_to_file = write_relationship_to_file;
    function_helper->read_element_from_file = read_relationship_from_file;
    function_helper->free_element = free_relationship;

    EntityIterator *entity_iterator = get_entity_iter_by_node(cursor, node, RELATIONSHIP);
    if (entity_iterator == NULL) {
        return NULL;
    }
    entity_iterator->iterator->function_helper = function_helper;

    Node *nd = (Node*) entity_iterator->iterator->element;
    Relationship *relationship = (Relationship*) malloc(RELATIONSHIP_SIZE);
    relationship->parent_id = nd->id;
    relationship->child_id = nd->id;
    strcpy(relationship->parent_type, nd->type);
    entity_iterator->iterator->helper = relationship;

    return entity_iterator;
}
