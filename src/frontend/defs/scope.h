#ifndef __SCOPE_H__
#define __SCOPE_H__

#include "struct.h"
#include "field.h"
#include "func.h"

// a scope, usually for struct def, function def, or just curly braces 
typedef struct {
    structmap *struct_defs;
    varmap *fields;
    funcmap *funcs;
    scopelist *subscopes; // subscopes refer to the first-level subscopes within this scope, excluding the subscopes within the subscopes
} scope;

scope *scope_new() {
    scope *s = new(scope);
    s->struct_defs = smap_new(8, 0.5, &smap_fkeys, &smap_fvals);
    s->fields = vmap_new(8, 0.5, &vmap_fkeys, &vmap_fvals);
    s->funcs = fmap_new(8, 0.5, &fmap_fkeys, &fmap_fvals);
    s->subscopes = sclist_new(4, &sclist_fvals);
    return s;
}

void scope_free(scope *s) {
    smap_free(s->struct_defs);
    vmap_free(s->fields);
    fmap_free(s->funcs);
    sclist_free(s->subscopes);
    free(s);
}

scope *scope_cpy(scope *s) {
    scope *res = new(scope);
    res->struct_defs = smap_copy_of(s->struct_defs);
    res->fields = vmap_copy_of(s->fields);
    res->funcs = fmap_copy_of(s->funcs);
    res->subscopes = sclist_copy_of(s->subscopes);
    return res;
}

bool scope_str(FILE *file, scope *s) {
    fprintf(file, "scope with %d struct defs, %d fields, %d funcs, %d subscopes", s->struct_defs->count, s->fields->count, s->funcs->count, s->subscopes->count);
    return true;
}

void scope_add_struct(scope *s, struct_def *struct_def) {
    smap_insert(s->struct_defs, str_copy(struct_def->name), struct_def);
}

struct_def *scope_get_struct(scope *s, char *name) {
    return smap_get(s->struct_defs, name);
}

void scope_add_field(scope *s, field_def *f) {
    vmap_insert(s->fields, str_copy(f->name), f);
}

field_def *scope_get_field(scope *s, char *name) {
    return vmap_get(s->fields, name);
}

void scope_add_func(scope *s, func_def *f) {
    fmap_insert(s->funcs, str_copy(f->name), f);
}

func_def *scope_get_func(scope *s, char *name) {
    return fmap_get(s->funcs, name);
}

void scope_add_subscope(scope *s, scope *scope) {
    sclist_push_back(s->subscopes, scope);
}

scope *scope_get_subscope(scope *s, size_t index) {
    return sclist_get(s->subscopes, index);
}

size_t scope_get_subscope_count(scope *s) {
    return s->subscopes->count;
}

#define SNAME scopelist
#define PFX sclist
#define V scope*
#include <cmc/list.h>

typedef struct scopelist scopelist;

struct scopelist_fval sclist_fvals = {
    .cmp = NULL,
    .cpy = scope_cpy,
    .str = scope_str,
    .free = scope_free,
    .hash = NULL,
    .pri = NULL,
};

#endif