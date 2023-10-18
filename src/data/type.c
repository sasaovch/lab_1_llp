#include "../../include/data/type.h"
void print_type(Entity* meta_page) {
    println("Print");
    println("Type - %i", meta_page->type);
    println("First - %i", meta_page->first_block);
    println("Last - %i", meta_page->last_block);
    println("Next Id - %i", meta_page->next_id);
    println("Name - %s", meta_page->name_type);
}

void save_meta_page_and_create_new(Cursor* cursor) {
    cursor->number_of_pages++;
    cursor->last_entity_block = cursor->number_of_pages;
    cursor->page->page_header->next_block = cursor->last_entity_block;
    
    flush_page(cursor);
    
    free(cursor->page->page_header);
    free(cursor->page->body);
    free(cursor->page);

    PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    Page* page = (Page*) malloc(PAGE_SIZE);
    
    page_header->block_number = cursor->last_entity_block;
    page->page_header = page_header;
    page->body = malloc(PAGE_BODY_SIZE);;
    cursor->page = page;
}

void create_page_for_type(Cursor* cursor) {
    void* page_for_type = malloc(PAGE_SIZE);
    PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);

    cursor->number_of_pages++;
    page_header->block_number = cursor->number_of_pages;
    
    memcpy(page_for_type, page_header, PAGE_HEADER_SIZE);
    
    set_pointer_offset_file(cursor->file, cursor->number_of_pages * PAGE_SIZE);
    write_to_file(cursor->file, page_for_type, PAGE_SIZE);

    free(page_header);
    free(page_for_type);
}

//FIXME: good
Entity* get_entity(Cursor* cursor, TypeOfElement type, const char* name, uint64_t* pointer, Entity* meta_page) {
    PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    char* page_body = (char*) malloc(PAGE_BODY_SIZE);
    
    int page_num = 0;

    // off_t file_length = lseek(cursor->file->file_descriptor, 0, SEEK_END); // find end of file
    // error_exit(file_length, "Failed to find end of file");
    do {
        set_pointer_offset_file(cursor->file, page_num * PAGE_SIZE);

        read_from_file(cursor->file, page_header, PAGE_HEADER_SIZE);        
        read_from_file(cursor->file, page_body, PAGE_BODY_SIZE);

        
        for (uint64_t i = 0; i < (page_header->offset / ENTITY_SIZE); i++) {
            memcpy(meta_page, page_body + i * ENTITY_SIZE, ENTITY_SIZE);
            if (strcmp(meta_page->name_type, name) == 0 && type == meta_page->type) {
                *pointer = page_num * PAGE_SIZE + i * ENTITY_SIZE + PAGE_HEADER_SIZE;
                free(page_header);
                free(page_body);
                return meta_page;
            }
        }
        page_num = page_header->next_block;
    } while (page_num != 0);
    
    free(page_header);
    free(page_body);
    return NULL;
}

bool create_type(Cursor* cursor, Entity* meta_page) {
    uint64_t pointer = 0;
    Entity* table = (Entity*) malloc(ENTITY_SIZE);
    table = get_entity(cursor, meta_page->type, meta_page->name_type, &(pointer), table);
    if (table != NULL) {
        println("%i with name %s already exists", meta_page->type, meta_page->name_type);
        return false;
    }

    if (cursor->page->page_header->offset + ENTITY_SIZE > PAGE_BODY_SIZE) {
        save_meta_page_and_create_new(cursor);
    }

    create_page_for_type(cursor);
    
    meta_page->next_id = 0;
    meta_page->first_block = cursor->number_of_pages;
    meta_page->last_block = cursor->number_of_pages;
    
    memcpy(cursor->page->body + cursor->page->page_header->offset, meta_page, ENTITY_SIZE);
    cursor->page->page_header->offset += ENTITY_SIZE;
    free(table);
    println("table");
    return true;
}

bool delete_type(Cursor* cursor, Entity* meta_page) {
    uint64_t* pointer = (uint64_t*) malloc(sizeof(uint64_t));
    *pointer = 0;
    PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    Entity* delete_table = (Entity*) malloc(ENTITY_SIZE);
    delete_table = get_entity(cursor, meta_page->type, meta_page->name_type, pointer, delete_table);

    if (delete_table == NULL) {
        println("Error to find Node with name %s", meta_page->name_type);
        return false;
    }

    uint32_t counter = 0;
    uint32_t *stack = find_all_blocks_to_delete(cursor, &(counter), delete_table);

    remove_blocks(cursor, counter, stack);
    uint64_t new_offset = erase_entity(cursor, pointer);

    if (new_offset == 0) {
        page_header = move_blocks_higher(cursor, pointer, page_header);
        remove_emtpy_blocks(cursor, page_header);
    }
    free(pointer);
    free(stack);
    free(page_header);
    free(delete_table);
    return true;
}

bool update_type(Cursor* cursor, Entity* old_meta_page, Entity* new_meta_page) {
    uint64_t* pointer = (uint64_t*) malloc(sizeof(uint64_t));
    *pointer = 0;
    Entity* table = (Entity*) malloc(ENTITY_SIZE);
    table = get_entity(cursor, old_meta_page->type, old_meta_page->name_type, pointer, table);
    if (table == NULL) {
        println("Error to find Node with name %s", old_meta_page->name_type);
        return false;
    }

    if (table->type != new_meta_page->type) {
        println("Forbidden to change type of meta table");
        return false;
    }

    new_meta_page->first_block = table->first_block;
    new_meta_page->last_block = table->last_block;
    new_meta_page->next_id = table->next_id;

    set_pointer_offset_file(cursor->file, *pointer);
    write_to_file(cursor->file, new_meta_page, ENTITY_SIZE);

    free(pointer);
    free(table);
    return true;
}
