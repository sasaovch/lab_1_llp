#ifndef LAB1_STRUCT_H
#define LAB1_STRUCT_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define PAGE_SIZE 64
#define NAME_TYPE_SIZE 15

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
    char* body;
} Page;

typedef struct {
    File* file;
    uint32_t first_entity_block;
    uint32_t last_entity_block;
    uint32_t number_of_pages;
    Page* page;
} Cursor;

typedef struct {
    TypeOfElement type;
    uint32_t first_block;
    uint32_t last_block;
    uint32_t next_id;
    char name_type[NAME_TYPE_SIZE + 1];
} Entity;

typedef struct {
    uint32_t id;
    uint32_t name_length;
    char* name;
    char name_type[NAME_TYPE_SIZE + 1];
} Node;

typedef struct {
    uint32_t id;
    uint32_t parent_length;
    char* parent;
    uint32_t child_length;
    char* child;
    char name_type[NAME_TYPE_SIZE + 1];
} Relationship;

typedef struct {
    uint32_t id;
    uint32_t parent_id;
    uint32_t child_id;
    char name_type[NAME_TYPE_SIZE + 1];
} Relationship2;

typedef struct {
    uint32_t id;
    uint32_t subject_length;
    char* subject;
    uint32_t key_length;
    char* key;
    ValueType value_type;
    uint32_t value_length;
    char* value;
} Property;

typedef struct {
    uint32_t id;
    ValueType value_type;
    uint32_t subject_id;
    uint32_t key_length;
    uint32_t value_length;
    char* key;
    char* value;
} Property2;

typedef enum {
    ID = 0,
    NAME = 1,
    SUBJECT = 2,
    PARENT = 3,
    CHILD = 4
} Field;

typedef enum {
    MORE = 0,
    LESS = 1,
    EQUALS = 2
} Operator;

typedef struct {
    TypeOfElement type_element;
    Field field;
    Operator Operator;
    ValueType type_value;
    uint32_t value;
    char* string_value;
} Request;

// const uint32_t ZERO = 0;
// const uint64_t UINT32_T_SIZE = sizeof(uint32_t);
// const uint64_t CHAR_SIZE = sizeof(char);
// const uint64_t NODE_SIZE = sizeof(Node);
// const uint64_t PROPERTY_SIZE = sizeof(Property);
// const uint64_t RELATIONSHIP_SIZE = sizeof(Relationship);
// const uint64_t ENTITY_SIZE = sizeof(Entity);
// const uint64_t PAGE_HEADER_SIZE = sizeof(PageHeader);
// const uint64_t VALUE_TYPE_SIZE = sizeof(ValueType);
// const uint64_t TYPE_OF_ELEMENT_SIZE = sizeof(TypeOfElement);
// const uint64_t PAGE_BODY_SIZE = (PAGE_SIZE - PAGE_HEADER_SIZE);

#define ZERO 0
#define UINT32_T_SIZE sizeof(uint32_t)
#define CHAR_SIZE sizeof(char)
#define NODE_SIZE sizeof(Node)
#define PROPERTY_SIZE sizeof(Property2)
#define RELATIONSHIP_SIZE sizeof(Relationship2)
#define ENTITY_SIZE sizeof(Entity)
#define PAGE_HEADER_SIZE sizeof(PageHeader)
#define VALUE_TYPE_SIZE sizeof(ValueType)
#define TYPE_OF_ELEMENT_SIZE sizeof(TypeOfElement)
#define PAGE_BODY_SIZE (PAGE_SIZE - PAGE_HEADER_SIZE)

#endif //LAB1_STRUCT_H
