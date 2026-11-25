#include <stdio.h>
#include <stdlib.h>
#include "flags.h"

static bool parse_subflag(char* s, Flags* f) {
    if (s[1] == '\0') {
        fprintf(stderr, "err: empty flag '-'\n");
        exit(1);
    }

    for (size_t i = 1; s[i] != '\0'; i++) {
        switch (s[i]) {
            case 'h': f->help = true; break;
            case 'i': f->interact = true; break;
            case 'y': f->auto_confirm = true; break;
            case 'd': f->directories = true; break;
            case 'f': f->files = true; break;
            case 'v': f->verbose = true; break;
            case 'n': f->dry_run = true; break;
            default:
                fprintf(stderr, "err: unknown flag '-%c'\n", s[i]);
                exit(1);
                break;
        }
    }
}

Flags parse_flags(int argc, char **argv) {
    Flags f = {0};
    
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            parse_subflag(argv[i], &f);
        }
    }
    if (f.interact && f.auto_confirm) {
        printf("err: unexpected ")
    }

    return res;
}