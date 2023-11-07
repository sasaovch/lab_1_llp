#pragma once

#include "../data/file.h"

void read_from_file(const File *file, void *read_buf, uint64_t size);

void set_pointer_offset_file(const File *file, uint64_t offset);

void write_to_file(const File *file, const void *write_buf, uint64_t size);

void write_uint_32_to_file(const File *file, uint32_t number);

void write_type_to_file(const File *file, const char *type);

void write_string_to_file(const File *file, const char *string, uint32_t length);

void truncate_file(const File *file, uint64_t offset);

void flush(const File *file);
