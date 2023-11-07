#include "../../include/data/stack.h"

#include "../../include/data/constants.h"

#include <stdint.h>
#include <stdlib.h>
//FIXME: ggod
Stack *new_stack(uint64_t initial_size) {
    Stack *stack = (Stack *) malloc(sizeof(Stack));
    stack->items = (uint64_t *)malloc(initial_size * UINT64_T_SIZE);
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

void resize(Stack *stack, uint64_t new_size) {
    uint64_t *new_items = (uint64_t *)realloc(stack->items, new_size * UINT64_T_SIZE);
    if (new_items == NULL) {
        printf("Failed to resize the stack\n");
        exit(1);
    }
    stack->items = new_items;
    stack->size = new_size;
}

void push(Stack *stack, uint64_t value) {
    if (stack->top == stack->size - 1) {
        resize(stack, stack->size * 2);
    }
    stack->items[stack->top] = value;
    stack->top++;
    stack->is_empty = false;
}

uint64_t pop(Stack *stack) {
    if (is_empty(stack)) {
        printf("Stack underflow when trying to pop a value\n");
        return -1;
    }
    stack->top--;
    uint64_t value = stack->items[stack->top];
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

bool stack_contains(Stack *stack, uint64_t value) {
    for (uint64_t i = 0; i < stack->top; i++) {
        if (stack->items[i] == value) {
            return true;
        }
    }
    return false;
}
