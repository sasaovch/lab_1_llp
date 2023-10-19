#ifndef LAB_1_CRUD
#define LAB_1_CRUD

#include "common.h"

uint32_t* create_node(Cursor* cursor, Node* node);

uint32_t* create_relationship(Cursor* cursor, Relationship2* relationship);

uint32_t* create_property(Cursor* cursor, Property2* property);

void delete_node(Cursor* cursor, Node* node);

void delete_node_by_id(Cursor* cursor, Node* node);

void delete_node_by_name(Cursor* cursor, Node* node);

void delete_relationship(Cursor* cursor, Relationship2* relationship);

void delete_relationship_by_id(Cursor* cursor, Relationship2* relationship);

void delete_relationship_by_parent(Cursor* cursor, Relationship2* relationship);

void delete_relationship_by_child(Cursor* cursor, Relationship2* relationship);

void delete_property(Cursor* cursor, Property2* property);

void delete_property_by_subject(Cursor* cursor, Property2* property);

void delete_property_by_key(Cursor* cursor, Property2* property);

void update_node(Cursor* cursor, Node* old_node, Node* new_node);

void update_node_by_id(Cursor* cursor, Node* old_node, Node* new_node);

void update_node_by_name(Cursor* cursor, Node* old_node, Node* new_node);

void update_relationship(Cursor* cursor, Relationship2* old_relationship, Relationship2* new_relationship);

void update_relationship_by_id(Cursor* cursor, Relationship2* old_relationship, Relationship2* new_relationship);

void update_relationship_by_child(Cursor* cursor, Relationship2* old_relationship, Relationship2* new_relationship);

void update_relationship_by_parent(Cursor* cursor, Relationship2* old_relationship, Relationship2* new_relationship);

void update_property(Cursor* cursor, Property2* old_property, Property2* new_property);

void update_property_by_subject(Cursor* cursor, Property2* old_property, Property2* new_property);

void update_property_by_key(Cursor* cursor, Property2* old_property, Property2* new_property);


void create_relationship2(Cursor* cursor, Relationship2* relationship);

void create_property2(Cursor* cursor, Property2* property);

void delete_relationship2(Cursor* cursor, Relationship2* relationship);

void delete_relationship2_by_id(Cursor* cursor, Relationship2* relationship);

void delete_relationship2_by_parent(Cursor* cursor, Relationship2* relationship);

void delete_relationship2_by_child(Cursor* cursor, Relationship2* relationship);

void delete_property2(Cursor* cursor, Property2* property);

void delete_property2_by_subject(Cursor* cursor, Property2* property);

void delete_property2_by_key(Cursor* cursor, Property2* property);

void update_relationship2(Cursor* cursor, Relationship2* old_relationship, Relationship2* new_relationship);

void update_relationship2_by_id(Cursor* cursor, Relationship2* old_relationship, Relationship2* new_relationship);

void update_relationship2_by_child(Cursor* cursor, Relationship2* old_relationship, Relationship2* new_relationship);

void update_relationship2_by_parent(Cursor* cursor, Relationship2* old_relationship, Relationship2* new_relationship);

void update_property2(Cursor* cursor, Property2* old_property, Property2* new_property);

void update_property2_by_subject(Cursor* cursor, Property2* old_property, Property2* new_property);

void update_property2_by_key(Cursor* cursor, Property2* old_property, Property2* new_property);

#endif
