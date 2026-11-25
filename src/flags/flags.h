#pragma once
#include <stdbool.h>

typedef struct {
    bool help;         // -h
    
    bool interact;     // -i
    bool auto_confirm; // -y 

    bool directories;  // -d
    bool files;        // -f

    bool verbose;      // -v

    bool dry_run;      // -n
} Flags;

Flags parse_flags(int argc, char **argv);