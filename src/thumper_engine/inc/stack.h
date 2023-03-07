#ifndef THUMPERD_STACK_H
#define THUMPERD_STACK_H

#include <stdint.h>

typedef struct {
    uint64_t stack_len;
    uint64_t *stack_base;
    uint64_t stack_pointer;
} stack_t;

stack_t *gen_stack(uint64_t size);

void push(stack_t *stack, uint64_t val);

uint64_t pop(stack_t *stack);

void free_stack(stack_t *stack);

#endif //THUMPERD_STACK_H
