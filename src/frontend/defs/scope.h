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
} scope;

scope *scope_new() {
    scope *s = new(scope);
    s->struct_defs = smap_new(8, 0.5, &smap_fkeys, &smap_fvals);
    s->fields = vmap_new(8, 0.5, &vmap_fkeys, &vmap_fvals);
    s->funcs = fmap_new(8, 0.5, &fmap_fkeys, &fmap_fvals);
    return s;
}

void scope_free(scope *s) {
    smap_free(s->struct_defs);
    vmap_free(s->fields);
    fmap_free(s->funcs);
    free(s);
}

scope *scope_cpy(scope *s) {
    scope *res = new(scope);
    res->struct_defs = smap_copy_of(s->struct_defs);
    res->fields = vmap_copy_of(s->fields);
    res->funcs = fmap_copy_of(s->funcs);
    return res;
}

bool scope_str(FILE *file, scope *s) {
    fprintf(file, "scope with %d struct defs, %d fields, %d funcs", s->struct_defs->count, s->fields->count, s->funcs->count);
    return true;
}

void scope_add_struct(scope *s, struct_def* struct_def) {
    smap_insert(s->struct_defs, struct_def->name, struct_def);
}

void scope_add_field(scope *s, field_def *f) {
    vmap_insert(s->fields, f->name, f);
}

void scope_add_func(scope *s, func_def* f) {
    fmap_insert(s->funcs, f->name, f);
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