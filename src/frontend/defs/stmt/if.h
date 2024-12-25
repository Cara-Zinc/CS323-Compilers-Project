#ifndef __STMT_IF_H__
#define __STMT_IF_H__

#include "stmt.h"
#include "../exp/exp.h"

typedef struct stmt_if {
    exp *predicate;
    struct stmt *if_stmt;
    struct stmt *else_stmt;
} stmt_if;

#endif