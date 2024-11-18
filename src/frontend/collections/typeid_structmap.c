#include "../defs/struct.h"

#define SNAME typeid_structmap
#define PFX tsmap
#define K type_id
#define V struct_def*
#include "hashmap.h"

struct typeid_structmap_fkey tsmap_fkeys = {
    .cmp = cmc_size_cmp,
    .cpy = NULL,
    .str = cmc_size_str,
    .free = NULL,
    .hash = cmc_size_hash,
    .pri = cmc_size_cmp,
};

struct typeid_structmap_fval tsmap_fvals = {
    .cmp = struct_def_cmp,
    .cpy = struct_def_cpy,
    .str = struct_def_str,
    .free = struct_def_free,
    .hash = struct_def_hash,
    .pri = struct_def_cmp,
};