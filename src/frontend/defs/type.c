#include "type.h"

// create a new type specification
type_def *type_def_new(type_id type_id, bool is_struct) {
    type_def *res = new(type_def);
    res->type_id = type_id;
    res->is_struct = is_struct;
    res->is_array = false;
    res->array_size = 0;
    res->array_type = NULL;
    return res;
}

type_def *type_def_cpy(type_def *t) {
    return mem_copy(type_def, t);
}

// create a new primitive type specification
type_def *type_def_new_primitive(type_id type_id) {
    return type_def_new(type_id, false);
}

// create a new struct type specification
type_def *type_def_new_struct(type_id type_id) {
    return type_def_new(type_id, true);
}

// create a new array type specification
type_def *type_def_new_array(type_def *array_type, size_t array_size) {
    type_def *res = type_def_new(TYPE_VOID, false);
    res->is_array = true;
    res->array_size = array_size;
    res->array_type = array_type;
    return res;
}

// free a type specification
void type_def_free(type_def *t) {
    if (t->array_type != NULL) {
        type_def_free(t->array_type);
    }
    free(t);
}