#include "copy.h"

#include <string.h>

void *_mem_copy(size_t size, void *src) {
    void * dst = malloc(size);
    memcpy(dst, src, size);
    return dst;
}

char *str_copy(char *src) {
    char *dst = malloc(strlen(src) + 1);
    strcpy(dst, src);
    return dst;
}