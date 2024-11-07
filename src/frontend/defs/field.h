#ifndef __FIELD_H__
#define __FIELD_H__

#include "list.h"
#include "type.h"
#include "util.h"
#include <stdbool.h>

typedef struct {
    char *name;
    type_id type;
    bool is_struct;
    bool is_array;
    size_t array_size;
} field_def;

field_def *field_def_new(char *name, type_id type) {
    field_def *res = new(field_def);
    res->name = name;
    res->type = type;
    res->is_struct = false;
    res->is_array = false;
    res->array_size = 0;
    return res;
}

typedef struct {
    char *name;
    type_id type;
    bool is_struct;
    bool is_array;
    size_t array_size;
    void *value;
} readonly_field_def;

readonly_field_def *readonly_field_def_new(char *name, type_id type, void *value) {
    readonly_field_def *res = new(readonly_field_def);
    res->name = name;
    res->type = type;
    res->is_struct = false;
    res->is_array = false;
    res->array_size = 0;
    res->value = value;
    return res;
}

#endif