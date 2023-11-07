#pragma once

#include "../data/relationship.h"
#include "../data/function_helper.h"
#include "../data/iterator.h"
#include "../data/property.h"
#include "../data/node.h"

bool check_constraints_create_relationship(
    Cursor *cursor, Relationship *relationship,
    FunctionHelper *function_helper
);

bool check_constraints_create_property(
    Cursor *cursor, Property *property,
    FunctionHelper *function_helper
);

bool delete_node_with_others(Cursor *cursor, Node *node) ;

void delete_relationship_iter(EntityIterator *entity_iterator);

void delete_property_iter(EntityIterator *entity_iterator);

EntityIterator *get_entity_iterator(Cursor *cursor, TypeOfElement element_type);
