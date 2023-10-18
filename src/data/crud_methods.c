#include "../../include/include.h"

uint32_t* create_node(Cursor* cursor, Node* node) {
    return create_element(cursor, node, write_node_to_file, node->name_type, NODE, get_size_of_node, set_node_id);
}
uint32_t* create_big_node(Cursor* cursor, Node* node) {
    return create_big_element(cursor, node, write_node_to_file, node->name_type, NODE, get_size_of_node, set_node_id);
}

void create_relationship(Cursor* cursor, Relationship* relationship) {
    create_element(cursor, relationship, write_relationship_to_file, relationship->name_type, RELATIONSHIP, get_size_of_relationship, set_relation_id);
}

void create_property(Cursor* cursor, Property* property) {
    create_element(cursor, property, write_property_to_file, "proper", PROPERTY, get_size_of_property, set_property_id);
}

void delete_node(Cursor* cursor, Node* node) {
    delete_element(cursor, node, get_size_of_node, NODE_SIZE, node->name_type, NODE, read_node, return_true);
}

void delete_node_by_id(Cursor* cursor, Node* node) {
    delete_element(cursor, node, get_size_of_node, NODE_SIZE, "person", NODE, read_node, compare_id_node);
}

void delete_node_by_name(Cursor* cursor, Node* node) {
    delete_element(cursor, node, get_size_of_node, NODE_SIZE, node->name_type, NODE, read_node, compare_name_node);
}

void delete_relationship(Cursor* cursor, Relationship* relationship) {
    delete_element(cursor, relationship, get_size_of_relationship, RELATIONSHIP_SIZE, relationship->name_type, RELATIONSHIP, read_relationship, compare_relationship);
}
void delete_relationship_by_id(Cursor* cursor, Relationship* relationship) {
    delete_element(cursor, relationship, get_size_of_relationship, RELATIONSHIP_SIZE, relationship->name_type, RELATIONSHIP, read_relationship, compare_id_relationship);
}
void delete_relationship_by_parent(Cursor* cursor, Relationship* relationship) {
    delete_element(cursor, relationship, get_size_of_relationship, RELATIONSHIP_SIZE, relationship->name_type, RELATIONSHIP, read_relationship, compare_parent_relationship);
}
void delete_relationship_by_child(Cursor* cursor, Relationship* relationship) {
    delete_element(cursor, relationship, get_size_of_relationship, RELATIONSHIP_SIZE, relationship->name_type, RELATIONSHIP, read_relationship, compare_child_relationship);
}

void delete_property(Cursor* cursor, Property* property) {
    delete_element(cursor, property, get_size_of_property, PROPERTY_SIZE, "proper", PROPERTY, read_property, compare_property);
}
void delete_property_by_subject(Cursor* cursor, Property* property) {
    delete_element(cursor, property, get_size_of_property, PROPERTY_SIZE, "proper", PROPERTY, read_property, compare_subject_property);
}
void delete_property_by_key(Cursor* cursor, Property* property) {
    delete_element(cursor, property, get_size_of_property, PROPERTY_SIZE, "proper", PROPERTY, read_property, compare_key_property);
}

void update_node(Cursor* cursor, Node* old_node, Node* new_node) {
    update_element(cursor, old_node, new_node, get_size_of_node, NODE_SIZE, old_node->name_type, NODE, read_node, return_true, write_node_to_file, set_node_id); 
}
void update_node_by_id(Cursor* cursor, Node* old_node, Node* new_node) {
    update_element(cursor, old_node, new_node, get_size_of_node, NODE_SIZE, old_node->name_type, NODE, read_node, compare_id_node, write_node_to_file, set_node_id); 
}
void update_node_by_name(Cursor* cursor, Node* old_node, Node* new_node) {
    update_element(cursor, old_node, new_node, get_size_of_node, NODE_SIZE, old_node->name_type, NODE, read_node, compare_name_node, write_node_to_file, set_node_id); 
}

void update_relationship(Cursor* cursor, Relationship* old_relationship, Relationship* new_relationship) {
    update_element(cursor, old_relationship, new_relationship, get_size_of_relationship, RELATIONSHIP_SIZE, old_relationship->name_type, RELATIONSHIP, read_relationship, compare_relationship, write_relationship_to_file, set_relation_id); 
}
void update_relationship_by_id(Cursor* cursor, Relationship* old_relationship, Relationship* new_relationship) {
    update_element(cursor, old_relationship, new_relationship, get_size_of_relationship, RELATIONSHIP_SIZE, old_relationship->name_type, RELATIONSHIP, read_relationship, compare_id_relationship, write_relationship_to_file, set_relation_id); 
}
void update_relationship_by_child(Cursor* cursor, Relationship* old_relationship, Relationship* new_relationship) {
    update_element(cursor, old_relationship, new_relationship, get_size_of_relationship, RELATIONSHIP_SIZE, old_relationship->name_type, RELATIONSHIP, read_relationship, compare_child_relationship, write_relationship_to_file, set_relation_id); 
}
void update_relationship_by_parent(Cursor* cursor, Relationship* old_relationship, Relationship* new_relationship) {
    update_element(cursor, old_relationship, new_relationship, get_size_of_relationship, RELATIONSHIP_SIZE, old_relationship->name_type, RELATIONSHIP, read_relationship, compare_parent_relationship, write_relationship_to_file, set_relation_id); 
}

void update_property(Cursor* cursor, Property* old_property, Property* new_property) {
    update_element(cursor, old_property, new_property, get_size_of_property, sizeof(Property), "proper", PROPERTY, read_property, compare_property, write_property_to_file, set_property_id); 
}

void update_property_by_subject(Cursor* cursor, Property* old_property, Property* new_property) {
    update_element(cursor, old_property, new_property, get_size_of_property, sizeof(Property), "proper", PROPERTY, read_property, compare_subject_property, write_property_to_file, set_property_id); 
}

void update_property_by_key(Cursor* cursor, Property* old_property, Property* new_property) {
    update_element(cursor, old_property, new_property, get_size_of_property, sizeof(Property), "proper", PROPERTY, read_property, compare_key_property, write_property_to_file, set_property_id); 
}
