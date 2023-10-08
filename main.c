#include <fcntl.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

//FIXME: подумать о слишком больших данных
const uint32_t PAGE_SIZE = 48;
const uint32_t NAME_TYPE_SIZE = 6;

typedef struct {
    int file_descriptor;
    uint64_t file_length;
} File;

typedef enum {
  NODE = 0,
  RELATIONSHIP = 1,
  PROPERTY = 2,
  META = 3,
} TypeOfElement;

typedef enum {
  INT = 0,
  FLOAT = 1,
  STRING = 2,
  BOOL = 3,
} ValueType;

typedef struct {
    uint32_t block_number;
    uint32_t next_block;
    uint32_t offset;
} PageHeader;

typedef struct {
    PageHeader* page_header;
    void* body;
} Page;

typedef struct {
    File* file;
    uint32_t first_meta_table_block;
    uint32_t last_meta_table_block;
    uint32_t number_of_pages;
    Page* page;
} Cursor;

typedef struct {
    TypeOfElement type;
    uint32_t first_block;
    uint32_t last_block;
    char name_type[NAME_TYPE_SIZE + 1];
} MetaTable;

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
    ValueType value_type;
    uint32_t value_length;
    char* value;
} Property;

const uint64_t UINT32_T_SIZE = sizeof(uint32_t);
const uint64_t CHAR_SIZE = sizeof(char);
const uint32_t META_TABLE_SIZE = sizeof(MetaTable);
const uint32_t PAGE_HEADER_SIZE = sizeof(PageHeader);
const uint32_t VALUE_TYPE_SIZE = sizeof(ValueType);
const uint32_t PAGE_BODY_SIZE = PAGE_SIZE - PAGE_HEADER_SIZE;

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

uint32_t find_last_meta_table(uint64_t file_length, Page* page, File* file) {
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

void print_page(PageHeader* page_header) {
    println("Header");
    println("Block - %i", page_header->block_number);
    println("Next - %i", page_header->next_block);
    println("Offset - %i", page_header->offset);
}

void print_type(MetaTable* meta_page) {
    println("Print");
    println("Type - %i", meta_page->type);
    println("First - %i", meta_page->first_block);
    println("Last - %i", meta_page->last_block);
    println("Name - %s", meta_page->name_type);
}

void print_node(Node* node) {
    println("Print node");
    println("Type - %s", node->name_type);
    println("Length - %i", node->name_length);
    println("Name - %s", node->name);
}

void print_relationship(Relationship* relationship) {
    println("Print relationship");
    println("Type - %s", relationship->name_type);
    println("Parent length - %i", relationship->parent_length);
    println("Parent - %s", relationship->parent);
    println("Child length - %i", relationship->child_length);
    println("Child - %s", relationship->child);
}

void print_property(Property* property) {
    println("Print property");
    println("Subject length - %i", property->subject_length);
    println("Subject - %s", property->subject);
    println("Key length - %i", property->key_length);
    println("Key - %s", property->key);
    println("Value type - %i", property->value_type);
    println("Value length - %i", property->value_length);
    println("Value - %s", property->value);
    
}

void write_node_to_file(Cursor* cursor, void* nd) {
    Node* node = (Node*) nd;
    write_to_file(cursor->file, &(node->name_length), UINT32_T_SIZE);
    write_to_file(cursor->file, node->name, CHAR_SIZE * node->name_length);
}
void write_relationship_to_file(Cursor* cursor, void* rl) {
    Relationship* relationship = (Relationship*) rl;
    write_to_file(cursor->file, &(relationship->parent_length), UINT32_T_SIZE);
    write_to_file(cursor->file, relationship->parent, CHAR_SIZE * relationship->parent_length);
    write_to_file(cursor->file, &(relationship->child_length), UINT32_T_SIZE);
    write_to_file(cursor->file, relationship->child, CHAR_SIZE * relationship->child_length);
}
void write_property_to_file(Cursor* cursor, void* pr) {
    Property* property = (Property*) pr;
    write_to_file(cursor->file, &(property->subject_length), UINT32_T_SIZE);
    write_to_file(cursor->file, property->subject, CHAR_SIZE * property->subject_length);
    write_to_file(cursor->file, &(property->key_length), UINT32_T_SIZE);
    write_to_file(cursor->file, property->key, CHAR_SIZE * property->key_length);
    write_to_file(cursor->file, &(property->value_type), VALUE_TYPE_SIZE);
    write_to_file(cursor->file, &(property->value_length), UINT32_T_SIZE);
    write_to_file(cursor->file, property->value, CHAR_SIZE * property->value_length);
}

uint64_t get_size_of_node(void* nd) {
    Node* node = (Node*) nd;
    return UINT32_T_SIZE + CHAR_SIZE * node->name_length;
}
uint64_t get_size_of_relationship(void* rl) {
    Relationship* relationship = (Relationship*) rl;
    return UINT32_T_SIZE * 2 + CHAR_SIZE * (relationship->child_length + relationship->parent_length);
}
uint64_t get_size_of_property(void* pr) {
    Property* property = (Property*) pr;
    return UINT32_T_SIZE * 3 + CHAR_SIZE * (property->subject_length + property->key_length + property->value_length) + VALUE_TYPE_SIZE;
}

void* read_node(void* element, MetaTable* metaPage, void* body, uint64_t* offset) {
    Node* node = (Node*) element;
    strcpy(node->name_type, metaPage->name_type);
    uint32_t name_length = 0;
    memcpy(&(name_length), body + *offset, UINT32_T_SIZE);
    char* name = malloc(CHAR_SIZE * name_length);
    memcpy(name, body + UINT32_T_SIZE + *offset, CHAR_SIZE * name_length);
    node->name_length = name_length;
    node->name = name;
    *offset += CHAR_SIZE * name_length + UINT32_T_SIZE;
    // print_node(node);
    return node;
}

void* read_relationship(void* element, MetaTable* metaPage, void* body, uint64_t* offset) {
    Relationship* relationship = (Relationship*) element;
    strcpy(relationship->name_type, metaPage->name_type);

    uint32_t parent_length = 0;
    uint32_t child_length = 0;

    memcpy(&(parent_length), body + *offset, UINT32_T_SIZE);
    char* parent = malloc(CHAR_SIZE * parent_length);
    memcpy(parent, body + UINT32_T_SIZE + *offset, CHAR_SIZE * parent_length);
    
    memcpy(&(child_length), body + *offset + UINT32_T_SIZE + CHAR_SIZE * parent_length, UINT32_T_SIZE);
    char* child = malloc(CHAR_SIZE * child_length);
    memcpy(child, body + UINT32_T_SIZE + *offset + UINT32_T_SIZE + CHAR_SIZE * parent_length, CHAR_SIZE * child_length);
    
    relationship->parent_length = parent_length;
    relationship->parent = parent;
    relationship->child_length = child_length;
    relationship->child = child;

    *offset += CHAR_SIZE * (parent_length + child_length) + 2 * UINT32_T_SIZE;
    // print_relationship(relationship);
    return relationship;
}

void* read_property(void* element, MetaTable* metaPage, void* body, uint64_t* offset) {
    Property* property = (Property*) element;
    uint32_t subject_length = 0;
    uint32_t key_length = 0;
    uint32_t value_length = 0;
    ValueType value_type;

    memcpy(&(subject_length), body + *offset, UINT32_T_SIZE);
    char* subject = malloc(CHAR_SIZE * subject_length);
    memcpy(subject, body + *offset + UINT32_T_SIZE , CHAR_SIZE * subject_length);
    
    memcpy(&(key_length), body + *offset + UINT32_T_SIZE + CHAR_SIZE * subject_length, UINT32_T_SIZE);
    char* key = malloc(CHAR_SIZE * key_length);
    memcpy(key, body + *offset + UINT32_T_SIZE + UINT32_T_SIZE + CHAR_SIZE * subject_length, CHAR_SIZE * key_length);
    
    memcpy(&(value_type), body + *offset + UINT32_T_SIZE + UINT32_T_SIZE + CHAR_SIZE * (subject_length + key_length), VALUE_TYPE_SIZE);
    memcpy(&(value_length), body + *offset + UINT32_T_SIZE + UINT32_T_SIZE + CHAR_SIZE * (subject_length + key_length) + VALUE_TYPE_SIZE, UINT32_T_SIZE);
    char* value = malloc(CHAR_SIZE * value_length);
    memcpy(value, body + *offset + UINT32_T_SIZE + UINT32_T_SIZE + UINT32_T_SIZE + CHAR_SIZE * (subject_length + key_length) + VALUE_TYPE_SIZE, CHAR_SIZE * value_length);
    
    property->subject_length = subject_length;
    property->subject = subject;
    property->key_length = key_length;
    property->key = key;
    property->value_type = value_type;
    property->value_length = value_length;
    property->value = value;

    *offset += CHAR_SIZE * (subject_length + key_length + value_length) + 3 * UINT32_T_SIZE + VALUE_TYPE_SIZE;
    // print_property(property);
    return property;
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

MetaTable* get_meta_table(Cursor* cursor, TypeOfElement type, const char* name, uint64_t* pointer) {
    MetaTable* meta_page = (MetaTable*) malloc(META_TABLE_SIZE);
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
            memcpy(meta_page, page_body + i * META_TABLE_SIZE, META_TABLE_SIZE);
            if (strcmp(meta_page->name_type, name) == 0 && type == meta_page->type) {
                *pointer = page_num * PAGE_SIZE + i * META_TABLE_SIZE + PAGE_HEADER_SIZE;
                return meta_page;
            }
        }
        page_num = pageHeader->next_block;
    } while (page_num != 0);
    
    return NULL;
}

void create_type(Cursor* cursor, MetaTable* meta_page) {
    // Если в текущем блоке не хватает места, то нужно сохранить блок в память и создать новый
    uint64_t pointer = 0;
    MetaTable* table = get_meta_table(cursor, meta_page->type, meta_page->name_type, &(pointer));
    if (table != NULL) {
        println("%i with name %s already exists", meta_page->type, meta_page->name_type);
        return;
    }

    if (cursor->page->page_header->offset + META_TABLE_SIZE > PAGE_BODY_SIZE) {
        save_meta_page_and_create_new(cursor);
    }

    // create page for type
    create_page_for_type(cursor);
    
    meta_page->first_block = cursor->number_of_pages;
    meta_page->last_block = cursor->number_of_pages;
    
    memcpy(cursor->page->body + cursor->page->page_header->offset, meta_page, META_TABLE_SIZE);
    cursor->page->page_header->offset += META_TABLE_SIZE;
}

void delete_type(Cursor* cursor, MetaTable* meta_page) {
    uint32_t* stack = (uint32_t*) malloc(100 * sizeof(uint32_t));
    uint64_t pointer = 0;

    MetaTable* delete_table = get_meta_table(cursor, meta_page->type, meta_page->name_type, &(pointer));
    debug(428, "%i", pointer);
    if (delete_table == NULL) {
        println("Error to find Node with name %s", meta_page->name_type);
        return;
    }

    PageHeader* pageHeader = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    uint32_t read_block = delete_table->first_block;
    uint32_t counter = 0;
    stack[counter] = read_block;

    while (read_block != 0) {
        uint64_t page_offset = read_block * PAGE_SIZE;
        set_pointer_offset_file(cursor->file, page_offset);
        read_from_file(cursor->file, pageHeader, PAGE_HEADER_SIZE);  

        read_block = pageHeader->next_block;
        stack[++counter] = read_block;
    }

    void* empty_body = calloc(1, PAGE_BODY_SIZE);
    while (--counter != -1) {
        uint32_t block_number = stack[counter];
        println("%i", block_number);
        uint64_t page_offset = block_number * PAGE_SIZE;
        uint32_t zero = 0;
        set_pointer_offset_file(cursor->file, page_offset + PAGE_HEADER_SIZE - UINT32_T_SIZE);
        write_to_file(cursor->file, &(zero), UINT32_T_SIZE);
        write_to_file(cursor->file, empty_body, PAGE_BODY_SIZE);
        if (block_number == cursor->number_of_pages) {
            cursor->number_of_pages--;
        }
    }
    if (ftruncate(cursor->file->file_descriptor, (cursor->number_of_pages + 1) * PAGE_SIZE - 1) == -1) {
        printf("Failed to clear the file.\n");
    }

    uint64_t page_number = pointer / PAGE_SIZE;
    uint32_t page_offset = pointer % PAGE_SIZE;
    uint64_t offset = pointer % PAGE_SIZE - PAGE_HEADER_SIZE;

    set_pointer_offset_file(cursor->file, page_number * PAGE_SIZE);
    read_from_file(cursor->file, pageHeader, PAGE_HEADER_SIZE);  

    void* old_body = malloc(PAGE_BODY_SIZE);
    read_from_file(cursor->file, old_body, PAGE_BODY_SIZE);

    void* new_body = malloc(PAGE_BODY_SIZE);

    memcpy(new_body, old_body, offset);
    memcpy(new_body + offset, old_body + offset + META_TABLE_SIZE, PAGE_BODY_SIZE - offset - META_TABLE_SIZE);

    uint64_t new_offset = pageHeader->offset - META_TABLE_SIZE;
    set_pointer_offset_file(cursor->file, page_number * PAGE_SIZE + PAGE_HEADER_SIZE - UINT32_T_SIZE);
    write_to_file(cursor->file, &(new_offset), UINT32_T_SIZE);
    write_to_file(cursor->file, new_body, PAGE_BODY_SIZE);

    if (new_offset == 0) {
        PageHeader* oldPage = (PageHeader*) malloc(PAGE_HEADER_SIZE);
        memcpy(oldPage, pageHeader, PAGE_HEADER_SIZE);
        print_page(oldPage);
        PageHeader* page_header = (PageHeader*) malloc(PAGE_HEADER_SIZE);
        void* copy_body = malloc(PAGE_BODY_SIZE);
        uint32_t* stack = (uint32_t*) malloc(100 * sizeof(uint32_t));
        uint32_t counter = 0;
        
        while (oldPage->next_block != 0) {
            set_pointer_offset_file(cursor->file, oldPage->next_block * PAGE_SIZE);
            read_from_file(cursor->file, page_header, PAGE_HEADER_SIZE);
            print_page(page_header);
            read_from_file(cursor->file, copy_body, PAGE_BODY_SIZE);

            set_pointer_offset_file(cursor->file, oldPage->block_number * PAGE_SIZE + PAGE_HEADER_SIZE - UINT32_T_SIZE);
            write_to_file(cursor->file, &(page_header->offset), UINT32_T_SIZE);
            write_to_file(cursor->file, copy_body, PAGE_BODY_SIZE);
            if (page_header->next_block == 0) {
                set_pointer_offset_file(cursor->file, oldPage->block_number * PAGE_SIZE + UINT32_T_SIZE);
                write_to_file(cursor->file, &(page_header->next_block), UINT32_T_SIZE);
            }
            memcpy(oldPage, page_header, PAGE_HEADER_SIZE);
        }

        void* empty_block = calloc(1, PAGE_BODY_SIZE);
        uint32_t zero = 0;
        set_pointer_offset_file(cursor->file, oldPage->block_number * PAGE_SIZE + PAGE_HEADER_SIZE - UINT32_T_SIZE);
        write_to_file(cursor->file, &(zero), UINT32_T_SIZE);
        write_to_file(cursor->file, empty_block, PAGE_BODY_SIZE);

        if (oldPage->block_number == cursor->number_of_pages) {
            oldPage->offset = 0;
            while (oldPage->offset == 0) {
                cursor->number_of_pages--;
                println("Remove %i", cursor->number_of_pages);
                set_pointer_offset_file(cursor->file, cursor->number_of_pages * PAGE_SIZE);
                read_from_file(cursor->file, oldPage, PAGE_HEADER_SIZE);
            }
            if (ftruncate(cursor->file->file_descriptor, (cursor->number_of_pages + 1) * PAGE_SIZE - 1) == -1) {
                printf("Failed to clear the file.\n");
            }
        }
    }
}

void update_type(Cursor* cursor, MetaTable* old_meta_page, MetaTable* new_meta_page) {
    uint64_t pointer = 0;
    MetaTable* table = get_meta_table(cursor, old_meta_page->type, old_meta_page->name_type, &(pointer));
    if (table == NULL) {
        println("Error to find Node with name %s", old_meta_page->name_type);
        return;
    }

    new_meta_page->first_block = table->first_block;
    new_meta_page->last_block = table->last_block;
    new_meta_page->type = table->type;

    set_pointer_offset_file(cursor->file, pointer);
    write_to_file(cursor->file, new_meta_page, META_TABLE_SIZE);
}

void create_new_page(Cursor* cursor, PageHeader* pageHeader, uint64_t curr_table_page_offset) {
    // Записать в текущий блок номер следующего блока
    cursor->number_of_pages++;

    set_pointer_offset_file(cursor->file, curr_table_page_offset + UINT32_T_SIZE);
    write_to_file(cursor->file, &(cursor->number_of_pages), UINT32_T_SIZE);

    void* new_page = malloc(PAGE_SIZE);

    pageHeader->block_number = cursor->number_of_pages;
    pageHeader->offset = 0;
    pageHeader->next_block = 0;

    memcpy(new_page, pageHeader, PAGE_HEADER_SIZE);
    
    set_pointer_offset_file(cursor->file, cursor->number_of_pages * PAGE_SIZE);
    write_to_file(cursor->file, new_page, PAGE_SIZE);
}

void create_element(Cursor* cursor, void* element, void (*write_to_file_element)(Cursor*, void *), const char* name_type, TypeOfElement type, uint64_t (*get_size_of_element)(void *)) {
    uint64_t pointer = 0;

    MetaTable* table = get_meta_table(cursor, type, name_type, &(pointer));
    if (table == NULL) {
        println("Error to find Node with name %s", name_type);
        return;
    }

    uint32_t page_num = table->last_block;
    uint64_t page_offset = page_num * PAGE_SIZE;

    PageHeader* pageHeader = (PageHeader*) malloc(PAGE_HEADER_SIZE);

    set_pointer_offset_file(cursor->file, page_offset);
    read_from_file(cursor->file, pageHeader, PAGE_HEADER_SIZE);

    uint64_t global_offset = page_offset + pageHeader->offset + PAGE_HEADER_SIZE;

    if (pageHeader->offset + get_size_of_element(element) > PAGE_BODY_SIZE) {
        create_new_page(cursor, pageHeader, page_offset);
        
        table->last_block = cursor->number_of_pages;
        global_offset = cursor->number_of_pages * PAGE_SIZE + PAGE_HEADER_SIZE;
        
        // Записать номер последнего блока для мета таблицы
        set_pointer_offset_file(cursor->file, pointer + sizeof(TypeOfElement) + UINT32_T_SIZE);
        write_to_file(cursor->file, &(table->last_block), UINT32_T_SIZE);
    }

    set_pointer_offset_file(cursor->file, global_offset);
    write_to_file_element(cursor, element);

    uint64_t header_offset = global_offset - pageHeader->offset - PAGE_HEADER_SIZE + UINT32_T_SIZE * 2;

    pageHeader->offset += get_size_of_element(element);

    set_pointer_offset_file(cursor->file, header_offset);
    write_to_file(cursor->file, &(pageHeader->offset), UINT32_T_SIZE);
}

void* find_element(Cursor* cursor, MetaTable* metaPage, Page* page, uint64_t size_of_element, void* find_elem, void* (*read_element)(void*, MetaTable*, void*, uint64_t*), bool (*comparator)(void*, void*), uint64_t* offset_) {
    uint32_t read_block = metaPage->first_block;
    PageHeader* pageHeader = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    void* body = malloc(PAGE_BODY_SIZE);

    while (read_block != 0) {
        uint64_t page_offset = read_block * PAGE_SIZE;
        set_pointer_offset_file(cursor->file, page_offset);
        read_from_file(cursor->file, pageHeader, PAGE_HEADER_SIZE);  
        read_from_file(cursor->file, body, PAGE_BODY_SIZE);
        uint64_t offset = 0;

        void* element = (void*) malloc(size_of_element);
        while (offset != pageHeader->offset) {
            read_element(element, metaPage, body, &(offset));
            if (comparator(element, find_elem)) {
                page->body = body;
                page->page_header = pageHeader;
                *offset_ = offset;
                return element;
            }
        }
        read_block = pageHeader->next_block;
    }
    return NULL;
}

void read_element_by_type(Cursor* cursor, MetaTable* metaPage, uint64_t size_of_element, void* (*read_element)(void*, MetaTable*, void*, uint64_t*)) {
    uint32_t read_block = metaPage->first_block;
    PageHeader* pageHeader = (PageHeader*) malloc(PAGE_HEADER_SIZE);
    void* body = malloc(PAGE_BODY_SIZE);

    while (read_block != 0) {
        uint64_t page_offset = read_block * PAGE_SIZE;
        set_pointer_offset_file(cursor->file, page_offset);
        read_from_file(cursor->file, pageHeader, PAGE_HEADER_SIZE);  
        read_from_file(cursor->file, body, PAGE_BODY_SIZE);
        uint64_t offset = 0;

        void* element = (void*) malloc(size_of_element);
        while (offset != pageHeader->offset) {
            read_element(element, metaPage, body, &(offset));
        }
        read_block = pageHeader->next_block;
    }
}


void delete_element(Cursor* cursor, void* element, uint64_t (*get_size_of_element)(void *), uint64_t size_of_sturcture, void* name_type, TypeOfElement type, void* (*read_element)(void*, MetaTable*, void*, uint64_t*), bool (*comparator)(void*, void*)) {
    uint64_t pointer = 0;

    MetaTable* table = get_meta_table(cursor, type, name_type, &(pointer));
    println("Pointer %i", pointer);
    if (table == NULL) {
        println("Error to find Node with name %s", name_type);
        return;
    }

    uint64_t offset = 0;
    uint32_t last_block = 0;
    void* new_body = malloc(PAGE_BODY_SIZE);
    Page* page = (Page*) malloc(sizeof(Page));

    void* find_el = find_element(cursor, table, page, size_of_sturcture, element, read_element, comparator, &(offset));
    offset -= get_size_of_element(find_el);

    memcpy(new_body, page->body, offset);
    memcpy(new_body + offset, page->body + offset + get_size_of_element(find_el), PAGE_BODY_SIZE - offset - get_size_of_element(find_el));

    uint64_t new_offset = page->page_header->offset - get_size_of_element(find_el);
    set_pointer_offset_file(cursor->file, page->page_header->block_number * PAGE_SIZE + PAGE_HEADER_SIZE - UINT32_T_SIZE);
    write_to_file(cursor->file, &(new_offset), UINT32_T_SIZE);
    write_to_file(cursor->file, new_body, PAGE_BODY_SIZE);
    
    if (new_offset == 0) {
        PageHeader* oldPage = (PageHeader*) malloc(PAGE_HEADER_SIZE);
        memcpy(oldPage, page->page_header, PAGE_HEADER_SIZE);
        print_page(oldPage);
        PageHeader* pageHeader = (PageHeader*) malloc(PAGE_HEADER_SIZE);
        void* copy_body = malloc(PAGE_BODY_SIZE);
        uint32_t* stack = (uint32_t*) malloc(100 * sizeof(uint32_t));
        uint32_t counter = 0;
        last_block = oldPage->block_number;
        
        while (oldPage->next_block != 0) {
            set_pointer_offset_file(cursor->file, oldPage->next_block * PAGE_SIZE);
            read_from_file(cursor->file, pageHeader, PAGE_HEADER_SIZE);
            print_page(pageHeader);
            read_from_file(cursor->file, copy_body, PAGE_BODY_SIZE);

            set_pointer_offset_file(cursor->file, oldPage->block_number * PAGE_SIZE + PAGE_HEADER_SIZE - UINT32_T_SIZE);
            write_to_file(cursor->file, &(pageHeader->offset), UINT32_T_SIZE);
            write_to_file(cursor->file, copy_body, PAGE_BODY_SIZE);
            if (pageHeader->next_block == 0 && cursor->number_of_pages == pageHeader->block_number) {
                set_pointer_offset_file(cursor->file, oldPage->block_number * PAGE_SIZE + UINT32_T_SIZE);
                write_to_file(cursor->file, &(pageHeader->next_block), UINT32_T_SIZE);
            }
            println("While");
            last_block = oldPage->block_number;
            memcpy(oldPage, pageHeader, PAGE_HEADER_SIZE);
        }

        void* empty_block = calloc(1, PAGE_BODY_SIZE);
        uint32_t zero = 0;
        set_pointer_offset_file(cursor->file, oldPage->block_number * PAGE_SIZE + PAGE_HEADER_SIZE);
        // write_to_file(cursor->file, &(zero), UINT32_T_SIZE);
        write_to_file(cursor->file, empty_block, PAGE_BODY_SIZE);

        if (oldPage->block_number == cursor->number_of_pages) {
            oldPage->offset = 0;
            while (oldPage->offset == 0) {
                cursor->number_of_pages--;
                println("Remove %i", cursor->number_of_pages);
                set_pointer_offset_file(cursor->file, cursor->number_of_pages * PAGE_SIZE);
                read_from_file(cursor->file, oldPage, PAGE_HEADER_SIZE);
            }
            if (ftruncate(cursor->file->file_descriptor, (cursor->number_of_pages + 1) * PAGE_SIZE - 1) == -1) {
                printf("Failed to clear the file.\n");
            }
        }
        println("Last %i", last_block);
        set_pointer_offset_file(cursor->file, pointer + sizeof(TypeOfElement) + UINT32_T_SIZE);
        write_to_file(cursor->file, &(last_block), UINT32_T_SIZE);
    }
    println("Finis");
}

void update_element(Cursor* cursor, void* old_element, void* new_element, uint64_t (*get_size_of_element)(void *), uint64_t size_of_sturcture, void *name_type, TypeOfElement type, void *(*read_element)(void *, MetaTable *, void *, uint64_t *), bool (*comparator)(void *, void *), void (*write_to_file_element)(Cursor *, void *)) {
    delete_element(cursor, old_element, get_size_of_element, size_of_sturcture, name_type, type, read_element, comparator);
    create_element(cursor, new_element, write_to_file_element, name_type, type, get_size_of_element);
}

void read_all_elements_by_type(Cursor* cursor, TypeOfElement type) {
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
            if (type == META) {
                // print_type(metaPage);
            } else if (metaPage->type == type) {
                switch (type) {
                    case NODE:
                        read_element_by_type(cursor, metaPage, sizeof(Node), read_node);
                        break;
                    case RELATIONSHIP:
                        read_element_by_type(cursor, metaPage, sizeof(Relationship), read_relationship);
                        break;
                    case PROPERTY:
                        read_element_by_type(cursor, metaPage, sizeof(Property), read_property);
                        break;
                    case META:
                        break;
                }
            }
        }
        page_num = pageHeader->next_block;
    } while (page_num != 0);
}

void create_meta_table(Cursor* cursor) {
    MetaTable data[] = {
        {NODE, 0, 0, "person"},
        {NODE, 0, 0, "pc"},
        { RELATIONSHIP, 0, 0, "play"},
        { RELATIONSHIP, 0, 0, "brothe"},
        {PROPERTY, 0, 0, "proper"},
    };

    int count_to_write = sizeof(data) / META_TABLE_SIZE;
    for (int i = 0; i < count_to_write; i++) {
        create_type(cursor, &(data[i]));
    }
    flush_page(cursor);
}

void create_nodes(Cursor* cursor) {
    Node nodes[] = {
        {"person", 9, "sasaovch"},
        { "person", 7, "andrey"},
        { "pc", 5, "asus"},
        { "pc", 4, "mac"},
        {"person", 9, "sasaovch1"},
        { "person", 7, "andrey2"},
    };

    int count_to_write = sizeof(nodes) / sizeof(Node);
    for (int i = 0; i < count_to_write; i++) {
        create_element(cursor, &(nodes[i]), write_node_to_file, nodes[i].name_type, NODE, get_size_of_node);
    }
}

void create_relationships(Cursor* cursor) {
    Relationship relationships[] = {
        {"play", 8, "sasaovch", 3, "mac"},
        {"play", 6, "andrey", 4, "asus"},
        {"brothe",8, "sasaovch", 3, "and"},
        {"brothe", 6, "andrey", 4, "sasa"},
    };
    
    int count_to_write = sizeof(relationships) / sizeof(Relationship);
    for (int i = 0; i < count_to_write; i++) {
        create_element(cursor, &(relationships[i]), write_relationship_to_file, relationships[i].name_type, RELATIONSHIP, get_size_of_relationship);
    }
}

void create_property(Cursor* cursor) {
    Property property[] = {
        {8, "sasaovch", 3, "age", INT, 2, "20"},
        {6, "andrey", 3, "age", INT, 2, "14"},
        {3, "mac", 5, "color", STRING, 6, "silver"},
        {4, "asus", 5, "color", STRING, 5, "black"}
    };

        
    int count_to_write = sizeof(property) / sizeof(Property);
    for (int i = 0; i < count_to_write; i++) {
        create_element(cursor, &(property[i]), write_property_to_file, "proper", PROPERTY, get_size_of_property);
    }
}

void delete_meta_table(Cursor* cursor) {
    MetaTable data[] = {
        {NODE, 0, 0, "person"},
        {NODE, 0, 0, "pc"},
        { RELATIONSHIP, 0, 0, "play"},
        { RELATIONSHIP, 0, 0, "brothe"},
        {PROPERTY, 0, 0, "proper"},
    };

    int count_to_write = sizeof(data) / META_TABLE_SIZE;
    for (int i = 0; i < count_to_write; i++) {
        delete_type(cursor, &(data[i]));
    }
}

bool compare_nodes(void* nd_1, void* nd_2) {
    Node* node_1 = (Node*) nd_1;
    Node* node_2 = (Node*) nd_2;
    return (strcmp(node_1->name, node_2->name) == 0) && (node_1->name_type == node_2->name_type) == 0;
}

bool compare_relationship(void* rl_1, void* rl_2) {
    Relationship* relationship_1 = (Relationship*) rl_1;
    Relationship* relationship_2 = (Relationship*) rl_2;
    return (strcmp(relationship_1->parent, relationship_2->parent) == 0) && (relationship_1->name_type == relationship_2->name_type) == 0 && (strcmp(relationship_1->child, relationship_2->child) == 0);
}

bool compare_property(void* pr_1, void* pr_2) {
    Property* property_1 = (Property*) pr_1;
    Property* property_2 = (Property*) pr_2;
    return (strcmp(property_1->subject, property_2->subject) == 0) && (property_1->key == property_2->key) == 0;
}

void delete_nodes(Cursor* cursor) {
    Node nodes[] = {
        { "pc", 4, "mac"},
        { "person", 7, "andrey"},
        {"person", 9, "sasaovch"},
        { "pc", 5, "asus"},
    };

    int count_to_write = sizeof(nodes) / sizeof(Node);
    for (int i = 0; i < count_to_write; i++) {
        delete_element(cursor, &(nodes[i]), get_size_of_node, sizeof(Node), nodes[i].name_type, NODE, read_node, compare_nodes);
    }
}

void delete_relationships(Cursor* cursor) {
    Relationship relationships[] = {
        {"play", 8, "sasaovch", 3, "mac"},
        {"play", 6, "andrey", 4, "asus"},
        {"brothe",8, "sasaovch", 3, "and"},
        {"brothe", 6, "andrey", 4, "sasa"},
    };
    
    int count_to_write = sizeof(relationships) / sizeof(Relationship);
    for (int i = 0; i < count_to_write; i++) {
        delete_element(cursor, &(relationships[i]), get_size_of_relationship, sizeof(Relationship), relationships[i].name_type, RELATIONSHIP, read_relationship, compare_relationship);
    }
}

void delete_property(Cursor* cursor) {
    Property property[] = {
        {8, "sasaovch", 3, "age", INT, 2, "20"},
        {6, "andrey", 3, "age", INT, 2, "14"},
        {3, "mac", 5, "color", STRING, 6, "silver"},
        {4, "asus", 5, "color", STRING, 5, "black"}
    };

        
    int count_to_write = sizeof(property) / sizeof(Property);
    for (int i = 0; i < count_to_write; i++) {
        //FIXME: запретить менять название у проперти
        delete_element(cursor, &(property[i]), get_size_of_property, sizeof(Property), "proper", PROPERTY, read_property, compare_property);
    }
}

void update_meta_table(Cursor* cursor) {
    MetaTable old_data[] = {
        {NODE, 0, 0, "person"},
        {NODE, 0, 0, "pc"},
        { RELATIONSHIP, 0, 0, "play"},
        { RELATIONSHIP, 0, 0, "brothe"},
        {PROPERTY, 0, 0, "proper"},
        {NODE, 0, 0, "human"},
        {NODE, 0, 0, "noteb"},
    };

    MetaTable new_data[] = {
        {NODE, 0, 0, "human"},
        {NODE, 0, 0, "noteb"},
        { RELATIONSHIP, 0, 0, "game"},
        { RELATIONSHIP, 0, 0, "cibli"},
        {PROPERTY, 0, 0, "some"},
        {NODE, 0, 0, "person"},
        {NODE, 0, 0, "pc"},
    };

    int count_to_write = sizeof(old_data) / META_TABLE_SIZE;
    for (int i = 0; i < count_to_write; i++) {
        update_type(cursor, &(old_data[i]), &(new_data[i]));
    }
}

void update_nodes(Cursor* cursor) {
    Node old_nodes[] = {
        {"person", 9, "sasaovch"},
        { "person", 7, "andrey"},
        { "pc", 5, "asus"},
        { "pc", 4, "mac"},
    };

    Node new_nodes[] = {
        {"person", 6, "asasa"},
        { "person", 7, "poppop"},
        { "pc", 5, "ASUS"},
        { "pc", 4, "pro"},
    };

    int count_to_write = sizeof(old_nodes) / sizeof(Node);
    for (int i = 0; i < count_to_write; i++) {
        update_element(cursor, &(old_nodes[i]), &(new_nodes[i]), get_size_of_node, sizeof(Node), old_nodes[i].name_type, NODE, read_node, compare_nodes, write_node_to_file); 
    }
}

void update_relationships(Cursor* cursor) {
    Relationship old_relationships[] = {
        {"play", 9, "sasaovch", 4, "mac"},
        {"play", 7, "andrey", 5, "asus"},
        {"brothe",9, "sasaovch", 4, "and"},
        {"brothe", 7, "andrey", 5, "sasa"},
    };
    //FIXME: проверка, что есть такая сущность или создавать новую
    Relationship new_relationships[] = {
        {"play", 5, "asas", 4, "mac"},
        {"play", 7, "andrey", 5, "asus"},
        {"brothe",9, "sasaovch", 4, "and"},
        {"brothe", 7, "andrey", 5, "sasa"},
    };
    
    int count_to_write = sizeof(old_relationships) / sizeof(Relationship);
    for (int i = 0; i < count_to_write; i++) {
        update_element(cursor, &(old_relationships[i]), &(new_relationships[i]), get_size_of_relationship, sizeof(Relationship), old_relationships[i].name_type, RELATIONSHIP, read_relationship, compare_relationship, write_relationship_to_file); 
    }
}

void update_property(Cursor* cursor) {
    Property old_property[] = {
        {9, "sasaovch", 4, "age", INT, 3, "20"},
        {7, "andrey", 4, "age", INT, 3, "14"},
        {4, "mac", 6, "color", STRING, 7, "silver"},
        {5, "asus", 6, "color", STRING, 6, "black"}
    };
    
    Property new_property[] = {
        {9, "sasaovch", 4, "age", INT, 3, "30"},
        {7, "andrey", 4, "age", INT, 3, "24"},
        {4, "mac", 6, "color", STRING, 5, "gold"},
        {5, "asus", 6, "color", STRING, 7, "yellow"}
    };

        
    int count_to_write = sizeof(old_property) / sizeof(Property);
    for (int i = 0; i < count_to_write; i++) {
        update_element(cursor, &(old_property[i]), &(new_property[i]), get_size_of_property, sizeof(Property), "proper", PROPERTY, read_property, compare_property, write_property_to_file); 
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Not enough parameters\n");
        exit(EXIT_FAILURE);
    }

    char* filename = argv[1];
    Cursor* cursor = db_open(filename);

    create_meta_table(cursor);
    create_nodes(cursor);    
    create_relationships(cursor);    
    create_property(cursor);

    read_all_elements_by_type(cursor, META);
    read_all_elements_by_type(cursor, NODE);
    read_all_elements_by_type(cursor, RELATIONSHIP);
    read_all_elements_by_type(cursor, PROPERTY);
    read_all_elements_by_type(cursor, META);

    // update_meta_table(cursor);
    read_all_elements_by_type(cursor, META);

    // delete_nodes(cursor);    
    // delete_relationships(cursor);    
    // delete_property(cursor);

    create_nodes(cursor);
    update_nodes(cursor);
    create_relationships(cursor);  
    update_relationships(cursor);  
    create_property(cursor);
    update_property(cursor);

    // delete_meta_table(cursor);
}
;