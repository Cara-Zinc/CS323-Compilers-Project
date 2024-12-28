#include "../defs/exp/exp.h"

#define SNAME explist
#define PFX explist
#define V exp*
#include "list.h"

struct explist_fval explist_fvals = {
    .cmp = NULL,
    .cpy = exp_cpy,
    .str = NULL,
    .free = exp_free,
    .hash = NULL,
    .pri = NULL,
};