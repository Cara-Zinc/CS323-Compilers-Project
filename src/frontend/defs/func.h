#ifndef __FUNC_H__
#define __FUNC_H__

#include "type.h"
#include "field.h"
#include "scope.h"
#include "util.h"

// definition of a function
typedef struct {
    char *name; // name of the function
    type_def *return_type; // return type specification of the function
    varlist *args; // arguments of the function
    scope *scope; // scope of the function
} func_def;

// create a new function definition
func_def *func_def_new(char *name, type_def *return_type) {
    func_def *res = new(func_def);
    res->name = name;
    res->return_type = return_type;
    res->args = vlist_new(4, &vlist_fvals);
    res->scope = scope_new();
    return res;
}

// free a function definition
void func_def_free(func_def *f) {
    free(f->name);
    type_def_free(f->return_type);
    while (vlist_pop_back(f->args)) {}
    vlist_free(f->args);
    scope_free(f->scope);
    free(f);
}

void func_def_add_arg(func_def *f, field_def *arg) {
    vlist_push_back(f->args, arg);
    scope_add_field(f->scope, arg);
}

field_def *func_def_get_arg(func_def *f, size_t index) {
    return vlist_get(f->args, index);
}

void func_def_add_variable(func_def *f, field_def *var) {
    scope_add_field(f->scope, var);
}

field_def *func_def_get_variable(func_def *f, char *name) {
    return scope_get_field(f->scope, name);
}

void func_def_add_func(func_def *f, func_def *func) {
    scope_add_func(f->scope, func);
}

func_def *func_def_get_func(func_def *f, char *name) {
    return scope_get_func(f->scope, name);
}

void func_def_add_struct(func_def *f, struct_def *struct_def) {
    scope_add_struct(f->scope, struct_def);
}

struct_def *func_def_get_struct(func_def *f, char *name) {
    return scope_get_struct(f->scope, name);
}

#define SNAME funcmap
#define PFX fmap
#define K char*
#define V func_def*
#include <cmc/hashmap.h>

typedef struct funcmap funcmap;

struct funcmap_fkey fmap_fkeys = {
    .cmp = cmc_str_cmp,
    .cpy = cmc_str_cpy,
    .str = cmc_str_str,
    .free = free,
    .hash = cmc_str_hash_java,
    .pri = cmc_str_cmp,
};

int func_def_cmp(func_def *f1, func_def *f2) {
    return cmc_str_cmp(f1->name, f2->name);
}

func_def *func_def_cpy(func_def *f) {
    func_def *res = new(func_def);
    res->name = str_copy(f->name);
    res->return_type = type_def_cpy(f->return_type);
    res->args = vlist_copy_of(f->args);
    res->scope = scope_cpy(f->scope);
}

bool func_def_str(FILE *file, func_def *f) {
    fprintf(file, "function %s", f->name);
    return true;
}

size_t func_def_hash(func_def *f) {
    return cmc_str_hash_java(f->name);
}

struct funcmap_fval fmap_fvals = {
    .cmp = func_def_cmp,
    .cpy = func_def_cpy,
    .str = func_def_str,
    .free = func_def_free,
    .hash = func_def_hash,
    .pri = func_def_cmp,
};

#endif