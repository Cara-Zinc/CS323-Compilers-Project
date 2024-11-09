#ifndef __COPY_H__
#define __COPY_H__

#include <stdlib.h>
#include <string.h>

void * _mem_copy(size_t size, void *src) {
    void * dst = malloc(size);
    memcpy(dst, src, size);
    return dst;
}

#define mem_copy(type, src) ((type *) _mem_copy(sizeof(type), src))

char *str_copy(char *src) {
    char *dst = malloc(strlen(src) + 1);
    strcpy(dst, src);
    return dst;
}

#endif