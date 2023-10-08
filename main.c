#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

//FIXME: подумать о слишком больших данных

const uint32_t PAGE_SIZE = 48;
const uint32_t PAGE_HEADER_SIZE = 12;
const uint32_t PAGE_BODY_SIZE = PAGE_SIZE - PAGE_HEADER_SIZE;
const uint32_t NAME_TYPE_SIZE = 6;

typedef struct {
    int file_descriptor;
    uint32_t file_length;
} File;

typedef struct {
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
  NODE = 0,
  RELATIONSHIP = 1,
  PROPERTY = 2,
} TypeOfElement;

typedef struct {
    TypeOfElement type;
    uint32_t first_block;
    uint32_t last_block;
    char name_type[NAME_TYPE_SIZE + 1];
} MetaTable;


typedef struct {
    uint32_t block_number;
    uint32_t next_block;
    uint32_t offset;
} PageHeader;

typedef struct {
    void* body;
    PageHeader* page_header;
} Page;

typedef struct {
    File* file;
    uint32_t first_meta_table_block;
    uint32_t last_meta_table_block;
    uint32_t offset;
    uint32_t number_of_pages;
    Page* page;
} Cursor;

typedef struct {
    char name_type[NAME_TYPE_SIZE + 1];
    uint32_t name_length;
    char* name;
} Node;

typedef struct {
    char name_type[NAME_TYPE_SIZE + 1];
    uint32_t parent_length;
    char* parent;
    uint32_t child_length;
    char* child;
} Relationship;

typedef struct {
    uint32_t subject_length;
    char* subject;
    uint32_t key_length;
    char* key;
    uint32_t value_length;
    char* value;
} Property;

const uint32_t META_TABLE_SIZE = sizeof(MetaTable);

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

void read_from_file(File* file, void* read_buf, uint64_t size) {
    ssize_t bytes_read = read(file->file_descriptor, read_buf, size);
    error_exit(bytes_read, "Error reading from file %s", file->file_descriptor);
}

uint32_t find_last_meta_table(uint32_t file_length, Page* page, File* file) {
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
    cursor->last_meta_table_block = find_last_meta_table(file_length, page, file);
    cursor->offset = PAGE_HEADER_SIZE + page->page_header->offset;
    cursor->number_of_pages = file_length / PAGE_SIZE;
    cursor->page = page;

    return cursor;
}

uint32_t get_page_offset(Cursor* cursor) {
    return cursor->page->page_header->block_number * PAGE_SIZE;
}

void set_pointer_offset_file(File* file, uint64_t offset) {
    off_t offs = lseek(file->file_descriptor, offset, SEEK_SET);
    error_exit(offs, "Error seeking offset");
}

void write_to_file(File* file, void* write_buf, uint64_t size) {
    ssize_t bytes_written = write(file->file_descriptor, write_buf, size);
    error_exit(bytes_written, "Error writing to file");
}

void flush_page(Cursor* cursor) {
    set_pointer_offset_file(cursor->file, get_page_offset(cursor));

    write_to_file(cursor->file, cursor->page->page_header, PAGE_HEADER_SIZE);
    write_to_file(cursor->file, cursor->page->body, PAGE_BODY_SIZE);
}

void save_meta_page_and_create_new(Cursor* cursor) {
    cursor->number_of_pages++;
    cursor->last_meta_table_block = cursor->number_of_pages;
    cursor->page->page_header->next_block = cursor->last_meta_table_block;
    
    flush_page(cursor);
    
    free(cursor->page->page_header);
    free(cursor->page->body);
    free(cursor->page);

    PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    Page* page = (Page*) malloc(PAGE_SIZE);
    
    page_header->block_number = cursor->last_meta_table_block;
    page->page_header = page_header;
    page->body = malloc(PAGE_BODY_SIZE);;
    cursor->page = page;
}

void create_page_for_type(Cursor* cursor) {
    void* page_for_type = malloc(PAGE_SIZE);
    PageHeader* pageHeader = (PageHeader*) malloc(PAGE_HEADER_SIZE);

    cursor->number_of_pages++;
    pageHeader->block_number = cursor->number_of_pages;
    
    memcpy(page_for_type, pageHeader, PAGE_HEADER_SIZE);
    
    set_pointer_offset_file(cursor->file, cursor->number_of_pages * PAGE_SIZE);
    write_to_file(cursor->file, page_for_type, PAGE_SIZE);

    free(pageHeader);
    free(page_for_type);
}

void save_type(Cursor* cursor, MetaTable* meta_page) {
    // Если в текущем блоке не хватает места, то нужно сохранить блок в память и создать новый
    if (cursor->page->page_header->offset + META_TABLE_SIZE > PAGE_SIZE) {
        save_meta_page_and_create_new(cursor);
    }

    // create page for type
    create_page_for_type(cursor);
    
    meta_page->first_block = cursor->number_of_pages;
    meta_page->last_block = cursor->number_of_pages;
    
    memcpy(cursor->page->body + cursor->page->page_header->offset, meta_page, META_TABLE_SIZE);
    cursor->page->page_header->offset += META_TABLE_SIZE;

    cursor->offset += META_TABLE_SIZE;
}

void print_type(MetaTable* meta_page) {
    printf("Print\n");
    printf("Type - %i\n", meta_page->type);
    printf("First - %i\n", meta_page->first_block);
    printf("Last - %i\n", meta_page->last_block);
    printf("Name - %s\n", meta_page->name_type);
}

void print_node(Node* node) {
    printf("Print node\n");
    printf("Type - %s\n", node->name_type);
    printf("Length - %i\n", node->name_length);
    printf("Name - %s\n", node->name);
}

void print_relationship(Relationship* node) {
    printf("Print relationship\n");
    printf("Type - %s\n", node->name_type);
    printf("Parent length - %i\n", node->parent_length);
    printf("Parent - %s\n", node->parent);
    printf("Child length - %i\n", node->child_length);
    printf("Child - %s\n", node->child);
}

void print_property(Property* node) {
    printf("Print property\n");
    printf("Subject length - %i\n", node->subject_length);
    printf("Subject - %s\n", node->subject);
    printf("Key length - %i\n", node->key_length);
    printf("Key - %s\n", node->key);
    printf("Value length - %i\n", node->value_length);
    printf("Value - %s\n", node->value);
    
}

MetaTable* get_meta_table(Cursor* cursor, TypeOfElement type, const char* name, uint64_t* pointer) {
    MetaTable* meta_page = (MetaTable*) malloc(META_TABLE_SIZE);
    PageHeader* pageHeader = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    Page* page = (Page*) malloc(PAGE_SIZE);
    
    void* page_body = malloc(PAGE_BODY_SIZE);
    int page_num = 0;

    off_t file_length = lseek(cursor->file->file_descriptor, 0, SEEK_END); // find end of file
    error_exit(file_length, "Failed to find end of file");

    do {
        set_pointer_offset_file(cursor->file, page_num * PAGE_SIZE);

        read_from_file(cursor->file, pageHeader, PAGE_HEADER_SIZE);        
        read_from_file(cursor->file, page_body, PAGE_BODY_SIZE);
        
        for (int i = 0; i < (pageHeader->offset / META_TABLE_SIZE); i++) {
            memcpy(meta_page, page_body + i * META_TABLE_SIZE, META_TABLE_SIZE);
            if (strcmp(meta_page->name_type, name) == 0 && type == meta_page->type) {
                *pointer = page_num * PAGE_SIZE + i * META_TABLE_SIZE + PAGE_HEADER_SIZE;
                return meta_page;
            }
        }
        page_num++;
    } while (page_num * PAGE_SIZE < file_length);
    
    return NULL;
}

void create_node_page(Cursor* cursor, PageHeader* pageHeader, uint64_t curr_table_page_offset) {
    // Записать в текущий блок номер следующего блока
    cursor->number_of_pages++;

    set_pointer_offset_file(cursor->file, curr_table_page_offset + sizeof(uint32_t));
    write_to_file(cursor->file, &(cursor->number_of_pages), sizeof(uint32_t));

    void* new_page = malloc(PAGE_SIZE);

    pageHeader->block_number = cursor->number_of_pages;
    pageHeader->offset = 0;
    pageHeader->next_block = 0;

    memcpy(new_page, pageHeader, PAGE_HEADER_SIZE);
    
    set_pointer_offset_file(cursor->file, cursor->number_of_pages * PAGE_SIZE);
    write_to_file(cursor->file, new_page, PAGE_SIZE);
}

void create_node(Cursor* cursor, Node* node) {
    uint64_t pointer = 0;

    MetaTable* table = get_meta_table(cursor, NODE, node->name_type, &(pointer));
    if (table == NULL) {
        printf("Error to find Node with name %s", node->name_type);
        return;
    }

    uint32_t page_num = table->last_block;
    uint32_t page_offset = page_num * PAGE_SIZE;

    PageHeader* pageHeader = (PageHeader*) malloc(PAGE_HEADER_SIZE);

    set_pointer_offset_file(cursor->file, page_offset);
    read_from_file(cursor->file, pageHeader, PAGE_HEADER_SIZE);

    uint32_t global_offset = page_offset + pageHeader->offset + PAGE_HEADER_SIZE;

    if (pageHeader->offset + sizeof(uint32_t) + sizeof(char) * node->name_length > PAGE_BODY_SIZE) {
        create_node_page(cursor, pageHeader, page_offset);
        
        table->last_block = cursor->number_of_pages;
        global_offset = cursor->number_of_pages * PAGE_SIZE + PAGE_HEADER_SIZE;
        
        // Записать номер последнего блока для мета таблицы
        set_pointer_offset_file(cursor->file, pointer + sizeof(TypeOfElement) + sizeof(uint32_t));
        write_to_file(cursor->file, &(table->last_block), sizeof(uint32_t));
    }

    set_pointer_offset_file(cursor->file, global_offset);
    write_to_file(cursor->file, &(node->name_length), sizeof(uint32_t));
    write_to_file(cursor->file, node->name, sizeof(char) * node->name_length);

    uint32_t header_offset = global_offset - pageHeader->offset - PAGE_HEADER_SIZE + sizeof(uint32_t) * 2;

    pageHeader->offset += sizeof(uint32_t) + sizeof(char) * node->name_length;

    set_pointer_offset_file(cursor->file, header_offset);
    write_to_file(cursor->file, &(pageHeader->offset), sizeof(uint32_t));
}

void read_nodes_by_type(Cursor* cursor, MetaTable* metaPage) {
    uint32_t read_block = metaPage->first_block;
    PageHeader* pageHeader = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    void* body = malloc(PAGE_BODY_SIZE);
    while (read_block != 0) {
        uint32_t page_offset = read_block * PAGE_SIZE;
        set_pointer_offset_file(cursor->file, page_offset);
        read_from_file(cursor->file, pageHeader, PAGE_HEADER_SIZE);  
        read_from_file(cursor->file, body, PAGE_BODY_SIZE);
        uint32_t offset = 0;
        Node* node = (Node*) malloc(sizeof(Node));
        strcpy(node->name_type, metaPage->name_type);
        while (offset != pageHeader->offset) {
            uint32_t name_length = 0;
            memcpy(&(name_length), body + offset, sizeof(uint32_t));
            char* name = malloc(sizeof(char) * name_length);
            memcpy(name, body + sizeof(uint32_t) + offset, sizeof(char) * name_length);
            node->name_length = name_length;
            node->name = name;
            offset += sizeof(char) * name_length + sizeof(uint32_t);
            print_node(node);
        }
        read_block = pageHeader->next_block;
    }
}

void read_all_nodes(Cursor* cursor) {
    MetaTable* metaPage = (MetaTable*) malloc(META_TABLE_SIZE);
    PageHeader* pageHeader = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    
    void* page_body = malloc(PAGE_BODY_SIZE);
    int page_num = 0;

    off_t file_length = lseek(cursor->file->file_descriptor, 0, SEEK_END); // find end of file
    error_exit(file_length, "Failed to find end of file");
    
    do {
        set_pointer_offset_file(cursor->file, page_num * PAGE_SIZE);

        read_from_file(cursor->file, pageHeader, PAGE_HEADER_SIZE);        
        read_from_file(cursor->file, page_body, PAGE_BODY_SIZE);
        
        for (int i = 0; i < (pageHeader->offset / META_TABLE_SIZE); i++) {
            memcpy(metaPage, page_body + i * META_TABLE_SIZE, META_TABLE_SIZE);
            if (metaPage->type == NODE) read_nodes_by_type(cursor, metaPage);
        }
        page_num++;
    } while (page_num * PAGE_SIZE < file_length);
}

void read_all_types(Cursor* cursor) {
    MetaTable* metaPage = (MetaTable*) malloc(META_TABLE_SIZE);
    PageHeader* pageHeader = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    
    void* page_body = malloc(PAGE_BODY_SIZE);
    int page_num = 0;

    off_t file_length = lseek(cursor->file->file_descriptor, 0, SEEK_END); // find end of file
    error_exit(file_length, "Failed to find end of file");
    

    do {
        set_pointer_offset_file(cursor->file, page_num * PAGE_SIZE);

        read_from_file(cursor->file, pageHeader, PAGE_HEADER_SIZE);        
        read_from_file(cursor->file, page_body, PAGE_BODY_SIZE);
        
        for (int i = 0; i < (pageHeader->offset / META_TABLE_SIZE); i++) {
            memcpy(metaPage, page_body + i * META_TABLE_SIZE, META_TABLE_SIZE);
            print_type(metaPage);
        }
        page_num++;
    } while (page_num * PAGE_SIZE < file_length);
}

void create_relationship_page(Cursor* cursor, PageHeader* pageHeader, uint64_t curr_table_page_offset) {
    // Записать в текущий блок номер следующего блока
    cursor->number_of_pages++;

    set_pointer_offset_file(cursor->file, curr_table_page_offset + sizeof(uint32_t));
    write_to_file(cursor->file, &(cursor->number_of_pages), sizeof(uint32_t));

    void* new_page = malloc(PAGE_SIZE);

    pageHeader->block_number = cursor->number_of_pages;
    pageHeader->offset = 0;
    pageHeader->next_block = 0;

    memcpy(new_page, pageHeader, PAGE_HEADER_SIZE);
    
    set_pointer_offset_file(cursor->file, cursor->number_of_pages * PAGE_SIZE);
    write_to_file(cursor->file, new_page, PAGE_SIZE);
}

void create_relationship(Cursor* cursor, Relationship* relationship) {
    uint64_t pointer = 0;

    MetaTable* table = get_meta_table(cursor, RELATIONSHIP, relationship->name_type, &(pointer));
    if (table == NULL) {
        printf("Error to find Node with name %s", relationship->name_type);
        return;
    }

    uint32_t page_num = table->last_block;
    uint32_t page_offset = page_num * PAGE_SIZE;

    PageHeader* pageHeader = (PageHeader*) malloc(PAGE_HEADER_SIZE);

    set_pointer_offset_file(cursor->file, page_offset);
    read_from_file(cursor->file, pageHeader, PAGE_HEADER_SIZE);

    uint32_t global_offset = page_offset + pageHeader->offset + PAGE_HEADER_SIZE;

    if (pageHeader->offset + sizeof(uint32_t) * 2 + sizeof(char) * (relationship->child_length + relationship->parent_length) > PAGE_BODY_SIZE) {
        create_relationship_page(cursor, pageHeader, page_offset);
        
        table->last_block = cursor->number_of_pages;
        global_offset = cursor->number_of_pages * PAGE_SIZE + PAGE_HEADER_SIZE;
        
        // Записать номер последнего блока для мета таблицы
        set_pointer_offset_file(cursor->file, pointer + sizeof(TypeOfElement) + sizeof(uint32_t));
        write_to_file(cursor->file, &(table->last_block), sizeof(uint32_t));
    }

    set_pointer_offset_file(cursor->file, global_offset);
    write_to_file(cursor->file, &(relationship->parent_length), sizeof(uint32_t));
    write_to_file(cursor->file, relationship->parent, sizeof(char) * relationship->parent_length);
    write_to_file(cursor->file, &(relationship->child_length), sizeof(uint32_t));
    write_to_file(cursor->file, relationship->child, sizeof(char) * relationship->child_length);

    uint32_t header_offset = global_offset - pageHeader->offset - PAGE_HEADER_SIZE + sizeof(uint32_t) * 2;

    pageHeader->offset += sizeof(uint32_t) * 2 + sizeof(char) * (relationship->child_length + relationship->parent_length);

    set_pointer_offset_file(cursor->file, header_offset);
    write_to_file(cursor->file, &(pageHeader->offset), sizeof(uint32_t));
}

void read_relationship_by_type(Cursor* cursor, MetaTable* metaPage) {
    uint32_t read_block = metaPage->first_block;
    PageHeader* pageHeader = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    void* body = malloc(PAGE_BODY_SIZE);
    while (read_block != 0) {
        uint32_t page_offset = read_block * PAGE_SIZE;
        set_pointer_offset_file(cursor->file, page_offset);
        read_from_file(cursor->file, pageHeader, PAGE_HEADER_SIZE);  
        read_from_file(cursor->file, body, PAGE_BODY_SIZE);
        uint32_t offset = 0;
        Relationship* relationship = (Relationship*) malloc(sizeof(Relationship));
        strcpy(relationship->name_type, metaPage->name_type);
        while (offset != pageHeader->offset) {
            uint32_t parent_length = 0;
            uint32_t child_length = 0;
            memcpy(&(parent_length), body + offset, sizeof(uint32_t));
            char* parent = malloc(sizeof(char) * parent_length);
            memcpy(parent, body + sizeof(uint32_t) + offset, sizeof(char) * parent_length);
            
            memcpy(&(child_length), body + offset + sizeof(uint32_t) + sizeof(char) * parent_length, sizeof(uint32_t));
            char* child = malloc(sizeof(char) * child_length);
            memcpy(child, body + sizeof(uint32_t) + offset + sizeof(uint32_t) + sizeof(char) * parent_length, sizeof(char) * child_length);
            
            relationship->parent_length = parent_length;
            relationship->parent = parent;
            relationship->child_length = child_length;
            relationship->child = child;
            offset += sizeof(char) * (parent_length + child_length) + 2 * sizeof(uint32_t);
            print_relationship(relationship);
        }
        read_block = pageHeader->next_block;
    }
}

void read_all_relationship(Cursor* cursor) {
    MetaTable* metaPage = (MetaTable*) malloc(META_TABLE_SIZE);
    PageHeader* pageHeader = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    
    void* page_body = malloc(PAGE_BODY_SIZE);
    int page_num = 0;

    off_t file_length = lseek(cursor->file->file_descriptor, 0, SEEK_END); // find end of file
    error_exit(file_length, "Failed to find end of file");
    
    do {
        set_pointer_offset_file(cursor->file, page_num * PAGE_SIZE);

        read_from_file(cursor->file, pageHeader, PAGE_HEADER_SIZE);        
        read_from_file(cursor->file, page_body, PAGE_BODY_SIZE);
        
        for (int i = 0; i < (pageHeader->offset / META_TABLE_SIZE); i++) {
            memcpy(metaPage, page_body + i * META_TABLE_SIZE, META_TABLE_SIZE);
            if (metaPage->type == RELATIONSHIP) read_relationship_by_type(cursor, metaPage);
        }
        page_num++;
    } while (page_num * PAGE_SIZE < file_length);
}

void create_property_page(Cursor* cursor, PageHeader* pageHeader, uint64_t curr_table_page_offset) {
    // Записать в текущий блок номер следующего блока
    cursor->number_of_pages++;

    set_pointer_offset_file(cursor->file, curr_table_page_offset + sizeof(uint32_t));
    write_to_file(cursor->file, &(cursor->number_of_pages), sizeof(uint32_t));

    void* new_page = malloc(PAGE_SIZE);

    pageHeader->block_number = cursor->number_of_pages;
    pageHeader->offset = 0;
    pageHeader->next_block = 0;

    memcpy(new_page, pageHeader, PAGE_HEADER_SIZE);
    
    set_pointer_offset_file(cursor->file, cursor->number_of_pages * PAGE_SIZE);
    write_to_file(cursor->file, new_page, PAGE_SIZE);
}

void create_property(Cursor* cursor, Property* property) {
    uint64_t pointer = 0;

    MetaTable* table = get_meta_table(cursor, PROPERTY, "proper", &(pointer));
    if (table == NULL) {
        printf("Error to find Node with name %s", "proper");
        return;
    }

    uint32_t page_num = table->last_block;
    uint32_t page_offset = page_num * PAGE_SIZE;

    PageHeader* pageHeader = (PageHeader*) malloc(PAGE_HEADER_SIZE);

    set_pointer_offset_file(cursor->file, page_offset);
    read_from_file(cursor->file, pageHeader, PAGE_HEADER_SIZE);

    uint32_t global_offset = page_offset + pageHeader->offset + PAGE_HEADER_SIZE;

    if (pageHeader->offset + sizeof(uint32_t) * 3 + sizeof(char) * (property->subject_length + property->key_length + property->value_length) > PAGE_BODY_SIZE) {
        create_property_page(cursor, pageHeader, page_offset);
        
        table->last_block = cursor->number_of_pages;
        global_offset = cursor->number_of_pages * PAGE_SIZE + PAGE_HEADER_SIZE;
        
        // Записать номер последнего блока для мета таблицы
        set_pointer_offset_file(cursor->file, pointer + sizeof(TypeOfElement) + sizeof(uint32_t));
        write_to_file(cursor->file, &(table->last_block), sizeof(uint32_t));
    }

    set_pointer_offset_file(cursor->file, global_offset);
    write_to_file(cursor->file, &(property->subject_length), sizeof(uint32_t));
    write_to_file(cursor->file, property->subject, sizeof(char) * property->subject_length);
    write_to_file(cursor->file, &(property->key_length), sizeof(uint32_t));
    write_to_file(cursor->file, property->key, sizeof(char) * property->key_length);
    write_to_file(cursor->file, &(property->value_length), sizeof(uint32_t));
    write_to_file(cursor->file, property->value, sizeof(char) * property->value_length);

    uint32_t header_offset = global_offset - pageHeader->offset - PAGE_HEADER_SIZE + sizeof(uint32_t) * 2;

    pageHeader->offset += sizeof(uint32_t) * 3 + sizeof(char) * (property->subject_length + property->key_length + property->value_length);

    set_pointer_offset_file(cursor->file, header_offset);
    write_to_file(cursor->file, &(pageHeader->offset), sizeof(uint32_t));
}

void read_property_by_type(Cursor* cursor, MetaTable* metaPage) {
    uint32_t read_block = metaPage->first_block;
    PageHeader* pageHeader = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    void* body = malloc(PAGE_BODY_SIZE);
    while (read_block != 0) {
        uint32_t page_offset = read_block * PAGE_SIZE;
        set_pointer_offset_file(cursor->file, page_offset);
        read_from_file(cursor->file, pageHeader, PAGE_HEADER_SIZE);  
        read_from_file(cursor->file, body, PAGE_BODY_SIZE);
        uint32_t offset = 0;
        Property* property = (Property*) malloc(sizeof(Property));
        // strcpy(property->name_type, metaPage->name_type);
        while (offset != pageHeader->offset) {
            uint32_t subject_length = 0;
            uint32_t key_length = 0;
            uint32_t value_length = 0;

            memcpy(&(subject_length), body + offset, sizeof(uint32_t));
            char* subject = malloc(sizeof(char) * subject_length);
            memcpy(subject, body + offset + sizeof(uint32_t) , sizeof(char) * subject_length);
            
            memcpy(&(key_length), body + offset + sizeof(uint32_t) + sizeof(char) * subject_length, sizeof(uint32_t));
            char* key = malloc(sizeof(char) * key_length);
            memcpy(key, body + offset + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(char) * subject_length, sizeof(char) * key_length);
            
            memcpy(&(value_length), body + offset + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(char) * (subject_length + key_length), sizeof(uint32_t));
            char* value = malloc(sizeof(char) * value_length);
            memcpy(value, body + offset + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(char) * (subject_length + key_length), sizeof(char) * value_length);
            
            property->subject_length = subject_length;
            property->subject = subject;
            property->key_length = key_length;
            property->key = key;
            property->value_length = value_length;
            property->value = value;
            offset += sizeof(char) * (subject_length + key_length + value_length) + 3 * sizeof(uint32_t);
            print_property(property);
        }
        read_block = pageHeader->next_block;
    }
}

void read_all_property(Cursor* cursor) {
    MetaTable* metaPage = (MetaTable*) malloc(META_TABLE_SIZE);
    PageHeader* pageHeader = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    
    void* page_body = malloc(PAGE_BODY_SIZE);
    int page_num = 0;

    off_t file_length = lseek(cursor->file->file_descriptor, 0, SEEK_END); // find end of file
    error_exit(file_length, "Failed to find end of file");
    
    do {
        set_pointer_offset_file(cursor->file, page_num * PAGE_SIZE);

        read_from_file(cursor->file, pageHeader, PAGE_HEADER_SIZE);        
        read_from_file(cursor->file, page_body, PAGE_BODY_SIZE);
        
        for (int i = 0; i < (pageHeader->offset / META_TABLE_SIZE); i++) {
            memcpy(metaPage, page_body + i * META_TABLE_SIZE, META_TABLE_SIZE);
            if (metaPage->type == PROPERTY) read_property_by_type(cursor, metaPage);
        }
        page_num++;
    } while (page_num * PAGE_SIZE < file_length);
}

void insert_meta_table(Cursor* cursor) {
    MetaTable data[] = {
        {NODE, 0, 0, "person"},
        {NODE, 0, 0, "pc"},
        { RELATIONSHIP, 0, 0, "play"},
        { RELATIONSHIP, 0, 0, "brothe"},
        {PROPERTY, 0, 0, "proper"},
    };

    int count_to_write = sizeof(data) / META_TABLE_SIZE;
    for (int i = 0; i < count_to_write; i++) {
        save_type(cursor, &(data[i]));
        printf("Saved table - %i\n", i);
    }
    flush_page(cursor);
}

void insert_node(Cursor* cursor) {
    Node nodes[] = {
        {"person", 9, "sasaovch"},
        { "person", 7, "andrey"},
        { "pc", 5, "asus"},
        { "pc", 4, "mac"},
    };

    int count_to_write = sizeof(nodes) / sizeof(Node);
    for (int i = 0; i < count_to_write; i++) {
        create_node(cursor, &(nodes[i]));
        printf("Saved node - %i\n", i);
    }
    // flush_page(cursor);
}

void insert_relationship(Cursor* cursor) {
    Relationship relationships[] = {
        {"play", 8, "sasaovch", 3, "mac"},
        {"play", 6, "andrey", 4, "asus"},
        {"brothe",8, "sasaovch", 3, "and"},
        {"brothe", 6, "andrey", 4, "sasa"},
    };
    
    int count_to_write = sizeof(relationships) / sizeof(Relationship);
    for (int i = 0; i < count_to_write; i++) {
        create_relationship(cursor, &(relationships[i]));
        printf("Saved relationship - %i\n", i);
    }
}

void insert_property(Cursor* cursor) {
    Property property[] = {
        {8, "sasaovch", 3, "age", 2, "20"},
        {6, "andrey", 3, "age", 2, "14"},
        {3, "mac", 5, "color", 6, "silver"},
        {4, "asus", 5, "color", 5, "black"}
    };

        
    int count_to_write = sizeof(property) / sizeof(Property);
    for (int i = 0; i < count_to_write; i++) {
        create_property(cursor, &(property[i]));
        printf("Saved property - %i\n", i);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Not enough parameters\n");
        exit(EXIT_FAILURE);
    }

    char* filename = argv[1];
    Cursor* cursor = db_open(filename);
    printf("Create file\n");

    insert_meta_table(cursor);
    printf("Inserted\n");
    read_all_types(cursor);

    insert_node(cursor);
    printf("Inserted nodes\n");
    read_all_nodes(cursor);
    
    insert_relationship(cursor);
    printf("Inserted relationship\n");
    read_all_relationship(cursor);
    
    insert_property(cursor);
    printf("Inserted property\n");
    read_all_property(cursor);


    // int fd = open("test",
    //             O_RDWR |      // Read/Write mode
    //                 O_CREAT,  // Create file if it does not exist
    //             S_IWUSR |     // User write permission
    //                 S_IRUSR   // User read permission
    // );

    // error_exit(fd, "Unabled to open file");
    // if (fd == -1) {
    //     printf("Unabled to open file\n");
    //     exit(EXIT_FAILURE);
    // }

    // off_t file_length = lseek(fd, 0, SEEK_END); // find end of file
    // printf("File lenght: %llu\n", file_length);

    // File* file = (File*) malloc(sizeof(File));
    // file->file_descriptor = fd;
    // file->file_length = file_length;

    // PageHeader* pageHeader = (PageHeader*) malloc(sizeof(PageHeader));
    // Page* page = (Page*) malloc(sizeof(Page));

    // void* page_header = malloc(PAGE_HEADER_SIZE);
    // void* page_body = malloc(PAGE_BODY_SIZE);

    // pageHeader->next_block = 909;
    // pageHeader->block_number = 3234234;
    // pageHeader->offset = 1023;

    // memcpy(page_header, pageHeader, PAGE_SIZE);
    // off_t offset = lseek(file->file_descriptor, 0, SEEK_SET);
    // error_exit(offset, "Error seeking offset");
    // if (offset == -1) {
    //     printf("Error seeking: %d\n", 1);
    //     exit(EXIT_FAILURE);
    // }
    // ssize_t bytes_written =
        // write(file->file_descriptor, page_header, PAGE_HEADER_SIZE);
}