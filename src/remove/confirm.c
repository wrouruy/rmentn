#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "remove.h"

static const char* const types_to_str[] = {
    "directory",
    "regular file"
};

bool rm_confirm(const char* path, file_type_t file_type) {
    char input[16];
    size_t types_count = sizeof(types_to_str) / sizeof(types_to_str[0]);

    const char* type_str = ((size_t)file_type < types_count) 
        ? types_to_str[file_type] 
        : "entry";

    while (true) {
        printf("remove %s '%s'? (y/n) ", type_str, path);
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
            return false;

        if (strchr(input, '\n') == NULL) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }

        char* p = input;
        while (isspace((unsigned char)*p)) p++;

        if (*p == '\0')
            return false;

        char* end = p + strlen(p) - 1;
        while (end > p && isspace((unsigned char)*end)) {
            *end-- = '\0';
        }
        for (char* c = p; *c; ++c) {
            *c = (char)tolower((unsigned char)*c);
        }

        if (strcmp(p, "y") == 0 || strcmp(p, "yes") == 0)
            return true;

        if (strcmp(p, "n") == 0 || strcmp(p, "no") == 0)
            return false;

        printf("please enter 'y' or 'n'\n");
    }
}