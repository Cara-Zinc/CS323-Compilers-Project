#ifndef __STRUCT_H__
#define __STRUCT_H__

#include "list.h"
#include "util.h"
#include "field.h"
#include "func.h"

// definition of a struct
typedef struct {
    char *name; // name of the struct
    list *fields; // fields of the struct
    list *funcs; // functions of the struct
} struct_def;

// create a new struct definition
struct_def *struct_def_new(char *name) {
    struct_def *s = new(struct_def);
    s->name = name;
    s->fields = list_new(field_def);
    s->funcs = list_new(func_def);
}

// free a struct definition
void struct_def_free(struct_def *s) {
    free(s->name);
    list_free(s->fields);
    list_free(s->funcs);
    free(s);
}

// Let's call it Structra! LGTM!
#endif