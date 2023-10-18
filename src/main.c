#include "../include/test_data.h"
#include "time.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Not enough parameters\n");
        exit(EXIT_FAILURE);
    }

    char* filename = argv[1];
    Cursor* cursor = db_open(filename);

    create_entity(cursor);
    create_nodes(cursor);
    create_big_nodes(cursor);
    create_nodes(cursor);
    println("created");
    read_all_elements_by_type(cursor, NODE);
    // read_all_elements_by_type(cursor, RELATIONSHIP);
    // read_all_elements_by_type(cursor, PROPERTY);
    // read_all_elements_by_type(cursor, META);
    // for (int i = 0; i < 20; i++) smoke_test(cursor, i);
    
    db_close(cursor);
    return 0;
}
