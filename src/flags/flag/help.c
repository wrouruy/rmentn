#include <stdio.h>
#include <stdlib.h>

void help_message(const char* utility_name) {
    printf(
        "usage: %s <directory> <name> <flags>\n"
        "-h : prints this message\n"
        "-i : interaction removal\n"
        "-y : auto-confirm\n"
        "-d : deletes directories\n"
        "-f : deletes files\n"
        "-v : (verbose) prints deleted files and directories\n"
        "-n : (dry-run) displays files and directories that can be deleted\n"
        "-j<int> : sets the number of threads to use\n",
        utility_name
    );
    exit(0);
}