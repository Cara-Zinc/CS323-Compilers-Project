#ifndef __TYPE_H__
#define __TYPE_H__

#include <stddef.h>
#include <stdbool.h>
#include "../utils/util.h"
#include "../utils/copy.h"

typedef size_t type_id;

// types of values are designed as follows when type_def::is_struct is false:
#define TYPE_VOID 0 // void type
#define TYPE_INT 1 // integer type
#define TYPE_FLOAT 2 // floating point type
#define TYPE_CHAR 3 // character type

// specification for type
typedef struct type_def {
    type_id type_id; // type id of the type
    bool is_struct; // whether the type is a struct
    bool is_array; // whether the type is an array
    size_t array_size; // size of the array
    struct type_def *array_type; // type of the array
} type_def;

// create a new type specification
type_def *type_def_new(type_id type_id, bool is_struct);

type_def *type_def_cpy(type_def *t);

// create a new primitive type specification
type_def *type_def_new_primitive(type_id type_id);

// create a new struct type specification
type_def *type_def_new_struct(type_id type_id);

// create a new array type specification
type_def *type_def_new_array(type_def *array_type, size_t array_size);

// free a type specification
void type_def_free(type_def *t);

// compare two type specifications
int type_def_cmp(type_def *t1, type_def *t2);

#endif