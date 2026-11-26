#include <stdio.h>
#include <stdlib.h>
#include "flags.h"
#include "../error/error.h"

static void parse_subflag(const char* s, Flags* f) {
    if (s[1] == '\0')
        raise_error("err: empty flag '-'");

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
    
    if (f.interact && f.auto_confirm)
        raise_error("an unexpected combination of flags: '-i' and '-y'");

    if (f.dry_run && ( f.interact || f.auto_confirm || f.verbose ))
        raise_error("use the '-n' flag without '-i', '-y', or '-v'");

    if (!f.directories && !f.files) {
        f.directories = true;
        f.files = true;
    }

    return f;
}