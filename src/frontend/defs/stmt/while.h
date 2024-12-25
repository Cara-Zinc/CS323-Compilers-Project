#ifndef __STMT_WHILE_H__
#define __STMT_WHILE_H__

#include "stmt.h"
#include "../exp/exp.h"

typedef struct stmt_while {
    exp *predicate;
    struct stmt *stmt;
} stmt_while;

#endif