#include "../defs/scope.h"

#define SNAME scopelist
#define PFX sclist
#define V scope*
#include "list.h"

struct scopelist_fval sclist_fvals = {
    .cmp = NULL,
    .cpy = scope_cpy,
    .str = scope_str,
    .free = scope_free,
    .hash = NULL,
    .pri = NULL,
};