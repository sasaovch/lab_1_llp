#pragma once

#include "../managers/page_manager.h"
#include "../managers/file_manager.h"
#include "../data/page.h"
#include "../data/cursor.h"
#include <stdint.h>

Cursor *db_open(const char *filename);

uint32_t find_last_entity(const Cursor *cursor);

void write_page_to_file_flush(const Cursor *cursor, const Page *page);

void db_close(Cursor *cursor);

void write_empty_page(Cursor *cursor);

Page *read_page_from_file(const Cursor *cursor, uint32_t page_number);

Page* get_free_page(Cursor *cursor, const Page *page);

void update_page_header_offset(const Cursor *cursor, const Page *page);

uint64_t get_page_offset(const Page *page);

void memcpy_page(const Page *dist, const Page *source);

void write_page_to_file(const Cursor *cursor, const Page *page);

void write_page_to_file_with_offset(const Cursor *cursor, const Page *page, uint64_t offset);
