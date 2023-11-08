#pragma once

#include "../data/cursor.h"

void prepare_stack(Cursor *cursor);

void push_in_stack(Cursor *cursor, uint32_t value);

uint32_t pop_from_stack(Cursor *cursor);
