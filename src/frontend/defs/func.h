#ifndef __FUNC_H__
#define __FUNC_H__

#include "list.h"
#include "type.h"
#include "util.h"
#include "field.h"

// definition of a function
typedef struct {
    char *name; // name of the function
    type_spec_def *return_type_spec; // return type specification of the function
    list *args; // arguments of the function
} func_def;

// create a new function definition
func_def *func_def_new(char *name, type_spec_def *return_type_spec) {
    func_def *res = new(func_def);
    res->name = name;
    res->return_type_spec = return_type_spec;
    res->args = list_new(field_def);
    return res;
}

// free a function definition
void func_def_free(func_def *f) {
    free(f->name);
    type_spec_def_free(f->return_type_spec);
    list_free(f->args);
    free(f);
}

#endif