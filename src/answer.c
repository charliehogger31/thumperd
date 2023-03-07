#include "inc/answer.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "thumper_engine/inc/thumper.h"

char *load_file(const char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) return NULL;
    fseek(file, 0, SEEK_END);
    size_t len = ftell(file);
    rewind(file);
    char *f_con = malloc(len+1);
    if (f_con == NULL) {
        fclose(file);
        return NULL;
    }
    memset(f_con, 0, len+1);
    fread(f_con, 1, len, file);
    fclose(file);
    return f_con;
}

enum MHD_Result answer_to_connection(void *cls, struct MHD_Connection *connection,
                                     const char *url,
                                     const char *method, const char *version,
                                     const char *upload_data,
                                     size_t *upload_data_size, void **con_cls) {
    struct MHD_Response *response;
    int ret;

    const char *default_path = getenv("TE_DEF_PATH");
    if (default_path == NULL) {
        default_path = ".";
    }

    if (strcmp(url, "/") == 0) url = "/index.bfml";

    char *full_path = malloc(strlen(default_path) + strlen(url) + 1);
    full_path[strlen(default_path) + strlen(url)] = '\0';
    strcpy(full_path, default_path);
    strcat(full_path, url);

    char *full_file = load_file(full_path);

    if (full_file == NULL) {
        response = MHD_create_response_from_buffer(strlen("NOT FOUND"), "NOT FOUND", MHD_RESPMEM_PERSISTENT);
        ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
        return ret;
    }

    char *full_resp = NULL;

    if (strcmp(full_path+(strlen(full_path)-5), ".bfml") != 0) {
        size_t full_len = strlen(full_file);
        full_resp = malloc(full_len);
        memcpy(full_resp, full_file, full_len);
    } else {
        size_t file_len = strlen(full_file);
        for (int i = 0; i < file_len; i++) {
            if (full_file[i] == '$') {
                int next = i;
                for (int j = next + 1; j < file_len; j++) {
                    if (full_file[j] == '$') {
                        next = j;
                        break;
                    }
                }

                if (next == i) break; // TODO: proper error handling

                full_file[next] = '\0';

                char *adv_ptr = full_file + i + 1;

                char *program = malloc(strlen(adv_ptr)+1);
                memset(program, 0, strlen(adv_ptr));
                strcpy(program, adv_ptr);

                te_engine_state_t *state;
                state = te_generate_state(program);
                if (state == NULL) break;

                te_begin(state);

                full_file[i] = '\0';

                size_t resp_len =
                        strlen(full_file) + strlen((char *) state->out_buffer) + strlen(full_file + next + 1) + 1;
                full_resp = malloc(resp_len);
                if (full_resp == NULL) break;
                full_resp[resp_len-1] = '\0';
                strcpy(full_resp, full_file);
                strcat(full_resp, (char *) state->out_buffer);
                strcat(full_resp, full_file + next + 1);

                free((void *) state->program_buffer);
                te_free_state(state);
                free(full_file);
                free(full_path);

                break;
            }
        }
    }
    if (full_resp == NULL) {
        response = MHD_create_response_from_buffer(strlen("INTERNAL ERROR"), "INTERNAL ERROR", MHD_RESPMEM_PERSISTENT);
        ret = MHD_queue_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response);
        return ret;
    }

    response = MHD_create_response_from_buffer(strlen(full_resp),
                                               (void*)full_resp, MHD_RESPMEM_MUST_FREE);

    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
}