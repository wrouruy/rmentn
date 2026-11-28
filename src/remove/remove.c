#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

#include "../path/path.h"
#include "../flags/flags.h"
#include "../flags/flag/flag.h"
#include "remove.h"

static bool rm_dir(const char *path, const bool verbose) {
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
            if (rm_dir(fullpath, verbose) != 0) {
                status = false;
            }
            if (verbose) verbose_message(fullpath);
                
        } else {
            if (unlink(fullpath) != 0) {
                perror("unlink");
                status = false;
            }
            if (verbose) verbose_message(fullpath);
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

bool rm(const char *path, const Flags flags) {
    struct stat st;
    if (lstat(path, &st) == -1) {
        perror("lstat");
        return false;
    }

    if (S_ISDIR(st.st_mode) && (flags.auto_confirm || rm_confirm(path, file_type_regular)))
        return rm_dir(path, flags.verbose);

    if (S_ISREG(st.st_mode) && (flags.auto_confirm || rm_confirm(path, file_type_regular))) {
        if (unlink(path) != 0) {
            perror("unlink");
            return false;
        }
        if (flags.verbose) verbose_message(path);
    }

    return true;
}