#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "worker.h"
#include "../path/path.h"

void add_task(Context *ctx, char *path) {
    pthread_mutex_lock(&ctx->mutex);

    enqueue(&ctx->tasks, path);
    ctx->pending++;

    pthread_cond_signal(&ctx->cond);

    pthread_mutex_unlock(&ctx->mutex);
}

void scan_directory(Context *ctx, char *path) {
    DIR *dir = opendir(path);

    if (!dir)
        return;

    struct dirent *entry;

    while ((entry = readdir(dir))) {
        if (!strcmp(entry->d_name, ".") ||
            !strcmp(entry->d_name, ".."))
            continue;

        char *full_path = join_path(path, entry->d_name);
        if (!full_path)
            continue;

        struct stat st;

        if (lstat(full_path, &st) != 0) {
            free(full_path);
            continue;
        }

        /*
        if (!S_ISDIR(st.st_mode)) {
            free(full_path);
            continue;
        }
        */

        // found a matching directory
        if (!strcmp(entry->d_name, ctx->target)) {
            add_result(ctx, strdup(full_path));

            free(full_path);
            continue;
        }

        // hand the child directory to a worker
        add_task(ctx, full_path);
    }

    closedir(dir);
}