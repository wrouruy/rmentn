#pragma once

#include <stdbool.h>
#include <stddef.h>

bool wildcard_match(char* pattern, char* text, size_t pattern_len, size_t text_len);