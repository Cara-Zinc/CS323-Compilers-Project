#ifndef __COPY_H__
#define __COPY_H__

#include <stdlib.h>

void *_mem_copy(size_t size, void *src);

#define mem_copy(type, src) ((type *) _mem_copy(sizeof(type), src))

char *str_copy(const char *src);

#endif