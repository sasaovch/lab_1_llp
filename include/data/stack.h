#pragma once

#include "./page.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    uint32_t size;
    uint32_t top;
    bool is_empty;
    uint32_t *items;
    Page *page;
} Stack;

Stack *new_stack(uint32_t initial_size);

bool is_empty(Stack *stack);

bool not_empty(Stack *stack);

void resize(Stack *stack, uint32_t new_size);

void push(Stack *stack, uint32_t value);

uint32_t pop(Stack *stack);

void destroy(Stack *stack);

bool stack_contains(Stack *stack, uint32_t value);
