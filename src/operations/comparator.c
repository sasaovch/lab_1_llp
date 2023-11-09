#include "../../include/operations/comparator.h"

#include <stdint.h>
#include <string.h>

bool is_less(const void *v1, const void *v2, ValueType type) {
    switch (type) {
        case INT: {
            return *((uint32_t*) v1) < *((uint32_t*) v2);
        }
        case FLOAT: {
            return *((float*) v1) < *((float*) v2);
        }
        case BOOL: {
            return *((bool*) v1) < *((bool*) v2);
        }
        case STRING: {
            return strcmp(v1, v2) < 0;
        }
        case VOID: return false;
    }
}

bool is_greater(const void *v1, const void *v2, ValueType type) {
    switch (type) {
        case INT: {
            return *((uint32_t*) v1) > *((uint32_t*) v2);
        }
        case FLOAT: {
            return *((float*) v1) > *((float*) v2);
        }
        case BOOL: {
            return *((bool*) v1) > *((bool*) v2);
        }
        case STRING: {
            return strcmp(v1, v2) > 0;
        }
        case VOID: return false;
    }
}

bool is_equal(const void *v1, const void *v2, ValueType type) {
    switch (type) {
        case INT: {
            return *((uint32_t*) v1) == *((uint32_t*) v2);
        }
        case FLOAT: {
            return *((float*) v1) == *((float*) v2);
        }
        case BOOL: {
            return *((bool*) v1) == *((bool*) v2);
        }
        case STRING: {
            return strcmp(v1, v2) == 0;
        }
        case VOID: return false;
    }
}

bool return_true(const void *d, const void *n) {
    (void) d;
    (void) n;
    return true;
}
