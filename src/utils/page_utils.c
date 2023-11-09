#include "../../include/utils/page_utils.h"

#include "../../include/utils/stack_utils.h"

#include "utils/checker.h"
#include "utils/logger.h"

#include <string.h>

Stack *find_all_pages_to_delete(const Cursor *cursor, const Entity *entity) {
    LOG_DEBUG("In find_all_pages_to_delete", "");
    if (check_is_null_arg(cursor, "cursor") ||
        check_is_null_arg(entity, "entity")) {
            return NULL;
    }
    Stack *stack = new_stack(1000);
    uint32_t read_block = entity->first_page;
    
    while (read_block != 0) {
        push(stack, read_block);
        Page *page = read_page_from_file(cursor, read_block);  
        read_block = page->page_header->next_block;
        free_page(page);
    }

    return stack;
}

void remove_pages(Cursor *cursor, Stack *stack) {
    LOG_DEBUG("In remove_pages", "");
    if (check_is_null_arg(cursor, "cursor") ||
        check_is_null_arg(stack, "stack")) {
            return;
    }
    uint32_t zero = 0;
    void *empty_body = calloc(1, PAGE_BODY_SIZE);
    uint32_t last_page = 0;

    if (not_empty(stack)) {
        uint32_t page_number = pop(stack);
        last_page = page_number;
        uint64_t page_offset = page_number * BLOCK_SIZE;

        set_pointer_offset_file(cursor->file, page_offset + UINT32_T_SIZE);
        write_to_file(cursor->file, &(zero), UINT32_T_SIZE);
        write_to_file(cursor->file, &(zero), UINT32_T_SIZE);
        write_to_file(cursor->file, empty_body, PAGE_BODY_SIZE);
    }
    
    while (not_empty(stack)) {
        uint32_t page_number = pop(stack);
        last_page = page_number;
        uint64_t page_offset = page_number * BLOCK_SIZE;

        set_pointer_offset_file(cursor->file, page_offset + PAGE_HEADER_SIZE - UINT32_T_SIZE);
        write_to_file(cursor->file, &(zero), UINT32_T_SIZE);
        write_to_file(cursor->file, empty_body, PAGE_BODY_SIZE);
    }
    push_in_stack(cursor, last_page);
    free(empty_body);
}

uint32_t find_page_before(const Cursor *cursor, uint32_t goal_page, uint32_t start_page) {
    LOG_DEBUG("In find_page_before", "");
    uint32_t prev_page = goal_page;
    uint32_t curr_page = start_page;
    PageHeader *curr_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    
    while (curr_page != goal_page && curr_page != 0) {

        set_pointer_offset_file(cursor->file, curr_page * BLOCK_SIZE);
        read_from_file(cursor->file, curr_header, PAGE_HEADER_SIZE);
        prev_page = curr_page;
        curr_page = curr_header->next_block;
    }

    free(curr_header);
    return prev_page;
}

uint32_t find_last_page(const Cursor *cursor, uint32_t start_block) {
    LOG_DEBUG("In find_last_page", "");
    uint32_t page_number = start_block;
    uint32_t read_page = start_block;
    PageHeader *page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    
    do {
        set_pointer_offset_file(cursor->file, read_page * BLOCK_SIZE);
        read_from_file(cursor->file, page_header, PAGE_HEADER_SIZE);
        page_number = page_header->block_number;
        read_page = page_header->next_block;
    } while (read_page != 0);
    
    free(page_header);
    return page_number;
}

void write_big_string_to_file(Cursor *cursor, Page *page, Entity *entity, uint32_t length, const char *string) {
    uint32_t string_offset = 0;
    char *string_to_write = (char*) calloc(1, PAGE_BODY_SIZE);

    Page *n_page = (Page *) new_page();
    memcpy_page(n_page, page);

    uint64_t page_address = n_page->page_header->block_number * BLOCK_SIZE;
    uint32_t page_offset = n_page->page_header->offset;

    while (length != 0) {
        uint32_t write_length = 0;
        if (length <= PAGE_BODY_SIZE - page_offset) {
            write_length = length;
        } else {
            write_length = PAGE_BODY_SIZE - page_offset;
        }

        memcpy(string_to_write, string + string_offset, write_length);
        write_string_to_file(cursor->file, string_to_write, write_length);

        length -= write_length;
        string_offset += write_length;
        page_offset += write_length;

        if (page_offset == PAGE_BODY_SIZE && length != 0) {
            n_page->page_header->offset = page_offset;
            update_page_header_offset(cursor, n_page);

            Page *nn_page = get_free_page(cursor, n_page);
            free(n_page);
            n_page = nn_page;
        
            entity->last_page = n_page->page_header->block_number;
            page_offset = 0;
            page_address = n_page->page_header->block_number * BLOCK_SIZE;
            uint32_t global_offset = page_address + PAGE_HEADER_SIZE;

            set_pointer_offset_file(cursor->file, global_offset);
        }
    }
    
    n_page->page_header->offset = page_offset;
    update_page_header_offset(cursor, n_page);
    free(string_to_write);
    memcpy_page(page, n_page);
}

void read_big_string_from_file(const Cursor *cursor, Page *page, char *string, uint32_t length, const uint64_t *offset_) {
    uint32_t to_read_length = 0;
    uint32_t string_offset = 0;
    
    uint64_t offset = *offset_;
    uint32_t read_block = page->page_header->block_number;

    Page *n_page = new_page();
    memcpy_page(n_page, page);
    
    while (length != 0) {

        if (length <= PAGE_BODY_SIZE - offset) {
            to_read_length = length;
        } else {
            to_read_length = PAGE_BODY_SIZE - offset;
        }
    
        memcpy(string + string_offset, n_page->page_body + offset, to_read_length);
        offset += to_read_length;
        string_offset += to_read_length;
        length -= to_read_length;

        if (offset == PAGE_BODY_SIZE && length != 0) {
            read_block = n_page->page_header->next_block;
            free_page(n_page);

            n_page = read_page_from_file(cursor, read_block);
            offset = 0;
        }
    }
    memcpy_page(page, n_page);
}
