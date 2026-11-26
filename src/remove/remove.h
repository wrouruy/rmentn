#pragma once
#include <stdbool.h>
#include "../flags/flags.h"

typedef enum {
    file_type_dir,
    file_type_regular
} file_type_t;

bool rm(const char *path, const Flags flags);
bool rm_confirm(const char* path, file_type_t file_type);