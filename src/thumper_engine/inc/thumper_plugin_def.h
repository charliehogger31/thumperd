#ifndef THUMPERD_THUMPER_PLUGIN_DEF_H
#define THUMPERD_THUMPER_PLUGIN_DEF_H

#include <stdint.h>

typedef struct {
    uint8_t *buffer;
    uint64_t buffer_length;
    uint64_t buffer_pointer;
    uint8_t state_test;
    uint8_t calling_vector;
} __attribute__((packed, aligned(sizeof(void*)))) engine_state_external_plugin_handoff_t;

typedef void(*thumper_plugin_entry_t)(engine_state_external_plugin_handoff_t *state);

#endif //THUMPERD_THUMPER_PLUGIN_DEF_H
