#pragma once

#include "../managers/page_manager.h"
#include "data/page.h"

#include <stdbool.h>

bool create_entities_test(Cursor *cursor);

bool create_entities_test_1(Cursor *cursor);

bool create_nodes_test(Cursor *cursor);

bool create_nodes_test_1(Cursor *cursor);

bool create_relationships_test(Cursor *cursor);

bool create_relationships_test_1(Cursor *cursor);

bool create_properties_test(Cursor *cursor);

bool delete_entities_test(Cursor *cursor);

bool delete_nodes_test(Cursor *cursor);

bool delete_relationships_test(Cursor *cursor);

bool delete_properties_test(Cursor *cursor);

bool update_entity_test(Cursor *cursor);

bool update_nodes_test(Cursor *cursor);

bool update_nodes_smoke_test(Cursor *cursor);

bool update_relationships_test(Cursor *cursor);

bool update_properties_test(Cursor *cursor);

void select_node_test(Cursor *cursor);

void select_properties_by_node_test(Cursor *cursor);

void select_relationships_by_node_test(Cursor *cursor);

void select_relationship_test(Cursor *cursor);

void select_property_test(Cursor *cursor);

bool create_test(Cursor *cursor);

bool update_test(Cursor *cursor);

bool delete_test(Cursor *cursor);

void select_test(Cursor *cursor);

void select_smoke_test(Cursor *cursor);

void update_smoke_test(Cursor *cursor);

void delete_smoke_test(Cursor *cursor);

void select_delete_smoke_test(Cursor *cursor);

void create_n_nodes_test(Cursor *cursor, int n);

void stack_test(void);
