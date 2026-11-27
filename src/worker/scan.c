#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "worker.h"
#include "../path/path.h"
#include "../flags/flags.h"
#include "../wildcard/wildcard.h"

void add_task(Context *ctx, char *path) {
    pthread_mutex_lock(&ctx->mutex);

    enqueue(&ctx->tasks, path);
    ctx->pending++;

    pthread_cond_signal(&ctx->cond);

    pthread_mutex_unlock(&ctx->mutex);
}

void scan_directory(Context *ctx, char *path, const Flags flags) {
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

        bool is_dir = S_ISDIR(st.st_mode);
        bool is_file = S_ISREG(st.st_mode);

        bool matches_type =
            (!flags.directories && !flags.files) ||
            (flags.directories && is_dir) ||
            (flags.files && is_file);

        if (matches_type && wildcard_match(ctx->target, entry->d_name, strlen(ctx->target), strlen(entry->d_name))) {
            char *result_path = strdup(full_path);

            if (result_path)
                add_result(ctx, result_path);
        }

        if (is_dir)
            add_task(ctx, full_path);
        else
            free(full_path);
    }

    closedir(dir);
}