#include "./type.c"

bool return_true(void* d, void* n) {
    return true;
}

bool is_property_for_node(void* rel, void* nd) {
    Property* prop = (Property*) rel;
    Node* node = (Node*) nd;
    return strcmp(prop->subject, node->name) == 0;
}

bool is_relationship_for_node(void* rel, void* nd) {
    Relationship* prop = (Relationship*) rel;
    Node* node = (Node*) nd;
    return strcmp(prop->parent, node->name) == 0;
}

bool create_element(
        Cursor* cursor, void* element, 
        void (*write_to_file_element)(Cursor*, void *), const char* name_type, 
        TypeOfElement type, uint64_t (*get_size_of_element)(void *), 
        void (*set_id)(void*, uint32_t)
    ) {
    
    uint64_t pointer = 0;
    Entity* table = (Entity*) malloc(ENTITY_SIZE);
    table = get_entity(cursor, type, name_type, &(pointer), table);

    if (table == NULL) {
        println("Error to find Node with name %s", name_type);
        return false;
    }

    uint32_t page_num = table->last_block;
    uint64_t page_offset = page_num * PAGE_SIZE;

    PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);

    set_pointer_offset_file(cursor->file, page_offset);
    read_from_file(cursor->file, page_header, PAGE_HEADER_SIZE);

    uint64_t global_offset = page_offset + page_header->offset + PAGE_HEADER_SIZE;

    if (get_size_of_element(element) > PAGE_BODY_SIZE) {
        println("Element is too big");
        return false;
    }

    if (page_header->offset + get_size_of_element(element) > PAGE_BODY_SIZE) {
        create_new_page(cursor, page_header, page_offset);
        
        table->last_block = cursor->number_of_pages;
        global_offset = cursor->number_of_pages * PAGE_SIZE + PAGE_HEADER_SIZE;
    }

    set_id(element, table->next_id);
    table->next_id += 1;
    
    set_pointer_offset_file(cursor->file, global_offset);
    write_to_file_element(cursor, element);

    uint64_t header_offset = global_offset - page_header->offset - PAGE_HEADER_SIZE + UINT32_T_SIZE * 2;
    page_header->offset += get_size_of_element(element);

    set_pointer_offset_file(cursor->file, header_offset);
    write_to_file(cursor->file, &(page_header->offset), UINT32_T_SIZE);

    set_pointer_offset_file(cursor->file, pointer + TYPE_OF_ELEMENT_SIZE + UINT32_T_SIZE);
    write_to_file(cursor->file, &(table->last_block), UINT32_T_SIZE);
    write_to_file(cursor->file, &(table->next_id), sizeof(uint32_t));

    free(table);
    free(page_header);
    return true;
}

void* find_element(
        Cursor* cursor, Entity* meta_page, 
        Page* page, uint64_t size_of_element, 
        void* find_elem, void* (*read_element)(void*, Entity*, void*, uint64_t*), 
        bool (*comparator)(void*, void*), uint64_t* offset_
    ) {
    uint32_t read_block = meta_page->first_block;
    PageHeader* page_header = page->page_header;
    void* body = page->body;
    void* element = (void*) malloc(size_of_element);

    while (read_block != 0) {
        uint64_t offset = 0;
        uint64_t page_offset = read_block * PAGE_SIZE;
        
        set_pointer_offset_file(cursor->file, page_offset);
        read_from_file(cursor->file, page_header, PAGE_HEADER_SIZE);  
        read_from_file(cursor->file, body, PAGE_BODY_SIZE);
        while (offset < page_header->offset) {
            element = read_element(element, meta_page, body, &(offset));
            if (comparator(element, find_elem)) {
                page->body = body;
                page->page_header = page_header;
                *offset_ = offset;
                return element;
            }
        }
        read_block = page_header->next_block;
    }
    free(element);
    return NULL;
}

void* get_elements_by_condition(
        Cursor* cursor, Entity* meta_page, 
        uint64_t size_of_element, 
        void* helper, void* (*read_element)(void*, Entity*, void*, uint64_t*), 
        bool (*condition)(void*, void*), uint64_t (*get_size_of_element)(void*),
        void (*write_element)(void*, Entity*, void*, uint64_t*)
    ) {
    uint32_t read_block = meta_page->first_block;
    uint64_t stack_offset = UINT32_T_SIZE;
    uint32_t find_number = 0;

    PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    uint64_t stack_size = PAGE_BODY_SIZE;
    void* body = malloc(PAGE_BODY_SIZE);
    void* stack = malloc(PAGE_BODY_SIZE);
    void* element = (void*) malloc(size_of_element);

    while (read_block != 0) {
        uint64_t offset = 0;
        uint64_t page_offset = read_block * PAGE_SIZE;

        set_pointer_offset_file(cursor->file, page_offset);
        read_from_file(cursor->file, page_header, PAGE_HEADER_SIZE);  
        read_from_file(cursor->file, body, PAGE_BODY_SIZE);

        while (offset < page_header->offset) {
            element = read_element(element, meta_page, body, &(offset));
            if (condition(element, helper)) {
                find_number++;
                uint64_t size = get_size_of_element(element);

                if (stack_offset + get_size_of_element(element) >= stack_size) {
                    stack_size += PAGE_BODY_SIZE;
                    stack = realloc(stack, stack_size);
                }
                write_element(element, meta_page, stack, &(stack_offset));
            }
        }
        read_block = page_header->next_block;
    }
    memcpy(stack, &(find_number), UINT32_T_SIZE);

    free(element);
    free(page_header);
    free(body);
    return stack;
}

void delete_element(
        Cursor* cursor, void* element, 
        uint64_t (*get_size_of_element)(void *), 
        uint64_t size_of_sturcture, void* name_type, TypeOfElement type, 
        void* (*read_element)(void*, Entity*, void*, uint64_t*), 
        bool (*comparator)(void*, void*)
    ) {
    uint64_t pointer = 0;
    uint64_t offset = 0;

    Entity* table = (Entity*) malloc(ENTITY_SIZE);
    table = get_entity(cursor, type, name_type, &(pointer), table);
    if (table == NULL) {
        println("Error to find Node with name %s", name_type);
        return;
    }

    void* new_body = malloc(PAGE_BODY_SIZE);
    Page* page = (Page*) malloc(sizeof(Page));
    PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    void* body = malloc(PAGE_BODY_SIZE);
    page->page_header = page_header;
    page->body = body;
    void* find_el = find_element(cursor, table, page, size_of_sturcture, element, read_element, comparator, &(offset));
    while (find_el != NULL) {
        if (find_el == NULL) {
            println("Can't find element %s", name_type);
            return;
        }
        offset -= get_size_of_element(find_el);

        memcpy(new_body, page->body, offset);
        memcpy(new_body + offset, page->body + offset + get_size_of_element(find_el), PAGE_BODY_SIZE - offset - get_size_of_element(find_el));

        uint64_t new_offset = page->page_header->offset - get_size_of_element(find_el);
        
        set_pointer_offset_file(cursor->file, page->page_header->block_number * PAGE_SIZE + PAGE_HEADER_SIZE - UINT32_T_SIZE);
        write_to_file(cursor->file, &(new_offset), UINT32_T_SIZE);
        write_to_file(cursor->file, new_body, PAGE_BODY_SIZE);
        
        if (new_offset == 0) {
            cut_blocks(cursor, page->page_header, &(pointer), table);
        }
        free(find_el);
        find_el = find_element(cursor, table, page, size_of_sturcture, element, read_element, comparator, &(offset));
    }

    free(page->page_header);
    free(page->body);
    free(page);
    free(new_body);
}

void update_element(
        Cursor* cursor, void* old_element, 
        void* new_element, uint64_t (*get_size_of_element)(void *), 
        uint64_t size_of_sturcture, void *name_type, TypeOfElement type, 
        void *(*read_element)(void *, Entity *, void *, uint64_t *), 
        bool (*comparator)(void *, void *), 
        void (*write_to_file_element)(Cursor *, void *), 
        void (*set_id)(void*, uint32_t)
    ) {
    delete_element(cursor, old_element, get_size_of_element, size_of_sturcture, name_type, type, read_element, comparator);
    create_element(cursor, new_element, write_to_file_element, name_type, type, get_size_of_element, set_id);
}