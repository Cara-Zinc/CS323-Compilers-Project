#ifndef __FIELD_H__
#define __FIELD_H__

#include "type.h"
#include "util.h"

// field definition
typedef struct {
    char *name; // field name
    type_def *type_spec; // field type specification
} field_def;

// create a new field definition
field_def *field_def_new(char *name, type_def *type_spec) {
    field_def *res = new(field_def);
    res->name = name;
    res->type_spec = type_spec;
    return res;
}

// free a field definition
void field_def_free(field_def *f) {
    free(f->name);
    type_def_free(f->type_spec);
    free(f);
}

#include <cmc/utl/futils.h>

#define SNAME varmap
#define PFX vmap
#define K char*
#define V field_def*
#include <cmc/hashmap.h>

struct varmap_fkey vmap_fkeys = {
    .cmp = cmc_str_cmp,
    .cpy = cmc_str_cpy,
    .str = cmc_str_str,
    .free = free,
    .hash = cmc_str_hash_java,
    .pri = cmc_str_cmp,
};

int field_def_cmp(field_def *f1, field_def *f2) {
    return cmc_chr_cmp(f1->name, f2->name);
}

field_def *field_def_cpy(field_def *f) {
    char *name = str_copy(f->name);
    return field_def_new(name, type_def_cpy(f->type_spec));
}

bool field_def_str(FILE *file, field_def *f) {
    fprintf(file, "variable %s", f->name);
    return true;
}

size_t field_def_hash(field_def *f) {
    return cmc_str_hash_java(f->name);
}

struct varmap_fval vmap_fvals = {
    .cmp = field_def_cmp,
    .cpy = field_def_cpy,
    .str = field_def_str,
    .free = field_def_free,
    .hash = field_def_hash,
    .pri = field_def_cmp,
};

#endif