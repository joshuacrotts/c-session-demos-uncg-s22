#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

void *s_malloc(size_t sz) {
    void *data = NULL;
    if ((data = malloc(sz)) == NULL) {
        fprintf(stderr, "Failed to malloc block of size %zu\n", sz);
        exit(1);
    }
    return data;
}

bool streq(const char *s1, const char *s2) {
    return s1 == s2 || strcmp(s1, s2) == 0;
}
