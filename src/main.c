#include <stdio.h>
#include <microhttpd.h>
#include <string.h>

#include "inc/answer.h"

#define DEBUG_PORT 8888


int main() {
    struct MHD_Daemon *daemon;
    daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION, DEBUG_PORT, NULL, NULL,
                              &answer_to_connection, NULL, MHD_OPTION_END);

    getchar();

    MHD_stop_daemon(daemon);

    return 0;
}
