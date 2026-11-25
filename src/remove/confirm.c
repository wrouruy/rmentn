#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

bool rm_confirm(const char* path) {
    char input[16]; 

    while (true) {
        printf("remove regular file '%s'? ", path);
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

        char answer = (char)tolower((unsigned char)*p);
        if (answer == 'y')
            return true;

        if (answer == 'n')
            return false;

        printf("please enter 'y' or 'n'\n");
    }
}