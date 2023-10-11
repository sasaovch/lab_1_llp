#include "../io/io.c"

void print_relationship(Relationship* relationship) {
    println("Print relationship");
    println("Id - %llu", relationship->id);
    println("Type - %s", relationship->name_type);
    println("Parent length - %i", relationship->parent_length);
    println("Parent - %s", relationship->parent);
    println("Child length - %i", relationship->child_length);
    println("Child - %s", relationship->child);
}

void write_relationship_to_file(Cursor* cursor, void* rl) {
    Relationship* relationship = (Relationship*) rl;

    write_to_file(cursor->file, &(relationship->id), UINT32_T_SIZE);
    write_to_file(cursor->file, &(relationship->parent_length), UINT32_T_SIZE);
    write_to_file(cursor->file, relationship->parent, CHAR_SIZE * relationship->parent_length);
    write_to_file(cursor->file, &(relationship->child_length), UINT32_T_SIZE);
    write_to_file(cursor->file, relationship->child, CHAR_SIZE * relationship->child_length);
}

uint64_t get_size_of_relationship(void* rl) {
    Relationship* relationship = (Relationship*) rl;
    return UINT32_T_SIZE + UINT32_T_SIZE * 2 + CHAR_SIZE * (relationship->child_length + relationship->parent_length);
}

bool compare_relationship(void* rl_1, void* rl_2) {
    Relationship* relationship_1 = (Relationship*) rl_1;
    Relationship* relationship_2 = (Relationship*) rl_2;
    return (strcmp(relationship_1->parent, relationship_2->parent) == 0) && (
        relationship_1->name_type == relationship_2->name_type) == 0 && 
        (strcmp(relationship_1->child, relationship_2->child) == 0);
}

bool compare_parent_relationship(void* rl_1, void* rl_2) {
    Relationship* relationship_1 = (Relationship*) rl_1;
    Relationship* relationship_2 = (Relationship*) rl_2;
    return (strcmp(relationship_1->parent, relationship_2->parent) == 0);
}

bool compare_child_relationship(void* rl_1, void* rl_2) {
    Relationship* relationship_1 = (Relationship*) rl_1;
    Relationship* relationship_2 = (Relationship*) rl_2;
    return (strcmp(relationship_1->child, relationship_2->child) == 0);
}

bool compare_id_relationship(void* rl_1, void* rl_2) {
    Relationship* relationship_1 = (Relationship*) rl_1;
    Relationship* relationship_2 = (Relationship*) rl_2;
    return relationship_1->id == relationship_2->id;
}

void* read_relationship(void* element, Entity* meta_page, void* body, uint64_t* offset) {
    Relationship* relationship = (Relationship*) element;
    strcpy(relationship->name_type, meta_page->name_type);

    uint32_t id = 0;
    uint32_t parent_length = 0;
    uint32_t child_length = 0;

    memcpy(&(id), body + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(&(parent_length), body + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    char* parent = malloc(CHAR_SIZE * parent_length);
    memcpy(parent, body + *offset, CHAR_SIZE * parent_length);
    *offset += CHAR_SIZE * parent_length;
    
    memcpy(&(child_length), body + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    char* child = malloc(CHAR_SIZE * child_length);
    memcpy(child, body + *offset, CHAR_SIZE * child_length);
    *offset += CHAR_SIZE * child_length;
    
    relationship->id = id;
    relationship->parent_length = parent_length;
    relationship->parent = parent;
    relationship->child_length = child_length;
    relationship->child = child;

    return relationship;
}

void memcpy_relationship(void* element, Entity* meta_page, void* stack, uint64_t* offset) {
    Relationship* relationship = (Relationship*) element;

    uint32_t parent_length = relationship->parent_length;
    uint32_t child_length = relationship->child_length;

    memcpy(stack + *offset, &(relationship->id), UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(stack + *offset, &(parent_length), UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(stack + *offset, relationship->parent, CHAR_SIZE * parent_length);
    *offset += CHAR_SIZE * parent_length;
    
    memcpy(stack + *offset, &(child_length), UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(stack + *offset, relationship->child, CHAR_SIZE * child_length);
    *offset += CHAR_SIZE * child_length;
}

void* memget_relationship(void* element, Entity* meta_page, void* stack, uint64_t* offset) {
    Relationship* relationship = (Relationship*) element;
    strcpy(relationship->name_type, meta_page->name_type);

    uint32_t id = 0;
    uint32_t parent_length = 0;
    uint32_t child_length = 0;

    memcpy(&(id), stack + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    memcpy(&(parent_length), stack + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    char* parent = malloc(CHAR_SIZE * parent_length);
    memcpy(parent, stack + *offset, CHAR_SIZE * parent_length);
    *offset += CHAR_SIZE * parent_length;
    
    memcpy(&(child_length), stack + *offset, UINT32_T_SIZE);
    *offset += UINT32_T_SIZE;

    char* child = malloc(CHAR_SIZE * child_length);
    memcpy(child, stack + *offset, CHAR_SIZE * child_length);
    *offset += CHAR_SIZE * child_length;
    
    relationship->id = id;
    relationship->parent_length = parent_length;
    relationship->parent = parent;
    relationship->child_length = child_length;
    relationship->child = child;
    print_relationship(relationship);

    return relationship;
}

void set_relation_id(void* nd, uint32_t id) {
    Relationship* node = (Relationship*) nd;
    node->id = id;
}