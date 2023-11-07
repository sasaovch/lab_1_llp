#pragma once

#include <stdbool.h>

extern const char *error_descriptions[];

bool check_is_null_arg(const void *arg, const char *name);

bool check_is_null_returned(const void *arg, const char *method_name);
