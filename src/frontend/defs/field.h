#ifndef __FIELD_H__
#define __FIELD_H__

#include "list.h"
#include "type.h"
#include "util.h"
#include <stdbool.h>

typedef struct {
    char *name;
    type_spec_def *type_spec;
} field_def;

field_def *field_def_new(char *name, type_spec_def *type_spec) {
    field_def *res = new(field_def);
    res->name = name;
    res->type_spec = type_spec;
    return res;
}

void field_def_free(field_def *f) {
    free(f->name);
    type_spec_def_free(f->type_spec);
    free(f);
}

typedef struct {
    char *name;
    type_spec_def *type_spec;
    void *value;
} global_field_def;

global_field_def *readonly_field_def_new(char *name, type_spec_def *type_spec, void *value) {
    global_field_def *res = new(global_field_def);
    res->name = name;
    res->type_spec = type_spec;
    res->value = value;
    return res;
}

void readonly_field_def_free(global_field_def *f) {
    free(f->name);
    type_spec_def_free(f->type_spec);
    free(f->value);
    free(f);
}

#endif