#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "worker.h"

// worker thread entry point
void *worker(void *arg) {
    Context *ctx = arg;

    while (1) {
        pthread_mutex_lock(&ctx->mutex);

        // wait for work until the search finishes
        while (!ctx->tasks.head && ctx->pending > 0) {
            pthread_cond_wait(&ctx->cond, &ctx->mutex);
        }

        // no work left; workers may exit
        if (ctx->pending == 0) {
            pthread_mutex_unlock(&ctx->mutex);
            break;
        }

        char *path = dequeue(&ctx->tasks);

        pthread_mutex_unlock(&ctx->mutex);

        // scan without holding the mutex
        scan_directory(ctx, path);

        free(path);

        // current task finished
        pthread_mutex_lock(&ctx->mutex);

        ctx->pending--;

        if (ctx->pending == 0)
            pthread_cond_broadcast(&ctx->cond);

        pthread_mutex_unlock(&ctx->mutex);
    }

    return NULL;
}