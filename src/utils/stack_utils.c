#include "../../include/utils/stack_utils.h"

#include "../../include/utils/page_utils.h"
#include "data/constants.h"
#include "data/page.h"
#include "data/stack.h"
#include "managers/page_manager.h"

#include <stdint.h>
#include <string.h>

void prepare_stack(Cursor *cursor) {
    Stack *stack = new_stack(256);
    cursor->empty_pages = stack;
    
    if (cursor->file->file_length == 0) {
        Page *page = new_page();
        page->page_header->block_number = 0;
        cursor->last_stack_page = 0;
        
        write_page_to_file_flush(cursor, page);
        stack->page = page;
    } else {
        uint32_t page_num = 0;
        uint32_t prev_num = 0;
        uint32_t* number = (uint32_t *) malloc(UINT32_T_SIZE);

        Page *page = NULL;
        do {
            page = read_page_from_file(cursor, page_num);
            PageHeader *page_header = page->page_header;
            char* page_body = page->page_body;

            for (uint32_t i = 0; i < (page_header->offset / UINT32_T_SIZE); i++) {
                memcpy(number, page_body + i * UINT32_T_SIZE, UINT32_T_SIZE);
                push(stack, *number);
            }
            page_num = page_header->next_block;
            if (page_num == 0) break;
        } while (page_num != 0);
        stack->page = page;
        cursor->last_stack_page = prev_num;
    }
}

void push_in_stack(Cursor *cursor, uint32_t value) {
    Stack *stack = cursor->empty_pages;

    if (stack->page->page_header->offset + UINT32_T_SIZE > PAGE_BODY_SIZE) {
        uint32_t last_number = INT32_MAX;
        if (not_empty(stack)) {
            last_number = pop(stack);
            push(stack, last_number);
        }

        Page *n_page = get_free_page(cursor, stack->page);
        if (n_page->page_header->block_number == last_number) {
            uint32_t zero = 0;
            memcpy(stack->page->page_body + stack->page->page_header->offset - UINT32_T_SIZE, &(zero), UINT32_T_SIZE);
            stack->page->page_header->offset -= UINT32_T_SIZE;
            stack->page->page_header->next_block = n_page->page_header->block_number;
        } else {
            stack->page->page_header->next_block = n_page->page_header->block_number;
            write_page_to_file_flush(cursor, stack->page);        
            memcpy_page(stack->page, n_page);
        }
        free_page(n_page);
    }
    memcpy(stack->page->page_body + stack->page->page_header->offset, &(value), UINT32_T_SIZE);
    stack->page->page_header->offset += UINT32_T_SIZE;
    write_page_to_file_flush(cursor, stack->page);
    push(stack, value);
}

uint32_t pop_from_stack(Cursor *cursor) {
    Stack *stack = cursor->empty_pages;
    if (stack->page->page_header->offset == 0) {
        uint32_t prev_page = find_page_before(cursor, stack->page->page_header->block_number, 0);
        
        Page *n_page = read_page_from_file(cursor, prev_page);
        memcpy_page(stack->page, n_page);
        free_page(n_page);
    }
    uint32_t zero = 0;
    memcpy(stack->page->page_body + stack->page->page_header->offset - UINT32_T_SIZE, &(zero), UINT32_T_SIZE);
    stack->page->page_header->offset -= UINT32_T_SIZE;
    return pop(stack);
}
