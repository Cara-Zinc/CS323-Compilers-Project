#ifndef __STRUCT_H__
#define __STRUCT_H__

#include "list.h"
#include "util.h"
#include "field.h"
#include "func.h"

typedef struct {
    char *name;
    list *fields;
    list *funcs;
} struct_def;

struct_def *struct_def_new(char *name) {
    struct_def *s = new(struct_def);
    s->name = name;
    s->fields = list_new(field_def);
    s->funcs = list_new(func_def);
}

void struct_def_free(struct_def *s) {
    free(s->name);
    list_free(s->fields);
    list_free(s->funcs);
    free(s);
}

// Let's call it Structra! 
#endif