#include "../defs/func.h"

#define SNAME funcmap
#define PFX fmap
#define K char*
#define V func_def*
#include "hashmap.h"

struct funcmap_fkey fmap_fkeys = {
    .cmp = cmc_str_cmp,
    .cpy = cmc_str_cpy,
    .str = cmc_str_str,
    .free = str_free,
    .hash = cmc_str_hash_java,
    .pri = cmc_str_cmp,
};

struct funcmap_fval fmap_fvals = {
    .cmp = func_def_cmp,
    .cpy = func_def_cpy,
    .str = func_def_str,
    .free = func_def_free,
    .hash = func_def_hash,
    .pri = func_def_cmp,
};