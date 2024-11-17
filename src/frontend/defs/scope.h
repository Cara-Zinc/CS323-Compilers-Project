#ifndef __SCOPE_H__
#define __SCOPE_H__

#include "struct.h"
#include "field.h"
#include "func.h"

typedef struct structmap structmap;
typedef struct varmap varmap;
typedef struct funcmap funcmap;
typedef struct scopelist scopelist;

// a scope, usually for struct def, function def, or just curly braces 
typedef struct scope {
    structmap *struct_defs;
    varmap *fields;
    funcmap *funcs;
    scopelist *subscopes; // subscopes refer to the first-level subscopes within this scope, excluding the subscopes within the subscopes
} scope;

scope *scope_new();

void scope_free(scope *s);

scope *scope_cpy(scope *s);

bool scope_str(FILE *file, scope *s);

void scope_add_struct(scope *s, struct_def *struct_def);

struct_def *scope_get_struct(scope *s, char *name);

void scope_add_field(scope *s, field_def *f);

field_def *scope_get_field(scope *s, char *name);

void scope_add_func(scope *s, func_def *f);

func_def *scope_get_func(scope *s, char *name);

void scope_add_subscope(scope *s, scope *scope);

scope *scope_get_subscope(scope *s, size_t index);

size_t scope_get_subscope_count(scope *s);

#define SNAME scopelist
#define PFX sclist
#define V scope*
#include <cmc/list.h>

struct scopelist_fval sclist_fvals = {
    .cmp = NULL,
    .cpy = scope_cpy,
    .str = scope_str,
    .free = scope_free,
    .hash = NULL,
    .pri = NULL,
};

#endif