#include "../../include/data/stack.h"

#include "../../include/data/constants.h"
#include "utils/io_utils.h"
#include "utils/logger.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

Stack *new_stack(uint32_t initial_size) {
    Stack *stack = (Stack *) malloc(sizeof(Stack));
    stack->items = (uint32_t *)malloc(initial_size * UINT64_T_SIZE);
    stack->size = initial_size;
    stack->top = 0;
    stack->is_empty = true;
    return stack;
}

bool not_empty(Stack *stack) {
    return !(stack->is_empty);
}

bool is_empty(Stack *stack) {
    return stack->is_empty;
}

void resize(Stack *stack, uint32_t new_size) {
    uint32_t *new_items = (uint32_t *)realloc(stack->items, new_size * UINT64_T_SIZE);
    if (new_items == NULL) {
        LOG_ERROR("Failed to resize the stack", "");
        exit(1);
    }
    stack->items = new_items;
    stack->size = new_size;
}

void push(Stack *stack, uint32_t value) {
    if (stack->top == stack->size - 1) {
        resize(stack, stack->size * 2);
    }
        stack->items[stack->top] = value;
    stack->top++;
    stack->is_empty = false;
}

uint32_t pop(Stack *stack) {
    if (is_empty(stack)) {
        LOG_INFO("Stack underflow when trying to pop a value", "");
        error_exit(ERROR_EXIT_CODE, "Stack underflow when trying to pop a value");
    }
    stack->top--;
    uint32_t value = stack->items[stack->top];
    stack->items[stack->top] = 0;

    if (stack->top <= stack->size / 4) {
        resize(stack, stack->size / 2);
    }

    if (stack->top == 0) {
        stack->is_empty = true;
    }
    
    return value;
}

void destroy(Stack *stack) {
    free(stack->items);
    free(stack);
}

bool stack_contains(Stack *stack, uint32_t value) {
    for (uint32_t i = 0; i < stack->top; i++) {
        if (stack->items[i] == value) {
            return true;
        }
    }
    return false;
}
