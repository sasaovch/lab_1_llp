#include "../../include/include.h"
// #include "data/data.h"
// #include "data/node.h"
// #include "data/property.h"
// #include "data/relationship.h"
#include <stdint.h>

uint32_t* create_node(Cursor* cursor, Node* node) {
    return create_element(cursor, node, NODE, node->name_type, get_size_of_node, write_node_to_file_big);
}

uint32_t* create_relationship(Cursor* cursor, Relationship2* relationship) {
    return create_element(cursor, relationship, RELATIONSHIP, relationship->name_type, get_size_of_relationship2, write_relationship2_to_file_big);
}

uint32_t* create_property(Cursor* cursor, Property2* property) {
    return create_element(cursor, property, PROPERTY, "proper", get_size_of_property2, write_property2_to_file_big);
}

void delete_node(Cursor* cursor, Node* node) {
    delete_element(cursor, node, get_size_of_node, NODE_SIZE, node->name_type, NODE, return_true, read_node);
}

void delete_node_by_id(Cursor* cursor, Node* node) {
    delete_element(cursor, node, get_size_of_node, NODE_SIZE, node->name_type, NODE, compare_id_node, read_node);
}

void delete_node_by_name(Cursor* cursor, Node* node) {
    delete_element(cursor, node, get_size_of_node, NODE_SIZE, node->name_type, NODE, compare_name_node, read_node);
}

void delete_relationship(Cursor* cursor, Relationship2* relationship) {
    delete_element(cursor, relationship, get_size_of_relationship2, RELATIONSHIP_SIZE, relationship->name_type, RELATIONSHIP, compare_relationship2, read_relationship2);
}
void delete_relationship_by_id(Cursor* cursor, Relationship2* relationship) {
    delete_element(cursor, relationship, get_size_of_relationship2, RELATIONSHIP_SIZE, relationship->name_type, RELATIONSHIP, compare_id_relationship2, read_relationship2);
}
void delete_relationship_by_parent(Cursor* cursor, Relationship2* relationship) {
    delete_element(cursor, relationship, get_size_of_relationship2, RELATIONSHIP_SIZE, relationship->name_type, RELATIONSHIP, compare_parent_relationship2, read_relationship2);
}
void delete_relationship_by_child(Cursor* cursor, Relationship2* relationship) {
    delete_element(cursor, relationship, get_size_of_relationship2, RELATIONSHIP_SIZE, relationship->name_type, RELATIONSHIP, compare_child_relationship2, read_relationship2);
}

void delete_property(Cursor* cursor, Property2* property) {
    delete_element(cursor, property, get_size_of_property2, PROPERTY_SIZE, "proper", PROPERTY, compare_property2, read_property2);
}
void delete_property_by_subject(Cursor* cursor, Property2* property) {
    delete_element(cursor, property, get_size_of_property2, PROPERTY_SIZE, "proper", PROPERTY, compare_subject_property2, read_property2);
}
void delete_property_by_key(Cursor* cursor, Property2* property) {
    delete_element(cursor, property, get_size_of_property2, PROPERTY_SIZE, "proper", PROPERTY, compare_key_property2, read_property2);
}

void update_node(Cursor* cursor, Node* old_node, Node* new_node) {
    update_element(cursor, old_node, new_node, get_size_of_node, NODE_SIZE, old_node->name_type, NODE, read_node, return_true, write_node_to_file_big); 
}
void update_node_by_id(Cursor* cursor, Node* old_node, Node* new_node) {
    update_element(cursor, old_node, new_node, get_size_of_node, NODE_SIZE, old_node->name_type, NODE, read_node, compare_id_node, write_node_to_file_big); 
}
void update_node_by_name(Cursor* cursor, Node* old_node, Node* new_node) {
    update_element(cursor, old_node, new_node, get_size_of_node, NODE_SIZE, old_node->name_type, NODE, read_node, compare_name_node, write_node_to_file_big); 
}

void update_relationship(Cursor* cursor, Relationship2* old_relationship, Relationship2* new_relationship) {
    update_element(cursor, old_relationship, new_relationship, get_size_of_relationship2, RELATIONSHIP_SIZE, old_relationship->name_type, RELATIONSHIP, read_relationship2, compare_relationship2, write_property2_to_file_big); 
}
void update_relationship_by_id(Cursor* cursor, Relationship2* old_relationship, Relationship2* new_relationship) {
    update_element(cursor, old_relationship, new_relationship, get_size_of_relationship2, RELATIONSHIP_SIZE, old_relationship->name_type, RELATIONSHIP, read_relationship2, compare_id_relationship2, write_property2_to_file_big); 
}
void update_relationship_by_child(Cursor* cursor, Relationship2* old_relationship, Relationship2* new_relationship) {
    update_element(cursor, old_relationship, new_relationship, get_size_of_relationship2, RELATIONSHIP_SIZE, old_relationship->name_type, RELATIONSHIP, read_relationship2, compare_child_relationship2, write_property2_to_file_big); 
}
void update_relationship_by_parent(Cursor* cursor, Relationship2* old_relationship, Relationship2* new_relationship) {
    update_element(cursor, old_relationship, new_relationship, get_size_of_relationship2, RELATIONSHIP_SIZE, old_relationship->name_type, RELATIONSHIP, read_relationship2, compare_parent_relationship2, write_property2_to_file_big); 
}

void update_property(Cursor* cursor, Property2* old_property, Property2* new_property) {
    update_element(cursor, old_property, new_property, get_size_of_property2, PROPERTY_SIZE, "proper", PROPERTY, read_property2, compare_property2, write_property2_to_file_big); 
}

void update_property_by_subject(Cursor* cursor, Property2* old_property, Property2* new_property) {
    update_element(cursor, old_property, new_property, get_size_of_property2, PROPERTY_SIZE, "proper", PROPERTY, read_property2, compare_subject_property2, write_property2_to_file_big); 
}

void update_property_by_key(Cursor* cursor, Property2* old_property, Property2* new_property) {
    update_element(cursor, old_property, new_property, get_size_of_property2, PROPERTY_SIZE, "proper", PROPERTY, read_property2, compare_key_property2, write_property2_to_file_big); 
}
