#ifndef __SCOPE_H__
#define __SCOPE_H__


#include "field.h"

typedef struct {
} scope;

scope *pscope_new() {
    scope *s = new(scope);
    return s;
}

void *pscope_free(scope *s) {
    free(s);
}

#endif