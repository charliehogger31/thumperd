#ifndef THUMPERD_THUMPER_H
#define THUMPERD_THUMPER_H

#include <stdint.h>
#include "stack.h"


typedef struct {
    uint64_t buffer_length;
    uint8_t* buffer;
    uint64_t buffer_pointer;
    uint64_t program_length;
    const char *program_buffer;
    uint64_t program_counter;
    stack_t *stack;
    uint8_t *out_buffer;
    uint64_t out_ptr;
    uint64_t out_len;
    uint8_t *in_buffer;
    uint64_t in_len;
    uint64_t in_ptr;
} te_engine_state_t;

void te_config_engine(uint64_t default_buffer, uint64_t default_stack, uint64_t default_out);

te_engine_state_t *te_generate_state(const char *program);

void te_begin(te_engine_state_t *state);

void te_free_state(te_engine_state_t *state);

#endif //THUMPERD_THUMPER_H
