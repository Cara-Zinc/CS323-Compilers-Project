#ifndef __FIELD_H__
#define __FIELD_H__

#include "type.h"
#include "../utils/util.h"

// field definition
typedef struct field_def {
    char *name; // field name
    type_def *type_spec; // field type specification
} field_def;

// create a new field definition
field_def *field_def_new(char *name, type_def *type_spec);

// free a field definition
void field_def_free(field_def *f);

int field_def_cmp(field_def *f1, field_def *f2);

field_def *field_def_cpy(field_def *f);

bool field_def_str(FILE *file, field_def *f);

size_t field_def_hash(field_def *f);

#include <cmc/utl/futils.h>

#define SNAME varmap
#define PFX vmap
#define K char*
#define V field_def*
#include <cmc/hashmap.h>

typedef struct varmap varmap;

struct varmap_fkey vmap_fkeys = {
    .cmp = cmc_str_cmp,
    .cpy = cmc_str_cpy,
    .str = cmc_str_str,
    .free = str_free,
    .hash = cmc_str_hash_java,
    .pri = cmc_str_cmp,
};

struct varmap_fval vmap_fvals = {
    .cmp = field_def_cmp,
    .cpy = field_def_cpy,
    .str = field_def_str,
    .free = field_def_free,
    .hash = field_def_hash,
    .pri = field_def_cmp,
};

#define SNAME varlist
#define PFX vlist
#define V field_def*
#include <cmc/list.h>

typedef struct varlist varlist;

struct varlist_fval vlist_fvals = {
    .cmp = field_def_cmp,
    .cpy = field_def_cpy,
    .str = field_def_str,
    .free = field_def_free,
    .hash = field_def_hash,
    .pri = field_def_cmp,
};

#endif