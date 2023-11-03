#include "../include/include.h"
#include "utils/test_utils.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Not enough parameters\n");
        exit(EXIT_FAILURE);
    }
    char* filename = argv[1];
    Cursor* cursor = db_open(filename);
    
    // bool result = create_test(cursor) &&
    //     delete_test(cursor) &&
    //     update_test(cursor);
    // if (result) {
    //     print_test_format("Tests passed");
    // } else {
    //     print_test_format("Tests failed");
    // }
    delete_smoke_test(cursor);
    db_close(cursor);
    return 0;
}
