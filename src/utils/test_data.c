#include "../../include/utils/test_utils.h"
#include "../../include/include.h"

#include "data/node.h"
#include "time.h"

bool create_entity(Cursor* cursor) {
    Entity data[] = {
        {NODE, VOID, 0, 0, 0, "person"},
        { RELATIONSHIP, VOID,  0, 0, 0,"play"},
        {PROPERTY, INT,  0, 0,0,"age"},
        {NODE, VOID,  0, 0, 0, "car"},
        { RELATIONSHIP, VOID,  0, 0, 0,"drive"},
        {PROPERTY, STRING,  0, 0,0,"color"},
    };

    int count_to_write = sizeof(data) / ENTITY_SIZE;
    for (int i = 0; i < count_to_write; i++) {
        if (!create_type(cursor, &(data[i]))) {
            print_test_format("Fail to create Entity %i", i);
            return false;
        };
    }
    return true;
}

bool create_entity_1(Cursor* cursor) {
    Entity data[] = {
        {NODE,  VOID, 0, 0, 0, "pc"},
        { RELATIONSHIP, VOID,  0, 0, 11110,"brother"},
        {NODE,  VOID, 0, 0, 0, "phone"},
        { RELATIONSHIP, VOID,  0, 0, 11110,"call"},
    };

    Entity duplicate_data[] = {
        {NODE, VOID,  0, 0, 0, "person"},
        { RELATIONSHIP, VOID,  0, 0, 110,"play"},
        { RELATIONSHIP, VOID,  0, 0, 110,"drive"},
        {NODE,  VOID, 0, 0, 0, "car"},
    };

    int count_to_write = sizeof(data) / ENTITY_SIZE;
    for (int i = 0; i < count_to_write; i++) {
        if (!create_type(cursor, &(data[i]))) {
            print_test_format("Fail to create Entity %i", i);
            return false;
        };
    }
    count_to_write = sizeof(duplicate_data) / ENTITY_SIZE;
    for (int i = 0; i < count_to_write; i++) {
        if (create_type(cursor, &(duplicate_data[i]))) {
            print_test_format("Fail, created duplicate Entity %i", i);
            return false;
        };
    }
    return true;
}

bool create_nodes(Cursor* cursor) {
    char* name = (char*) malloc(PAGE_SIZE + PAGE_BODY_SIZE);
    for (uint32_t i = 0; i < PAGE_SIZE + PAGE_BODY_SIZE - 1; i++) {
        name[i] = 'a';
    }
    Node nodes[] = {
        {0, 9, "person", "sasaovch"},
        {0,  7, "person", "andrey"},
        {0, 4, "car", "bmw"},
        {0, 4, "person", "ani"},
        {0, PAGE_SIZE + PAGE_BODY_SIZE, "person", name},
    };

    Node non_existing_node[] = {
        {0,  5, "pc", "asus"}
    };

    int count_to_write = sizeof(nodes) / sizeof(Node);
    for (int i = 0; i < count_to_write; i++) {
        uint32_t* id = create_node(cursor, &(nodes[i]));
        if (id == NULL) {
            free(id);
            print_test_format("Fail to create Node %i", i);
        };
        free(id);
    }
    count_to_write = sizeof(non_existing_node) / sizeof(Node);
    for (int i = 0; i < count_to_write; i++) {
        uint32_t* id = create_node(cursor, &(non_existing_node[i]));
        if (id != NULL) {
            free(id);
            print_test_format("Fail, created Node with non existing type");
            return false;
        };
        free(id);
    }
    return true;
}

bool create_nodes_1(Cursor* cursor) {
    char* name1 = (char*) malloc(PAGE_SIZE + PAGE_BODY_SIZE);
    for (uint32_t i = 0; i < PAGE_SIZE + PAGE_BODY_SIZE - 1; i++) {
        name1[i] = 'b';
    }
    (void) cursor;
    Node nodes[] = {
        {0,  5, "pc","asus"},
        {0, 4, "pc", "mac"},
        {0, 4, "pc", "mac"},
        {0, 4, "person", "llll"},
        {0, PAGE_SIZE + PAGE_BODY_SIZE,"person", name1},
        {0, 9, "person","sasaovch1"},
        {0, 7, "phone","iphone"},
        {0, 8, "phone", "oneplus"},
    };

    int count_to_write = sizeof(nodes) / sizeof(Node);
    for (int i = 0; i < count_to_write; i++) {
        uint32_t* id = create_node(cursor, &(nodes[i]));
        if (id == NULL) {
            free(id);
            print_test_format("Fail to create Node %i", i);
            return false;
        };
        free(id);
    }
    return true;
}

bool create_relationships(Cursor* cursor) {
    Relationship relationships[] = {
        {0, 0 , 0, "drive", "person", "car"},
        {0, 1 ,1, "some", "person", "pc"},
        {0, 100 ,1, "some", "person", "pc"},
    };
    uint32_t* id = create_relationship(cursor, &(relationships[0]));
    if (id == NULL) {
        free(id);
        print_test_format("Fail to create Relationship 0");
        return false;
    };
    free(id);

    id = create_relationship(cursor, &(relationships[1]));
    if (id != NULL) {
        free(id);
        print_test_format("Fail, created Relationship with non existing type");
        return false;
    };

    id = create_relationship(cursor, &(relationships[2]));
    if (id != NULL) {
        free(id);
        print_test_format("Fail, created Relationship with non existing parent node id");
        return false;
    };
    return true;
}

bool create_relationships_1(Cursor* cursor) {
    Relationship relationships[] = {
        {0, 0, 0, "play", "person", "pc"},
        {0, 1,0, "play", "person", "pc"},
        {0, 3,0, "play", "person", "pc"},
        {0,0,1, "brother", "person", "person"},
        {0,1, 0, "brother", "person", "person"},
    };
    
    int count_to_write = sizeof(relationships) / sizeof(Relationship);
    for (int i = 0; i < count_to_write; i++) {
        uint32_t* id = create_relationship(cursor, &(relationships[i]));
        if (id == NULL) {
            free(id);
            print_test_format("Fail to creat Relationship %i", i);
            return false;
        };
        free(id);
    }
    return true;
}

bool create_properties(Cursor* cursor) {
    uint32_t age = 20;
    float fl = 1.1f;
    Property property[] = {
        {INT,2, UINT32_T_SIZE, "age","person",  &(age)},
        {INT,0, UINT32_T_SIZE, "age","person",  &(age)},
        {INT,3, UINT32_T_SIZE, "age","person",  &(age)},
        {STRING,1, 6, "color", "person",  "black"},
    };
    Property wrong_property[] = {
        {INT,0, UINT32_T_SIZE, "age","person",  &(age)},
        {FLOAT, 1, sizeof(float),"age", "person", &(fl)},
        {STRING,0, 2, "some","car",  "2"},
    };

    int count_to_write = sizeof(property) / sizeof(Property);
    for (int i = 0; i < count_to_write; i++) {
        uint32_t* id = create_property(cursor, &(property[i]));
        if (id == NULL) {
            free(id);
            print_test_format("Fail to creat Property %i", i);
            return false;
        };
        free(id);
    }
    count_to_write = sizeof(wrong_property) / sizeof(Property);
    for (int i = 0; i < count_to_write; i++) {
        uint32_t* id = create_property(cursor, &(wrong_property[i]));
        if (id != NULL) {
            free(id);
            print_test_format("Fail, created wrong Property %i", i);
            return false;
        };
    }
    return true;
}

bool delete_entity(Cursor* cursor) {
    Entity data[] = {
        {NODE, VOID, 0, 0,0, "car"},
    };

    int count_to_write = sizeof(data) / ENTITY_SIZE;
    for (int i = 0; i < count_to_write; i++) {
        delete_type(cursor, &(data[i]));
    }
    return true;
}

bool delete_nodes(Cursor* cursor) {
    Node node = {
        0, 9, "person", ""
    };
    
    if (!delete_node_with_others(cursor, &(node))) {
        print_test_format("Failed to delete node");
        return false;
    };

    Iterator* iterator = select_node_by_id(cursor, &(node));
    if (has_next(iterator)) {
        free_iter(iterator);
        print_test_format("Fail, node wasn't deleted");
        return false;
    }

    if (delete_node_by_id(cursor, &(node))) {
        free_iter(iterator);
        print_test_format("Deleted non existing node");
        return false;
    }
    free_iter(iterator);
    return true;
}

bool delete_relationships(Cursor* cursor) {
    Relationship non_exist_relationship = {
        100,0, 0, "play", "", ""
    };
    Relationship relationship = {1, 1,2, "play", "person", "pc"};
    
    if (delete_relationship_by_id(cursor, &(non_exist_relationship))) {
        print_test_format("Failed to delete relationship");
        return false;
    };
    
    if (!delete_relationship_by_id(cursor, &(relationship))) {
        print_test_format("Failed to delete relationship");
        return false;
    };

    Iterator* iterator = select_relationship_by_id(cursor, &(relationship));
    if (has_next(iterator)) {
        free_iter(iterator);
        print_test_format("Fail, deleted non existing relationship");
        return false;
    }

    if (delete_relationship_by_id(cursor, &(relationship))) {
        free_iter(iterator);
        print_test_format("Deleted non existing relationship");
        return false;
    };
    free_iter(iterator);
    return true;
}

bool delete_properties(Cursor* cursor) {
    uint32_t age = 20;
    Property property = {INT,2, UINT32_T_SIZE, "age","person",  &(age)};

    if (!delete_property_by_subject(cursor, &(property))) {
        print_test_format("Failed to delete property");
        return false;
    };
    Iterator* iterator = select_property_by_subject(cursor, &(property));
    if (has_next(iterator)) {
        free_iter(iterator);
        print_test_format("Fail, deleted non existing property");
        return false;
    }
    free_iter(iterator);
    return true;
}

bool update_nodes(Cursor* cursor) {
    Node old_nodes[] = {
        {3, 9,"person", "sasaovch"},
    };

    Node new_nodes[] = {
        {0, 2, "person", "I"},
    };

    int count_to_write = sizeof(old_nodes) / sizeof(Node);
    for (int i = 0; i < count_to_write; i++) {
        if (!update_node_by_id(cursor, &(old_nodes[i]), &(new_nodes[i]))) {
            print_test_format("Fail to update node %i", i);
            return false;
        }; 
    }

    Iterator* iterator = select_node_by_id(cursor, &(old_nodes[0]));
    if (!has_next(iterator)) {
        free_iter(iterator);
        print_test_format("Fail, not found updated node");
        return false;
    }
    Node* nd = (Node*) next(iterator);
    if (strcmp(nd->name, new_nodes[0].name) != 0) {
        free_iter(iterator);
        print_test_format("Fail, wasn't updated node");
        return false;
    }
    free_iter(iterator);
    return true;
}

bool update_relationships(Cursor* cursor) {
    Relationship old_relationships[] = {
        {2, 3,2, "play", "person", "pc"},
    };
    Relationship new_relationships[] = {
        {0, 3,5, "play", "person", "pc"},
    };
    
    int count_to_write = sizeof(old_relationships) / sizeof(Relationship);
    for (int i = 0; i < count_to_write; i++) {
        if (!update_relationship_by_id(cursor, &(old_relationships[i]), &(new_relationships[i]))) {
            print_test_format("Fail to update relationship %i", i);
            return false;
        }; 
    }

    Iterator* iterator = select_relationship_by_id(cursor, &(old_relationships[0]));
    if (!has_next(iterator)) {
        free_iter(iterator);
        print_test_format("Fail, not found updated relationship");
        return false;
    }
    Relationship* rel = (Relationship*) next(iterator);
    if (rel->child_id != new_relationships[0].child_id) {
        free_iter(iterator);
        print_test_format("Fail, wasn't updated relationship");
        return false;
    }
    free_iter(iterator);
    return true;
}

bool update_properties(Cursor* cursor) {
    uint32_t age = 20;
    uint32_t new_age = 21;
    Property old_property[] = {
        {INT,3, UINT32_T_SIZE, "age","person",  &(age)},
    };
    Property new_property[] = {
        {INT,3, UINT32_T_SIZE, "age","person",  &(new_age)},
    };
   
    int count_to_write = sizeof(old_property) / sizeof(Property);
    for (int i = 0; i < count_to_write; i++) {
        if (!update_property_by_subject(cursor, &(old_property[i]), &(new_property[i]))) {
            print_test_format("Fail to update property %i", i);
            return false;
        }; 
    }

    Iterator* iterator = select_property_by_subject(cursor, &(old_property[0]));
    if (!has_next(iterator)) {
        free_iter(iterator);
        print_test_format("Fail, not found updated property");
        return false;
    }

    Property* prop = (Property*) next(iterator);
    if (*((uint32_t*)prop->value) != *((uint32_t*)new_property[0].value)) {
        free_iter(iterator);
        print_test_format("Fail, wasn't updated property");
        return false;
    }
    return true;
}

void select_node(Cursor* cursor) {
    Entity data[] = {
        {NODE,  VOID, 0, 0, 0, "pc"},
        {NODE,  VOID, 0, 0, 0, "phone"},
        {NODE, VOID,  0, 0, 0, "person"},
        {NODE,  VOID, 0, 0, 0, "car"},
    };
    Node node = {0, 2, "l", "person"};
    for (int i = 0; i < 4; i++) {
        strcpy(node.type, data[i].type);
        Iterator* iterator = select_all_nodes(cursor, &(node));
        while (has_next(iterator)) {
            print_node(next(iterator));
        }
    }
}

void select_relationship(Cursor* cursor) {
    Relationship rel = {0, 2, 0, "", "", ""};
    for (int i = 0; i < 4; i++) {
        Iterator* iterator = select_all_relationships(cursor, &(rel));
        while (has_next(iterator)) {
            print_relationship(next(iterator));
        }
    }
}

void select_property(Cursor* cursor) {
    Entity data[] = {
        {PROPERTY, INT,  0, 0,0,"age"},
        {PROPERTY, STRING,  0, 0,0,"color"},
    };
    Property pr = {0, 2, INT, "", "", ""};
    for (int i = 0; i < 2; i++) {
        strcpy(pr.type, data[i].type);
        Iterator* iterator = select_all_properties(cursor, &(pr));
        while (has_next(iterator)) {
            print_property(next(iterator));
        }
    }
}

void select_properties_by_node_t(Cursor* cursor) {
    Node node = {0, 0, "person", ""};

    EntityIterator* entity_iterator = select_properties_by_node(cursor, &(node));

    while (entity_iterator != NULL && entity_has_next(entity_iterator)) {
        print_property(entity_next(entity_iterator));
    }
}

void select_relationships_by_node_t(Cursor* cursor) {
    Node node = {0, 0, "person", ""};

    EntityIterator* entity_iterator = select_relationships_by_node(cursor, &(node));
    
    while (entity_iterator != NULL && entity_has_next(entity_iterator)) {
        print_relationship(entity_next(entity_iterator));
    }
}

bool create_test(Cursor* cursor) {
    print_test_format("Start to create");
    bool result = create_entity(cursor) &&
        create_nodes(cursor) &&
        create_relationships(cursor) &&
        create_properties(cursor) &&
        
        create_entity_1(cursor) &&
        create_nodes_1(cursor) &&
        create_relationships_1(cursor);
    if (result) {
        print_test_format("Created successfuly");
    } else {
        print_test_format("Failed to create elments");
    }
    return result;
}

bool delete_test(Cursor* cursor) {
    print_test_format("Start to delete");
    bool result = delete_nodes(cursor);
    print_test_format("Delete Node");
    result = result && delete_relationships(cursor);
    print_test_format("Delete Relationships");
    result = result && delete_properties(cursor);
    print_test_format("Delete Property");
    result = result && delete_entity(cursor);
    if (result) {
        print_test_format("Deleted successfuly");
    } else {
        print_test_format("Failed to delete");
    }
    return result;
}

bool update_test(Cursor* cursor) {
    print_test_format("Start to update");
    bool result = update_nodes(cursor) &&
        update_relationships(cursor) &&
        update_properties(cursor);

    if (result) {
        print_test_format("Updated successfuly");
    } else {
        print_test_format("Failed to update");
    }
    return result;
}

void select_test(Cursor* cursor) {
    select_node(cursor);
    select_relationship(cursor);
    select_property(cursor);
    select_properties_by_node_t(cursor);
    select_relationships_by_node_t(cursor);
}

static void prepare_smoke(Cursor* cursor) {
    Entity data[] = {
        {NODE, 0, 0, 0, 0, "person"},
        {NODE, 0, 0, 0, 0, "pc"},
        // {NODE, 0, 0, 0, 0, "laba"},
        // {NODE, 0, 0, 0, 0, "unic"},
        // {NODE, 0, 0, 0, 0, "human"},
        // {NODE, 0, 0, 0, 0, "mac"},
        // {NODE, 0, 0, 0, 0, "io"},
        // {NODE, 0, 0, 0, 0, "seque"},
        // {NODE, 0, 0, 0, 0, "read"},
        // {NODE, 0, 0, 0, 0, "write"},
        // {NODE, 0, 0, 0, 0, "some"},
        // {NODE, 0, 0, 0, 0, "idea"},
        // {NODE, 0, 0, 0, 0, "watch"},
        // {NODE, 0, 0, 0, 0, "clock"},
        // {NODE, 0, 0, 0, 0, "phone"},
        // {NODE, 0, 0, 0, 0, "build"},
    };

    int count_to_write = sizeof(data) / ENTITY_SIZE;
    for (int i = 0; i < count_to_write; i++) {
        create_type(cursor, &(data[i]));
    }
}

static void create_node_smoke(Cursor* cursor, int num) {
    char* types[] = {
        "person", 
        "pc",
        "laba",
        "unic",
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
        "build"
    };

    Node *node = (Node*) malloc(NODE_SIZE);
    char* name = malloc(11 * CHAR_SIZE);

    clock_t begin = clock();
    
    for (int j = 0; j < 500 * num; j++) {
        int r = rand() % 15;

        strcpy(name, "sasaovchsa");
        strcpy(node->type, types[r]);

        node->id = 0;
        node->name_length = 11;
        node->name = name;
        uint32_t* id = create_node(cursor, node);
        if (id == NULL) continue;
        free(id);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    // println("%llu", num * 500);
    printf("%f\n", time_spent);

    free(node);
    free(name);
}

void select_smoke_test(Cursor* cursor) {
    prepare_smoke(cursor);
    for (int i = 0; i < 2000000000; i++) {
        create_node_smoke(cursor, i);
    }
}

void delete_smoke_test(Cursor* cursor) {
    prepare_smoke(cursor);

    char* types[] = {
        "person", 
        "pc",
        "laba",
        "unic",
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
        "build"
    };
    Node *node = (Node*) malloc(NODE_SIZE);
    char* name = malloc(11 * CHAR_SIZE);

    for (int i = 0; i < 2000000000; i++) {

        for (int j = 0; j < 1 * i; j++) {
            int r = rand() % 2;

            strcpy(name, "sasaovch");
            strcpy(node->type, types[r]);

            node->id = 0;
            node->name_length = 11;
            node->name = name;
            uint32_t* id = create_node(cursor, node);

            if (id == NULL) continue;
            free(id);
        }
        if (i == 25) {
            println("");
        }
        clock_t begin = clock();
        delete_all_nodes(cursor, node);
        clock_t end = clock();

        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("%f\n", time_spent);
    }
    free(node);
    free(name);
}

void update_smoke_test(Cursor* cursor) {
    prepare_smoke(cursor);

    char* types[] = {
        "person", 
        "pc",
        "laba",
        "unic",
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
        "build"
    };
    Node *node = (Node*) malloc(NODE_SIZE);
    char* name = malloc(11 * CHAR_SIZE);

    for (int i = 0; i < 2000000000; i++) {

    
        for (int j = 0; j < 100; j++) {
            int r = rand() % 15;

            strcpy(name, "sasaovchsa");
            strcpy(node->type, types[r]);

            node->id = 0;
            node->name_length = 11;
            node->name = name;
            uint32_t* id = create_node(cursor, node);
            if (id == NULL) continue;
            free(id);
        }

        clock_t begin = clock();
        update_nodes(cursor);
        clock_t end = clock();

        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("%f\n", time_spent);
    }
    free(node);
    free(name);
}

void select_delete_smoke_test(Cursor* cursor) {
    Entity data = {NODE, 0, 0, 0, 0, "person"};
    create_type(cursor, &(data));

    Node *node = (Node*) malloc(NODE_SIZE);
    char* name = malloc(11 * CHAR_SIZE);

    strcpy(name, "sasaovch");
    strcpy(node->type, "person");

    node->id = 0;
    node->name_length = 11;
    node->name = name;

    for (int i = 0; i < INT32_MAX; i++) {

        clock_t begin = clock();
    
        for (int j = 0; j < 500; j++) {
            uint32_t* id = create_node(cursor, node);
            if (id == NULL) continue;
            free(id);
        }

        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("%f\n", time_spent);

        clock_t begin_2 = clock();

        node->id += 200;
        
        for (int j = 0; j < 200; j++) {
            delete_nodes_greater_id(cursor, node);
        }
    
        clock_t end_2 = clock();
        double time_spent_2 = (double)(end_2 - begin_2) / CLOCKS_PER_SEC;
        (void) time_spent_2;
        // printf("%f\n", time_spent_2);
    }
    free(node);
    free(name);
}

void create_n_nodes_(Cursor* cursor, int n) {
    Entity data = {NODE, 0, 0, 0, 0, "person"};
    create_type(cursor, &(data));

    Node *node = (Node*) malloc(NODE_SIZE);
    char* name = malloc(11 * CHAR_SIZE);

    strcpy(name, "sasaovch");
    strcpy(node->type, "person");

    node->id = 0;
    node->name_length = 11;
    node->name = name;

    for (int i = 0; i < n; i++) {
        uint32_t* id = create_node(cursor, node);
        if (id == NULL) continue;
        free(id);
    }
    free(node);
    free(name);
}
