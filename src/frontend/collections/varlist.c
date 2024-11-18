#include "../defs/field.h"

#define SNAME varlist
#define PFX vlist
#define V field_def*
#include "list.h"

struct varlist_fval vlist_fvals = {
    .cmp = field_def_cmp,
    .cpy = field_def_cpy,
    .str = field_def_str,
    .free = field_def_free,
    .hash = field_def_hash,
    .pri = field_def_cmp,
};