#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include "../path/path.h"
#include "remove.h"

static bool rm_dir(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return false;
    }

    struct stat st;
    struct dirent *entry;
    bool status = true;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char *fullpath = join_path(path, entry->d_name);
        if (fullpath == NULL) {
            status = false;
            break;
        }

        if (lstat(fullpath, &st) == -1) {
            perror("lstat");
            free(fullpath);
            status = false;
            break;
        }

        if (S_ISDIR(st.st_mode)) {
            if (rm_dir(fullpath) != 0) {
                status = false;
            }
        } else {
            if (unlink(fullpath) != 0) {
                perror("unlink");
                status = false;
            }
        }

        free(fullpath);
        if (status != true) break;
    }

    closedir(dir);

    if (status == true && rmdir(path) != 0) {
        perror("rmdir");
        return false;
    }

    return status;
}

bool rm(const char *path) {
    struct stat st;
    if (lstat(path, &st) == -1) {
        perror("lstat");
        return false;
    }

    if(!rm_confirm(path))
        return true;

    if (S_ISDIR(st.st_mode)) {
        return rm_dir(path);
    }

    if (unlink(path) != 0) {
        perror("unlink");
        return false;
    }

    return true;
}