#include <stdbool.h>
#include <stddef.h>

bool wildcard_match(const char* pattern, const char* text, size_t pattern_len, size_t text_len) {
    size_t p = 0; // index for pattern
    size_t t = 0; // index for text
    size_t star_idx = (size_t)-1; // the position of the last '*' in the pattern
    size_t match_idx = 0; // the position in the text that corresponds to the char following '*'

    while (t < text_len) {
        if (p < pattern_len && (pattern[p] == '?' || pattern[p] == text[t])) {
            p++;
            t++;
        }
        else if (p < pattern_len && pattern[p] == '*') {
            star_idx = p;
            match_idx = t;
            p++;
        }
        else if (star_idx != (size_t)-1) {
            p = star_idx + 1;
            match_idx++;
            t = match_idx;
        }
        else
            return false;
    }

    while (p < pattern_len && pattern[p] == '*') {
        p++;
    }

    return p == pattern_len;
}