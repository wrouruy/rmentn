#include <stdio.h>

void dry_run_message(const char* path) {
    printf("\033[90mfound:\033[0m %s\n", path);
}