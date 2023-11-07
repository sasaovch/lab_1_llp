#include "../include/managers/file_manager.h"

#include "../include/utils/io_utils.h"
#include "../include/data/constants.h"
#include "data/property.h"
//FIXME: good
void read_from_file(const File *file, void *read_buf, uint64_t size) {
    uint32_t bytes_read = fread(read_buf, size, 1, file->file);
    error_exit(bytes_read, "Error reading from file");
}

void flush(const File *file) {
    fflush(file->file);
}

void set_pointer_offset_file(const File *file, uint64_t offset) {
    uint64_t offs = fseek(file->file, offset, SEEK_SET);
    error_exit(offs, "Error seeking offset");
}

void write_to_file(const File *file, const void *write_buf, uint64_t size) {
    uint32_t bytes_written = fwrite(write_buf, size, 1, file->file);
    error_exit(bytes_written, "Error writing to file");
}

void write_uint_32_to_file(const File *file, uint32_t number) {
    write_to_file(file, &(number), UINT32_T_SIZE);
}

void write_type_to_file(const File *file, const char *type) {
    write_to_file(file, type, NAME_TYPE_LENGTH + 1);
}

void write_string_to_file(const File *file, const char *string, uint32_t length) {
    write_to_file(file, string, CHAR_SIZE * length);
}
