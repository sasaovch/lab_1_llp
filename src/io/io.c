#ifndef LAB_1_IO
#define LAB_1_IO

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


#include "../data/data.c"

void println(const char *line, ...) {
    va_list args;
    va_start(args, line);
    vprintf(line, args);
    printf("\n");
    va_end(args);
}

void error_exit(int var, const char *line, ...) {
    if (var == -1) {
        va_list args;
        va_start(args, line);
        vprintf(line, args);
        printf("\n");
        va_end(args);
        exit(EXIT_FAILURE);
    }
}

void debug(int num, const char *line, ...) {
    println("Debug %i", num);
    va_list args;
    va_start(args, line);
    vprintf(line, args);
    printf("\n");
    va_end(args);
}

void read_from_file(File* file, void* read_buf, uint64_t size) {
    ssize_t bytes_read = read(file->file_descriptor, read_buf, size);
    error_exit(bytes_read, "Error reading from file %s", file->file_descriptor);
}

uint32_t find_last_entity(uint64_t file_length, Page* page, File* file) {
    if (file_length == 0) {
        return 0;
    }

    uint32_t last_block_number = 0;
    do {
        last_block_number = page->page_header->next_block;
        lseek(file->file_descriptor, page->page_header->next_block * PAGE_SIZE, SEEK_SET);
        read_from_file(file, page->page_header, PAGE_HEADER_SIZE);
    } while (page->page_header->next_block != 0);

    read_from_file(file, page->body, PAGE_BODY_SIZE);
    return last_block_number;
}

Cursor* db_open(const char* filename) {
    int fd = open(filename,
                O_RDWR |      // Read/Write mode
                    O_CREAT,  // Create file if it does not exist
                S_IWUSR |     // User write permission
                    S_IRUSR   // User read permission
    );

    error_exit(fd, "Unabled to open file");
    off_t file_length = lseek(fd, 0, SEEK_END); // find end of file
    error_exit(file_length, "Failed to find end of file");
    
    File* file = (File*) malloc(sizeof(File));
    PageHeader* pageHeader = (PageHeader*) malloc(sizeof(PageHeader));
    Page* page = (Page*) malloc(sizeof(Page));
    Cursor* cursor = (Cursor*) malloc(sizeof(Cursor));

    void* page_body = malloc(PAGE_BODY_SIZE);

    file->file_descriptor = fd;
    file->file_length = file_length;

    page->body = page_body;
    page->page_header = pageHeader;

    cursor->file = file;
    cursor->last_entity_block = find_last_entity(file_length, page, file);
    cursor->number_of_pages = file_length / PAGE_SIZE;
    cursor->page = page;

    return cursor;
}

void set_pointer_offset_file(File* file, uint64_t offset) {
    off_t offs = lseek(file->file_descriptor, offset, SEEK_SET);
    error_exit(offs, "Error seeking offset");
}

void write_to_file(File* file, void* write_buf, uint64_t size) {
    ssize_t bytes_written = write(file->file_descriptor, write_buf, size);
    error_exit(bytes_written, "Error writing to file");
}

uint64_t get_page_offset(Cursor* cursor) {
    return cursor->page->page_header->block_number * PAGE_SIZE;
}

void flush_page(Cursor* cursor) {
    set_pointer_offset_file(cursor->file, get_page_offset(cursor));
    write_to_file(cursor->file, cursor->page->page_header, PAGE_HEADER_SIZE);
    write_to_file(cursor->file, cursor->page->body, PAGE_BODY_SIZE);
}

void db_close(Cursor* cursor) {
    close(cursor->file->file_descriptor);
    free(cursor->file);
    free(cursor->page->page_header);
    free(cursor->page->body);
    free(cursor->page);
    free(cursor);
}

#endif