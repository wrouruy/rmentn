#include "worker.h"

void add_result(Context *ctx, char *path) {
    pthread_mutex_lock(&ctx->mutex);

    enqueue(&ctx->results, path);

    pthread_mutex_unlock(&ctx->mutex);
}