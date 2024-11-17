#ifndef __FUNC_H__
#define __FUNC_H__

#include <stdio.h>

#include "type.h"
#include "field.h"
#include "scope.h"
#include "../utils/util.h"

typedef struct scope scope;
typedef struct field_def field_def;
typedef struct struct_def struct_def;

// definition of a function
typedef struct func_def {
    char *name; // name of the function
    type_def *return_type; // return type specification of the function
    varlist *args; // arguments of the function
    scope *scope; // scope of the function
} func_def;

// create a new function definition
func_def *func_def_new(char *name, type_def *return_type);

// free a function definition
void func_def_free(func_def *f);

void func_def_add_arg(func_def *f, field_def *arg);

field_def *func_def_get_arg(func_def *f, size_t index);

size_t func_def_get_arg_count(func_def *f);

void func_def_add_variable(func_def *f, field_def *var);

field_def *func_def_get_variable(func_def *f, char *name);

void func_def_add_func(func_def *f, func_def *func);

func_def *func_def_get_func(func_def *f, char *name);

void func_def_add_struct(func_def *f, struct_def *struct_def);

struct_def *func_def_get_struct(func_def *f, char *name);

void func_def_add_subscope(func_def *f, scope *scope);

scope *func_def_get_subscope(func_def *f, size_t index);

size_t func_def_get_subscope_count(func_def *f);

int func_def_cmp(func_def *f1, func_def *f2);

func_def *func_def_cpy(func_def *f);

bool func_def_str(FILE *file, func_def *f);

size_t func_def_hash(func_def *f);

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
    .free = str_free,
    .hash = cmc_str_hash_java,
    .pri = cmc_str_cmp,
};

struct funcmap_fval fmap_fvals = {
    .cmp = func_def_cmp,
    .cpy = func_def_cpy,
    .str = func_def_str,
    .free = func_def_free,
    .hash = func_def_hash,
    .pri = func_def_cmp,
};

#endif