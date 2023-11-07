#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    uint64_t size;
    uint64_t top;
    bool is_empty;
    uint64_t *items;
} Stack;

Stack *new_stack(uint64_t initial_size);

bool is_empty(Stack *stack);

bool not_empty(Stack *stack);

void resize(Stack *stack, uint64_t new_size);

void push(Stack *stack, uint64_t value);

uint64_t pop(Stack *stack);

void destroy(Stack *stack);

bool stack_contains(Stack *stack, uint64_t value);
