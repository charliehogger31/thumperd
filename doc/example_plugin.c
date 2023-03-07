#include "../src/thumper_engine/inc/thumper_plugin_def.h"

void te_plugin_entry(engine_state_external_plugin_handoff_t *state) {
    state->buffer[state->buffer_pointer-1] = 'B';
}