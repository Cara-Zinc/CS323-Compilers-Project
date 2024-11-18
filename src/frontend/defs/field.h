#ifndef __FIELD_H__
#define __FIELD_H__

#include <stdio.h>

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
#include <cmc/cor/core.h>
#include <cmc/cor/hashtable.h>
#include <cmc/hashmap/struct.h>
#include <cmc/hashmap/header.h>

typedef struct varmap varmap;

extern struct varmap_fkey vmap_fkeys;

extern struct varmap_fval vmap_fvals;

#include <cmc/cor/undef.h>

#define SNAME varlist
#define PFX vlist
#define V field_def*
#include <cmc/cor/core.h>
#include <cmc/cor/hashtable.h>
#include <cmc/list/struct.h>
#include <cmc/list/header.h>

typedef struct varlist varlist;

extern struct varlist_fval vlist_fvals;

#include <cmc/cor/undef.h>

#endif