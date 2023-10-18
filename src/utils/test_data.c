#include "../../include/test_data.h"
#include "data/data.h"
#include "time.h"
#include <stdint.h>

char* generateRandomString(void) {
    int32_t length = 10;
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char* string = (char*) malloc(length + 1);

    for (int i = 0; i < length; i++) {
        string[i] = charset[rand() % (sizeof(charset) - 1)];
    }

    string[length] = '\0';
    return string;
}

void create_entity(Cursor* cursor) {
    Entity data[] = {
        {NODE, 0, 0, 0, "person"},
        {NODE, 0, 0, 0, "pc"},
        { RELATIONSHIP, 0, 0, 110,"play"},
        { RELATIONSHIP, 0, 0, 11110,"brothe"},
        {PROPERTY, 0, 0,1111,"proper"},
    };

    int count_to_write = sizeof(data) / ENTITY_SIZE;
    for (int i = 0; i < count_to_write; i++) {
        create_type(cursor, &(data[i]));
    }
    flush_page(cursor);
}

void 
create_entity_for_test(Cursor* cursor) {
    Entity data[] = {
        {NODE, 0, 0, 0, "person"},
        {NODE, 0, 0, 0, "pc"},
        {NODE, 0, 0, 0, "laba"},
        {NODE, 0, 0, 0, "unic"},
        {NODE, 0, 0, 0, "human"},
        {NODE, 0, 0, 0, "mac"},
        {NODE, 0, 0, 0, "io"},
        {NODE, 0, 0, 0, "seque"},
        {NODE, 0, 0, 0, "read"},
        {NODE, 0, 0, 0, "write"},
        {NODE, 0, 0, 0, "some"},
        {NODE, 0, 0, 0, "idea"},
        {NODE, 0, 0, 0, "watch"},
        {NODE, 0, 0, 0, "clock"},
        {NODE, 0, 0, 0, "phone"},
        {NODE, 0, 0, 0, "build"},
    };

    int count_to_write = sizeof(data) / ENTITY_SIZE;
    for (int i = 0; i < count_to_write; i++) {
        create_type(cursor, &(data[i]));
    }
    // flush_page(cursor);
}

void create_nodes(Cursor* cursor) {
    Node nodes[] = {
        {0,"person", 9, "sasaovch"},
        {0, "person", 7, "andrey"},
        {0, "pc", 5, "asus"},
        {0, "pc", 4, "mac"},
        {0,"person", 9, "sasaovch1"},
        { 0,"person", 7, "andrey2"},
    };

    int count_to_write = sizeof(nodes) / sizeof(Node);
    for (int i = 0; i < count_to_write; i++) {
        create_node(cursor, &(nodes[i]));
    }
}

void create_big_nodes(Cursor* cursor) {
    uint32_t length = PAGE_SIZE * 2 + PAGE_HEADER_SIZE;
    char* big_string = (char*) malloc(length);
    for (uint32_t i = 0; i < length; i++) {
        memcpy(big_string + i, "s", CHAR_SIZE);
    }
    println(big_string);
    Node nodes[] = {
        {0,"person", length,  big_string}
    };

    int count_to_write = sizeof(nodes) / sizeof(Node);
    for (int i = 0; i < count_to_write; i++) {
        create_big_element(cursor, &(nodes[i]), write_node_to_file, nodes[i].name_type, NODE, get_size_of_node, set_node_id);
    }
}

void create_relationships(Cursor* cursor) {
    Relationship relationships[] = {
        {0,"play", 9, "sasaovch", 5, "asus"},
        {0,"play", 9, "sasaovch", 4, "mac"},
        {0,"play", 7, "andrey", 5, "asus"},
        {0,"brothe",9, "sasaovch", 4, "and"},
        {0,"brothe", 7, "andrey", 5, "sasa"},
    };
    
    int count_to_write = sizeof(relationships) / sizeof(Relationship);
    for (int i = 0; i < count_to_write; i++) {
        create_element(cursor, &(relationships[i]), write_relationship_to_file, relationships[i].name_type, RELATIONSHIP, get_size_of_relationship, set_relation_id);
    }
}

void create_properties(Cursor* cursor) {
    Property property[] = {
        {0,9, "sasaovch", 4, "age", INT, 3, "20"},
        {0,7, "andrey", 4, "age", INT, 3, "14"},
        {0,4, "mac", 7, "color", STRING, 7, "silver"},
        {0,5, "asus", 7, "color", STRING, 6, "black"}
    };

        
    int count_to_write = sizeof(property) / sizeof(Property);
    for (int i = 0; i < count_to_write; i++) {
        create_property(cursor, &(property[i]));
    }
}

void delete_entity(Cursor* cursor) {
    Entity data[] = {
        {NODE, 0, 0, 0,"person"},
        {NODE, 0, 0, 0,"pc"},
        { RELATIONSHIP, 0, 0, 0,"brothe"},
        { RELATIONSHIP, 0, 0,0, "play"},
        {PROPERTY, 0, 0, 0,"proper"},
    };

    int count_to_write = sizeof(data) / ENTITY_SIZE;
    for (int i = 0; i < count_to_write; i++) {
        delete_type(cursor, &(data[i]));
    }
}

void delete_nodes(Cursor* cursor) {
    Node nodes[] = {
        {20000, "person", 11, "lrfkQyuQFj"},
        {0, "person", 7, "andrey"},
        {0,"person", 9, "sasaovch"},
        {0, "pc", 5, "asus"},
    };

    int count_to_write = sizeof(nodes) / sizeof(Node);
    for (int i = 0; i < count_to_write; i++) {
        delete_node_by_name(cursor, &(nodes[i]));
    }
}

void delete_relationships(Cursor* cursor) {
    Relationship relationships[] = {
        {0,"play", 9, "sasaovch", 4, "mac"},
        {0,"play", 7, "andrey", 5, "asus"},
        {0,"brothe",9, "sasaovch", 4, "and"},
        {0,"brothe", 7, "andrey", 5, "sasa"},
    };
    
    int count_to_write = sizeof(relationships) / sizeof(Relationship);
    for (int i = 0; i < count_to_write; i++) {
        delete_relationship(cursor, &(relationships[i]));
    }
}

void delete_properties(Cursor* cursor) {
    Property property[] = {
        {0,9, "sasaovch", 4, "age", INT, 3, "20"},
        {0,7, "andrey", 4, "age", INT, 3, "14"},
        {0,4, "mac", 6, "color", STRING, 7, "silver"},
        {0,5, "asus", 6, "color", STRING, 6, "black"}
    };

        
    int count_to_write = sizeof(property) / sizeof(Property);
    for (int i = 0; i < count_to_write; i++) {
        delete_property(cursor, &(property[i]));
    }
}

void update_entity(Cursor* cursor) {
    Entity old_data[] = {
        {NODE, 0, 0, 0,"person"},
        {NODE, 0, 0, 0,"pc"},
        { RELATIONSHIP, 0, 0,0, "play"},
        { RELATIONSHIP, 0, 0,0, "brothe"},
        {PROPERTY, 0, 0,0, "proper"},
        {NODE, 0, 0,0, "human"},
        {NODE, 0, 0, 0,"noteb"},
    };

    Entity new_data[] = {
        {NODE, 0, 0, 0,"human"},
        {NODE, 0, 0,0, "noteb"},
        { RELATIONSHIP, 0, 0, 0,"game"},
        { RELATIONSHIP, 0, 0, 0,"cibli"},
        {PROPERTY, 0, 0,0, "some"},
        {NODE, 0, 0, 0,"person"},
        {NODE, 0, 0,0, "pc"},
    };

    int count_to_write = sizeof(old_data) / ENTITY_SIZE;
    for (int i = 0; i < count_to_write; i++) {
        update_type(cursor, &(old_data[i]), &(new_data[i]));
    }
}

void update_nodes(Cursor* cursor) {
    Node old_nodes[] = {
        {0,"person", 9, "sasaovch"},
        { 0,"person", 7, "andrey"},
        { 0,"pc", 5, "asus"},
        { 0,"pc", 4, "mac"},
    };

    Node new_nodes[] = {
        {0,"person", 6, "asasa"},
        { 0,"person", 7, "poppop"},
        { 0,"pc", 5, "ASUS"},
        { 0,"pc", 4, "pro"},
    };

    int count_to_write = sizeof(old_nodes) / sizeof(Node);
    for (int i = 0; i < count_to_write; i++) {
        update_node(cursor, &(old_nodes[i]), &(new_nodes[i])); 
    }
}

void update_relationships(Cursor* cursor) {
    Relationship old_relationships[] = {
        {0,"play", 9, "sasaovch", 4, "mac"},
        {0,"play", 7, "andrey", 5, "asus"},
        {0,"brothe",9, "sasaovch", 4, "and"},
        {0,"brothe", 7, "andrey", 5, "sasa"},
    };
    Relationship new_relationships[] = {
        {0,"play", 5, "asas", 4, "mac"},
        {0,"play", 7, "andrey", 5, "asus"},
        {0,"brothe",9, "sasaovch", 4, "and"},
        {0,"brothe", 7, "andrey", 5, "sasa"},
    };
    
    int count_to_write = sizeof(old_relationships) / sizeof(Relationship);
    for (int i = 0; i < count_to_write; i++) {
        update_relationship(cursor, &(old_relationships[i]), &(new_relationships[i])); 
    }
}

void update_properties(Cursor* cursor) {
    Property old_property[] = {
        {0,9, "sasaovch", 4, "age", INT, 3, "20"},
        {0,7, "andrey", 4, "age", INT, 3, "14"},
        {0,4, "mac", 6, "color", STRING, 7, "silver"},
        {0,5, "asus", 6, "color", STRING, 6, "black"}
    };
    
    Property new_property[] = {
        {0,9, "sasaovch", 4, "age", INT, 3, "30"},
        {0,7, "andrey", 4, "age", INT, 3, "24"},
        {0,4, "mac", 6, "color", STRING, 5, "gold"},
        {0,5, "asus", 6, "color", STRING, 7, "yellow"}
    };

        
    int count_to_write = sizeof(old_property) / sizeof(Property);
    for (int i = 0; i < count_to_write; i++) {
        update_property(cursor, &(old_property[i]), &(new_property[i])); 
    }
}

void smoke_test(Cursor* cursor, int num) {
    char* types[] = {"person", "pc","laba","unic",
        "human",
        "mac",
        "io",
        "seque",
        "read",
        "write",
        "some",
        "idea",
        "watch",
        "clock",
        "phone",
        "build"};

    Node *node = (Node*) malloc(NODE_SIZE);

    char* name = malloc(11 * CHAR_SIZE);
    uint32_t* stack = malloc(NODE_SIZE * 500);
    clock_t begin = clock();
    for (int j = 0; j < 500 * num; j++) {
        int r = rand() % 15;
        strcpy(name, "sasaovchsa");
        strcpy(node->name_type, types[r]);

        node->id = 0;
        node->name_length = 11;
        node->name = name;
        uint32_t* id = create_node(cursor, node);
        if (id == NULL) continue;
        node->id = *id;
        memcpy(stack + j * NODE_SIZE, node, NODE_SIZE);
        free(id);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Write %f\n", time_spent);
    clock_t begin_2 = clock();
    for (int j = 0; j <= 200 * num ; j++) {
        memcpy(node, stack + j * NODE_SIZE, NODE_SIZE);
        delete_node_by_id(cursor, node);
    }
    clock_t end_2 = clock();
    double time_spent_2 = (double)(end_2 - begin_2) / CLOCKS_PER_SEC;
    println("Read %f", time_spent_2);
    free(name);
}

void print_by_node(Cursor* cursor) {
    Node node = {0,"person", 9, "sasaovch"};
    print_property_by_node(cursor, &(node));
    print_relationship_by_node(cursor, &(node));
}

void read_test(Cursor* cursor) {
    read_all_elements_by_type(cursor, NODE);
}

int test(int argc, char* argv[]) {
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
    // read_all_elements_by_type(cursor, NODE);
    // read_all_elements_by_type(cursor, RELATIONSHIP);
    // read_all_elements_by_type(cursor, PROPERTY);
    // read_all_elements_by_type(cursor, META);
    return 0;
}
