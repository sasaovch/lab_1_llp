#ifndef LAB_1_TEST
#define LAB_1_TEST

#include "data/data.h"
#include "include.h"

char* generateRandomString(void);

void create_entity(Cursor* cursor);

void 
create_entity_for_test(Cursor* cursor);

void create_nodes(Cursor* cursor);
void create_big_nodes(Cursor* cursor);

void create_relationships(Cursor* cursor);

void create_properties(Cursor* cursor);
void create_relationships2(Cursor* cursor);

void create_properties2(Cursor* cursor);

void delete_entity(Cursor* cursor);

void delete_nodes(Cursor* cursor);

void delete_relationships(Cursor* cursor) ;

void delete_properties(Cursor* cursor);
void delete_relationships2(Cursor* cursor) ;

void delete_properties2(Cursor* cursor);

void update_entity(Cursor* cursor) ;
void update_nodes(Cursor* cursor) ;
void update_relationships(Cursor* cursor);
void update_properties(Cursor* cursor);
void update_relationships2(Cursor* cursor);
void update_properties2(Cursor* cursor);
void smoke_test(Cursor* cursor, int num
) ;

void print_by_node(Cursor* cursor);
void read_test(Cursor* cursor) ;
int test(int argc, char* argv[]) ;
void test_copy(void);
// void read_all_elements_by_type(Cursor* cursor, TypeOfElement type);

#endif
