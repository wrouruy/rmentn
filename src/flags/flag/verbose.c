#include <stdio.h>

void verbose_message(const char* path) {
    printf("\033[90mdeleted: %s\033[0m\n", path);
}
