#include "../../include/utils/io_utils.h"

#include "../../include/managers/page_manager.h"
#include "../../include/data/constants.h"
#include "utils/checker.h"
#include "utils/logger.h"

#include <stdint.h>
#include <string.h>
//FIXME: good
uint32_t find_last_entity(const Cursor *cursor) {
    LOG_DEBUG("In find_last_entity", "");
    if (check_is_null_arg(cursor, "cursor")) {
        error_exit(ERROR_EXIT_CODE, "Some parameters are NULL in find_last_entity");
        return 0;
    }

    if (cursor->file->file_length == 0) {
        return 0;
    }

    uint32_t last_block_number = START_PAGE;
    Page *n_page = read_page_from_file(cursor, last_block_number);

    while (n_page->page_header->next_block != 0) {
        free_page(n_page);
        last_block_number = n_page->page_header->next_block;
        n_page = read_page_from_file(cursor, last_block_number);
    }

    return last_block_number;
}

Cursor *db_open(const char *filename) {
    bool file_exists = access(filename, F_OK) == 0;

    if (!file_exists) {
        println("File %s doesn't exist", filename);

        FILE *f = fopen(filename, "w");
        if (f == NULL) {
            println("Can't create file");
        }
        error_exit(fclose(f), "Error while closing file");
    }

    FILE *f = fopen(filename, "rb+");
    if (f == NULL) {
        error_exit(-1, "Unabled to open file");
    }

    uint64_t file_length = fseek(f, 0, SEEK_END);
    error_exit(file_length, "Failed to find end of file");
    
    File *file = (File*) malloc(sizeof(File));
    Cursor *cursor = (Cursor*) malloc(sizeof(Cursor));
    Stack *stack = new_stack(256);

    file->file = f;
    file->file_length = file_length;

    cursor->file = file;
    cursor->empty_pages = stack;
    cursor->last_entity_block = find_last_entity(cursor);

    if (file_length == 0) {
        cursor->number_of_pages = 0;
    } else {
        cursor->number_of_pages = (file_length - 1) / BLOCK_SIZE;
    }

    return cursor;
}

void update_page_header_offset(const Cursor *cursor, const Page *page) {
    uint64_t header_offset = page->page_header->block_number * BLOCK_SIZE + UINT32_T_SIZE * 2;
    set_pointer_offset_file(cursor->file, header_offset);
    write_uint_32_to_file(cursor->file, page->page_header->offset);
}

void write_empty_page(Cursor *cursor) {
    void *page = malloc(BLOCK_SIZE);
    PageHeader *page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);

    cursor->number_of_pages++;
    page_header->block_number = cursor->number_of_pages;
    
    memcpy(page, page_header, PAGE_HEADER_SIZE);
    
    set_pointer_offset_file(cursor->file, cursor->number_of_pages * BLOCK_SIZE);
    write_to_file(cursor->file, page, BLOCK_SIZE);

    free(page_header);
    free(page);
}

uint64_t get_page_offset(const Page *page) {
    return page->page_header->block_number * BLOCK_SIZE;
}

void write_page_to_file_flush(const Cursor *cursor, const Page *page) {
    set_pointer_offset_file(cursor->file, get_page_offset(page));
    write_to_file(cursor->file, page->page_header, PAGE_HEADER_SIZE);
    write_to_file(cursor->file, page->page_body, PAGE_BODY_SIZE);
    flush(cursor->file);
}

Page* get_free_page(Cursor *cursor, const Page *page) {
    LOG_DEBUG("In get_free_page", "");
    if (check_is_null_arg(cursor, "cursor") ||
        check_is_null_arg(cursor->empty_pages, "empty_pages")) {
            return NULL;
    }

    // если после этой страницы есть другая в списке, то берем ее
    if (page != NULL && page->page_header->next_block != 0) {
        uint64_t next_page = page->page_header->next_block;
        
        return read_page_from_file(cursor, next_page);
    }

    // если есть пустая страница в стеке
    if (not_empty(cursor->empty_pages)) {
        
        uint64_t next_page_number = pop(cursor->empty_pages);

        uint64_t page_offset = page->page_header->block_number * BLOCK_SIZE;
        set_pointer_offset_file(cursor->file, page_offset + UINT32_T_SIZE);
        write_uint_32_to_file(cursor->file, next_page_number);
        flush(cursor->file);

        return read_page_from_file(cursor, next_page_number);
    }
    
    //создаем пустую страницу и записываем ее в файл
    cursor->number_of_pages++;
    if (page != NULL) {
        uint64_t page_offset = page->page_header->block_number * BLOCK_SIZE;
        set_pointer_offset_file(cursor->file, page_offset + UINT32_T_SIZE);
        write_uint_32_to_file(cursor->file, cursor->number_of_pages);
    }

    Page *n_page = new_page();
    n_page->page_header->block_number = cursor->number_of_pages;

    write_page_to_file_flush(cursor, n_page);
    return n_page;
}

void db_close(Cursor *cursor) {
    fclose(cursor->file->file);
    free(cursor->file);
    destroy(cursor->empty_pages);
    free(cursor);
}

Page *read_page_from_file(const Cursor *cursor, uint32_t page_number) {
    if (check_is_null_arg(cursor, "cursor")) {
        return NULL;
    }
    set_pointer_offset_file(cursor->file, page_number * BLOCK_SIZE);
    
    Page *page = (Page *) malloc(PAGE_SIZE);
    if (!page) {
        LOG_DEBUG("Failed to allocate memory for the Page structure.", "");
        return NULL;
    }

    page->page_header = (PageHeader *) malloc(PAGE_HEADER_SIZE);
    if (!page->page_header) {
        LOG_DEBUG("Failed to allocate memory for the PageHeader.", "");
        free(page);
        return NULL;
    }
    read_from_file(cursor->file, page->page_header, PAGE_HEADER_SIZE);

    page->page_body = (char*) malloc(PAGE_BODY_SIZE);
    if (!page->page_body) {
        LOG_DEBUG("Failed to allocate memory for the body.", "");
        free(page->page_header);
        free(page);
        return NULL;
    }
    read_from_file(cursor->file, page->page_body, PAGE_BODY_SIZE);

    return page;
}

void memcpy_page(const Page *dist, const Page *source) {
    memcpy(dist->page_header, source->page_header, PAGE_HEADER_SIZE);
    memcpy(dist->page_body, source->page_body, PAGE_BODY_SIZE);
}

void write_page_to_file(const Cursor *cursor, const Page *page) {
    write_to_file(cursor->file, page->page_header, PAGE_HEADER_SIZE);
    write_to_file(cursor->file, page->page_body, PAGE_BODY_SIZE);
}

void write_page_to_file_with_offset(const Cursor *cursor, const Page *page, uint64_t offset) {
    set_pointer_offset_file(cursor->file, offset);
    write_page_to_file(cursor, page);
}
