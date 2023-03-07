#include "inc/stack.h"

#include <stdlib.h>

stack_t *gen_stack(uint64_t size) {
    stack_t *stack = malloc(sizeof(stack_t));
    if (stack == NULL) return NULL;

    stack->stack_base = malloc(size * sizeof(uint64_t));
    if (stack->stack_base == NULL) {
        free(stack);
        return NULL;
    }

    stack->stack_len = size;
    stack->stack_pointer = 0;

    return stack;
}

void push(stack_t *stack, uint64_t val) {
    stack->stack_base[stack->stack_pointer] = val;
    stack->stack_pointer++;
    if (stack->stack_pointer >= stack->stack_len) stack->stack_pointer = 0;
}

uint64_t pop(stack_t *stack) {
    stack->stack_pointer--;
    return stack->stack_base[stack->stack_pointer];
}

void free_stack(stack_t *stack) {
    free(stack->stack_base);
    free(stack);
}