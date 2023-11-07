#pragma once

#include "../data/enums.h"

#include <stdbool.h>

bool is_less(const void *v1, const void *v2, ValueType type);

bool is_greater(const void *v1, const void *v2, ValueType type);

bool is_equal(const void *v1, const void *v2, ValueType type);

bool return_true(const void *d, const void *n);
