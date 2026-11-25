#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *join_path(const char *parent, const char *name) {
    size_t len = strlen(parent) + strlen(name) + 2;

    char *path = malloc(len);
    if (!path)
        return NULL;

    snprintf(path, len, "%s/%s", parent, name);

    return path;
}
