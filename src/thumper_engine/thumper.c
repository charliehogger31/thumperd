#include "inc/thumper.h"

#include <stdlib.h>
#include <string.h>

static uint64_t def_buf_sz = 30000;
static uint64_t def_stack_sz = 100;
static uint64_t def_out_buffer = 1000;

void te_config_engine(uint64_t default_buffer, uint64_t default_stack, uint64_t default_out) {
    def_buf_sz = default_buffer;
    def_stack_sz = default_stack;
    def_out_buffer = default_out;
}

te_engine_state_t *te_generate_state(const char *program) {
    te_engine_state_t *state = malloc(sizeof(te_engine_state_t));
    if (state == NULL) return NULL;

    state->buffer_length = def_buf_sz;
    state->buffer_pointer = 0;
    state->buffer = malloc(def_buf_sz);
    state->program_buffer = program;
    state->program_counter = 0;
    state->program_length = strlen(program);
    state->stack = gen_stack(def_stack_sz);
    state->out_buffer = malloc(def_out_buffer);
    state->out_ptr = 0;
    state->out_len = def_out_buffer;

    state->in_buffer = NULL;
    state->in_len = 0;
    state->in_ptr = 0; // User must initialize their own input system

    if (state->buffer == NULL || state->stack == NULL || state->out_buffer == NULL) {
        free(state);
        return NULL;
    }

    memset(state->out_buffer, 0, state->out_len);
    memset(state->buffer, 0, state->buffer_length);

    return state;
}

void te_begin(te_engine_state_t *state) {
    while (state->program_counter < state->program_length) {
        switch (state->program_buffer[state->program_counter]) {
            case '+':
                state->buffer[state->buffer_pointer]++;
                break;
            case '-':
                state->buffer[state->buffer_pointer]--;
                break;
            case '>':
                state->buffer_pointer++;
                if (state->buffer_pointer >= state->buffer_length)
                    state->buffer_pointer = 0;
                break;
            case '<':
                if (state->buffer_pointer == 0)
                    state->buffer_pointer = state->buffer_length-1;
                else
                    state->buffer_pointer--;
                break;
            case '.':
                state->out_buffer[state->out_ptr] = state->buffer[state->buffer_pointer];
                state->out_ptr++;
                if (state->out_ptr >= state->out_len-1) state->out_ptr = 0;
                break;
            case ',':
                if (state->in_buffer != NULL) {
                    state->buffer[state->buffer_pointer] = state->in_buffer[state->in_ptr];
                    state->in_ptr++;
                    if (state->in_ptr >= state->in_len) state->in_ptr = 0;
                } else state->buffer[state->buffer_pointer] = 0;
                break;
            case '[':
                if (state->buffer[state->buffer_pointer] == 0) {
                    int level = 0;
                    for (int i = 0; i < state->program_length; i++) {
                        if (state->program_buffer[i+state->program_counter] == '[') level++;
                        if (state->program_buffer[i+state->program_counter] == ']') {
                            if (level > 0) level--;
                            else state->program_counter += i;
                        }
                    }
                } else push(state->stack, state->program_counter);
                break;
            case ']':
                if (state->buffer[state->buffer_pointer] != 0)
                    state->program_counter = pop(state->stack)-1;
                else
                    pop(state->stack);
                break;
        }

        state->program_counter++;
    }
}

void te_free_state(te_engine_state_t *state) {
    free(state->buffer);
    free_stack(state->stack);
    free(state->out_buffer);
    free(state);
}