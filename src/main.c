#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <microhttpd.h>
#include <dlfcn.h>
#include <linux/limits.h>

#define DEBUG_PORT 8888 // Remove these debug values later pls

#define TE_MAX_PLUG 4096

#include "inc/answer.h"
#include "thumper_engine/inc/thumper.h"

// sig copied from answer.c
// to lazy to make header file
char *load_file(const char *path);

const char **load_line_by_line(const char *file_path) {
    FILE *line_file = fopen(file_path, "r");
    if (line_file == NULL) return NULL;

    char **lines = NULL;
    size_t l_buf_len = 0;
    char line[PATH_MAX];
    while (fgets(line, PATH_MAX, line_file)) {
        line[strlen(line)-1] = '\0';
        lines = realloc(lines, l_buf_len + sizeof(char*));
        l_buf_len += sizeof(char*);
        lines[(l_buf_len-sizeof(char*))/sizeof(char*)] = malloc(strlen(line)+1); // (l_buf_len-sizeof(char*))/sizeof(char*) l_buf_len is in bytes,\
                                                                                                                                        must resolve to index with math
        lines[(l_buf_len-sizeof(char*))/sizeof(char*)][strlen(line)] = '\0';
        strcpy(lines[(l_buf_len-sizeof(char*))/sizeof(char*)], line);
    }

    lines = realloc(lines, l_buf_len+(sizeof(char*)*3));
    lines[l_buf_len/sizeof(char*)] = NULL;

    return (const char **)lines;
}

int main() {
    const char *list_file_path = getenv("TE_LIST_FILE");
    thumper_plugin_entry_t *plugins = malloc(TE_MAX_PLUG * sizeof(thumper_plugin_entry_t));
    memset(plugins, 0, TE_MAX_PLUG * sizeof(thumper_plugin_entry_t));
    if (list_file_path != NULL) {
        const char ** lines = load_line_by_line(list_file_path);
        int i = 0;
        while (lines[i] != NULL) {
            printf("[INFO] Loading plugin: %s\n", lines[i]);
            void *handle = dlopen(lines[i], RTLD_NOW | RTLD_NODELETE);
            if (handle == NULL) {
                printf("[FATAL]: dlopen failed on %s\n", lines[i]);
                perror("dlopen");
                return 1;
            }
            thumper_plugin_entry_t plugin = dlsym(handle, "te_plugin_entry");
            if (plugin == NULL) {
                printf("[FATAL]: dlsym failed on %s\n", lines[i]);
                return 1;
            }
            plugins[i] = plugin;
            i++;
        }

        te_register_iv_map(plugins);
    }

    struct MHD_Daemon *daemon;
    daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION, DEBUG_PORT, NULL, NULL,
                              &answer_to_connection, NULL, MHD_OPTION_END);

    getchar();

    MHD_stop_daemon(daemon);

    return 0;
}
