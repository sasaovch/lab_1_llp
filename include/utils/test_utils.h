#ifndef LAB_1_TEST_H
#define LAB_1_TEST_H

#include "../data/util_data.h"

bool create_entity(Cursor* cursor);
bool create_entity_1(Cursor* cursor);

bool create_nodes(Cursor* cursor);
bool create_nodes_1(Cursor* cursor);

bool create_relationships(Cursor* cursor);
bool create_relationships_1(Cursor* cursor);

bool create_properties(Cursor* cursor);

bool delete_entity(Cursor* cursor);
bool delete_nodes(Cursor* cursor);
bool delete_relationships(Cursor* cursor);
bool delete_properties(Cursor* cursor);

bool update_entity(Cursor* cursor);
bool update_nodes(Cursor* cursor);
bool update_relationships(Cursor* cursor);
bool update_properties(Cursor* cursor);

void select_node(Cursor* cursor);
void select_properties_by_node_t(Cursor* cursor);
void select_relationships_by_node_t(Cursor* cursor);
void select_relationship(Cursor* cursor);
void select_property(Cursor* cursor);

bool create_test(Cursor* cursor);
bool update_test(Cursor* cursor);
bool delete_test(Cursor* cursor);
void select_test(Cursor* cursor);

void select_smoke_test(Cursor* cursor);
void update_smoke_test(Cursor* cursor);
void delete_smoke_test(Cursor* cursor);
void select_delete_smoke_test(Cursor* cursor);

void create_n_nodes_(Cursor* cursor, int n);

#endif
