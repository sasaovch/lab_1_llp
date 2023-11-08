#include "../include/utils/test_utils.h"
#include "../include/utils/io_utils.h"

#include "stdlib.h"

char log_level = 100;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Not enough parameters\n");
        exit(EXIT_FAILURE);
    }
    char *filename = argv[1];
    Cursor *cursor = db_open(filename);
    
    // bool result = create_test(cursor);
    // (void) result;
        delete_test(cursor);
    //     update_test(cursor);
    // if (result) {
    //     print_test_format("Tests passed");
    // } else {
    //     print_test_format("Tests failed");
    // }
    // select_test(cursor);
    // delete_smoke_test(cursor);
    // update_smoke_test(cursor);
    // select_smoke_test(cursor);
    db_close(cursor);
    return 0;
}
