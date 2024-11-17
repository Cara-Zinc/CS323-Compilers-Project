#include "type.h"

// create a new type specification
type_def *type_def_new(type_id type_id, bool is_struct) {
    type_def *res = new(type_def);
    res->type_id = type_id;
    res->is_struct = is_struct;
    res->is_array = false;
    res->array_size = 0;
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