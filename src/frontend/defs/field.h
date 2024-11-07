#ifndef __FIELD_H__
#define __FIELD_H__

#include "list.h"
#include "type.h"
#include "util.h"
#include <stdbool.h>

// field definition
typedef struct {
    char *name; // field name
    type_spec_def *type_spec; // field type specification
} field_def;

// create a new field definition
field_def *field_def_new(char *name, type_spec_def *type_spec) {
    field_def *res = new(field_def);
    res->name = name;
    res->type_spec = type_spec;
    return res;
}

// free a field definition
void field_def_free(field_def *f) {
    free(f->name);
    type_spec_def_free(f->type_spec);
    free(f);
}

// global field definition
typedef struct {
    char *name; // field name
    type_spec_def *type_spec; // field type specification
    void *value; // field initial value
} global_field_def;

// create a new global field definition
global_field_def *readonly_field_def_new(char *name, type_spec_def *type_spec, void *value) {
    global_field_def *res = new(global_field_def);
    res->name = name;
    res->type_spec = type_spec;
    res->value = value;
    return res;
}

// free a global field definition
void readonly_field_def_free(global_field_def *f) {
    free(f->name);
    type_spec_def_free(f->type_spec);
    free(f->value);
    free(f);
}

#endif