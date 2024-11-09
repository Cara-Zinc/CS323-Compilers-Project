#ifndef __TYPE_H__
#define __TYPE_H__

#include <stddef.h>
#include <stdbool.h>
#include "util.h"

typedef size_t type_id;

// types of values are designed as follows when type_def::is_struct is false:
#define TYPE_VOID 0 // void type
#define TYPE_INT 1 // integer type
#define TYPE_FLOAT 2 // floating point type
#define TYPE_CHAR 3 // character type

// specification for type
typedef struct {
    type_id type_id; // type id of the type
    bool is_struct; // whether the type is a struct
    bool is_array; // whether the type is an array
    size_t array_size; // size of the array
} type_def;

// create a new type specification
type_def *type_def_new(type_id type_id, bool is_struct) {
    type_def *res = new(type_def);
    res->type_id = type_id;
    res->is_struct = is_struct;
    res->is_array = false;
    res->array_size = 0;
    return res;
}

// create a new primitive type specification
type_def *type_def_new_primitive(type_id type_id) {
    return type_def_new(type_id, false);
}

// create a new struct type specification
type_def *type_def_new_struct(type_id type_id) {
    return type_def_new(type_id, true);
}

// create a new primitive array type specification
type_def *type_def_new_primitive_array(type_id type_id, size_t array_size) {
    type_def *res = type_def_new_primitive(type_id);
    res->is_array = true;
    res->array_size = array_size;
    return res;
}

// create a new struct array type specification
type_def *type_def_new_struct_array(type_id type_id, size_t array_size) {
    type_def *res = type_def_new_struct(type_id);
    res->is_array = true;
    res->array_size = array_size;
    return res;
}

// free a type specification
void type_def_free(type_def *t) {
    free(t);
}

#endif