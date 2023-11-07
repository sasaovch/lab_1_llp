#pragma once

#include "../managers/page_manager.h"
#include "../data/entity.h"

Stack *find_all_pages_to_delete(const Cursor *cursor, const Entity *entity);

void remove_pages(const Cursor *cursor, Stack *stack);

uint32_t find_last_page(const Cursor *cursor, uint32_t start_block);

uint32_t find_page_before(const Cursor *cursor, uint32_t goal_page, uint32_t start_page);

void write_big_string_to_file(Cursor *cursor, Page *page, Entity *entity, uint32_t length, const char *string);

void read_big_string_from_file(const Cursor *cursor, Page *page, char *string, uint32_t length, const uint64_t *offset);

