#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "worker/worker.h"
#include "remove/remove.h"
#include "flags/flags.h"

static void dry_run_message(const char* path) {
    printf("\033[90mfound:\033[0m %s\n", path);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <directory> <name>\n", argv[0]);
        return 1;
    }
    Flags flags = parse_flags(argc, argv);

    Context ctx = {0};
    ctx.target = argv[2];
    ctx.flags = flags;

    pthread_mutex_init(&ctx.mutex, NULL);
    pthread_cond_init(&ctx.cond, NULL);

    // seed the queue with the root directory
    enqueue(&ctx.tasks, strdup(argv[1]));
    ctx.pending = 1;

    pthread_t threads[WORKERS];

    // start workers
    for (int i = 0; i < WORKERS; i++) {
        pthread_create(&threads[i], NULL, worker, &ctx);
    }

    // wait for all workers
    for (int i = 0; i < WORKERS; i++) {
        pthread_join(threads[i], NULL);
    }

    // all workers are done; results can be read safely
    char *path;
    while ((path = dequeue(&ctx.results))) {
        if (flags.dry_run)
            dry_run_message(path);
        else rm(path, flags);

        free(path);
    }

    pthread_mutex_destroy(&ctx.mutex);
    pthread_cond_destroy(&ctx.cond);

    return 0;
}
