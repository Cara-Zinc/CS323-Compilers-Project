#include "../defs/field.h"

#define SNAME varmap
#define PFX vmap
#define K char*
#define V field_def*
#include "hashmap.h"

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
