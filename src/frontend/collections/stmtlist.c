#include "../defs/stmt/stmt.h"

#define SNAME stmtlist
#define PFX stmtlist
#define V stmt*
#include "list.h"

struct stmtlist_fval stmtlist_fvals = {
    .cmp = NULL,
    .cpy = NULL,
    .str = NULL,
    .free = stmt_free,
    .hash = NULL,
    .pri = NULL,
};