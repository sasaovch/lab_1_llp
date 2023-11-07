#include "../../include/data/page.h"
#include "../../include/utils/io_utils.h"
#include "data/constants.h"
#include "utils/checker.h"
#include <stdlib.h>

Page *new_page(void) {
    Page *page = (Page *) malloc(PAGE_SIZE);
    page->page_body = (char *) calloc(1, PAGE_BODY_SIZE);
    page->page_header = (PageHeader *) malloc(PAGE_HEADER_SIZE);

    page->page_header->block_number = 0;
    page->page_header->next_block = 0;
    page->page_header->offset = 0;

    return page;
}

void print_page(const PageHeader *page_header) {
    if (check_is_null_arg(page_header, "page_header")) return;
    println("Header");
    println("Block - %i", page_header->block_number);
    println("Next - %i", page_header->next_block);
    println("Offset - %i", page_header->offset);
}

void free_page(Page *page) {
    if (page == NULL) return;
    if (page->page_header != NULL) free(page->page_header);
    if (page->page_body != NULL) free(page->page_body);
    free(page);
}
