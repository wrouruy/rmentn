#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

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
            case 'j': {
                size_t worker = 0;
                size_t start = i + 1;

                for (i = start; isdigit((unsigned char)s[i]); i++) {
                    size_t digit = (size_t)(s[i] - '0');
                    if (worker > (SIZE_MAX - digit) / 10)
                        raise_error("'-j' value is too large");
                    worker = worker * 10 + digit;
                }

                if (i == start)
                    raise_error("'-j' requires a number");

                f->worker = worker;
                i--; // let the outer loop advance past the last digit
                break;
            }

            default:
                raise_error("unknown flag '-%c'\n", s[i]);
        }
    }
}

Flags parse_flags(int argc, char **argv) {
    Flags f = {0};
    
    f.worker = 3;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            parse_subflag(argv[i], &f);
        }
    }
    
    if (f.interact && f.auto_confirm)
        raise_error("an unexpected combination of flags: '-i' and '-y'");

    if (f.interact && f.verbose)
        raise_error("an unexpected combination of flags: '-i' and '-v'");

    if (f.dry_run && ( f.interact || f.auto_confirm || f.verbose ))
        raise_error("use the '-n' flag without '-i', '-y', or '-v'");

    if (!f.directories && !f.files) {
        f.directories = true;
        f.files = true;
    }

    return f;
}