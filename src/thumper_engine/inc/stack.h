#ifndef THUMPERD_STACK_H
#define THUMPERD_STACK_H

#include <stdint.h>

typedef struct {
    uint64_t stack_len;
    uint64_t *stack_base;
    uint64_t stack_pointer;
} te_stack_t;

te_stack_t *gen_stack(uint64_t size);

void push(te_stack_t *stack, uint64_t val);

uint64_t pop(te_stack_t *stack);

void free_stack(te_stack_t *stack);

#endif //THUMPERD_STACK_H
