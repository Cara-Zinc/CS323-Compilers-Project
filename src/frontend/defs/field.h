#ifndef __FIELD_H__
#define __FIELD_H__

#include "type.h"
#include "util.h"

// field definition
typedef struct {
    char *name; // field name
    type_def *type_spec; // field type specification
} field_def;

// create a new field definition
field_def *field_def_new(char *name, type_def *type_spec) {
    field_def *res = new(field_def);
    res->name = name;
    res->type_spec = type_spec;
    return res;
}

// free a field definition
void field_def_free(field_def *f) {
    free(f->name);
    type_def_free(f->type_spec);
    free(f);
}

#endif