#pragma once

#include "../data/enums.h"

#include <stdint.h>

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
    char *string_value;
} Request;
