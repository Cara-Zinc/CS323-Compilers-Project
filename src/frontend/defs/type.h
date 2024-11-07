#ifndef __TYPE_H__
#define __TYPE_H__

#include <stddef.h>
#include <stdbool.h>
#include "util.h"

typedef size_t type_id;

// types of values are designed as follows
#define TYPE_VOID 0 // void type
#define TYPE_INT 1 // integer type
#define TYPE_FLOAT 2 // floating point type
#define TYPE_CHAR 3 // character type

// specification for type
typedef struct {
    type_id type_id; // type id of the type
    bool is_struct;
    bool is_array;
    size_t array_size;
} type_spec_def;

type_spec_def *type_spec_def_new(type_id type_id, bool is_struct) {
    type_spec_def *res = new(type_spec_def);
    res->type_id = type_id;
    res->is_struct = is_struct;
    res->is_array = false;
    res->array_size = 0;
    return res;
}

void type_spec_def_free(type_spec_def *t) {
    free(t);
}

#endif