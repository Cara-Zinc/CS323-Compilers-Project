#include "type.h"
#include "../mm/program_manager.h"
#include <cmc/utl/futils.h>

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
    if (t == NULL) {
        return;
    }

    if (t->array_type != NULL) {
        type_def_free(t->array_type);
    }
    free(t);
}

/**
 * compare two type specifications
 * @return 0 if the two type specifications are equal, otherwise a non-zero value
 */
int type_def_cmp(type_def *t1, type_def *t2) {
    int struct_cmp = cmc_u8_cmp(t1->is_struct, t2->is_struct);
    if (struct_cmp != 0) {
        return struct_cmp;
    }

    int array_cmp = cmc_u8_cmp(t1->is_array, t2->is_array);
    if (array_cmp != 0) {
        return array_cmp;
    }

    if (t1->is_array) {
        int size_cmp = cmc_size_cmp(t1->array_size, t2->array_size);
        if (size_cmp != 0) {
            return size_cmp;
        }

        return type_def_cmp(t1->array_type, t2->array_type);
    }

    return cmc_size_cmp(t1->type_id, t2->type_id);
}

// get the name of a type
char *type_def_name(program_manager *pm, type_def *t)
{
    if (t->is_array)
    {
        char *array_type = type_def_name(pm, t->array_type);
        char *res = malloc(strlen(array_type) + 10);
        if (array_type[0] == '[')
            sprintf(res, "[%zu]%s", t->array_size, array_type + 1);
        else
            sprintf(res, "%s[%zu]", array_type, t->array_size);
        free(array_type);
        return res;
    }

    if (t->is_struct)
    {
        struct_def *s = program_manager_get_struct_by_id(pm, t->type_id);
        return s->name;
    }

    switch (t->type_id)
    {
    case TYPE_VOID:
        return "void";
    case TYPE_INT:
        return "int";
    case TYPE_FLOAT:
        return "float";
    case TYPE_CHAR:
        return "char";
    default:
        return "unknown";
    }

}