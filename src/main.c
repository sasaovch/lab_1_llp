#include "test.c"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Not enough parameters\n");
        exit(EXIT_FAILURE);
    }

    char* filename = argv[1];
    Cursor* cursor = db_open(filename);

    create_entity(cursor);
    create_nodes(cursor);    
    create_relationships(cursor);    
    create_properties(cursor);

    delete_properties(cursor);
    delete_relationships(cursor);    
    delete_nodes(cursor);    
    delete_entity(cursor);

    create_relationships(cursor);  
    update_relationships(cursor);  
    create_properties(cursor);
    update_properties(cursor);
    create_nodes(cursor);
    update_nodes(cursor);
    read_all_elements_by_type(cursor, NODE);
    read_all_elements_by_type(cursor, RELATIONSHIP);
    read_all_elements_by_type(cursor, PROPERTY);
    read_all_elements_by_type(cursor, META);

    print_by_node(cursor);
    
    db_close(cursor);
    return 0;
}
