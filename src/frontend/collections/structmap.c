#include "../defs/struct.h"

#define SNAME structmap
#define PFX smap
#define K char*
#define V struct_def*
#include "hashmap.h"

struct structmap_fkey smap_fkeys = {
    .cmp = cmc_str_cmp,
    .cpy = cmc_str_cpy,
    .str = cmc_str_str,
    .free = str_free,
    .hash = cmc_str_hash_java,
    .pri = cmc_str_cmp,
};

struct structmap_fval smap_fvals = {
    .cmp = struct_def_cmp,
    .cpy = struct_def_cpy,
    .str = struct_def_str,
    .free = struct_def_free,
    .hash = struct_def_hash,
    .pri = struct_def_cmp,
};