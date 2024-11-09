#ifndef __SCOPE_H__
#define __SCOPE_H__


#include "field.h"

typedef struct {
} scope;

scope *scope_new() {
    scope *s = new(scope);
    return s;
}

void *scope_free(scope *s) {
    free(s);
}

#endif