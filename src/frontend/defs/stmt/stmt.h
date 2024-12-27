#ifndef __STMT_H__
#define __STMT_H__

#include "exp.h"
#include "comp.h"
#include "return.h"
#include "if.h"
#include "while.h"

typedef enum stmt_type {
    STMT_INVALID = 0,
    STMT_EXP,
    STMT_COMP,
    STMT_RETURN,
    STMT_IF,
    STMT_WHILE,
} stmt_type;

typedef struct stmt {
    stmt_type stmt_type;
    union {
        stmt_exp exp;
        stmt_comp comp;
        stmt_return return_;
        stmt_if if_;
        stmt_while while_;
    };
} stmt;

stmt *stmt_new_invalid();

stmt *stmt_new_exp(exp *exp);

stmt *stmt_new_comp();

stmt *stmt_new_return(exp *exp);

stmt *stmt_new_if(exp *predicate, stmt *if_stmt, stmt *else_stmt);

stmt *stmt_new_while(exp *predicate, stmt *stmt);

void stmt_free(stmt *stmt);

stmt *stmt_cpy(stmt *s);

#define SNAME stmtlist
#define PFX stmtlist
#define V stmt*
#include <../collections/list_h.h>

extern struct stmtlist_fval stmtlist_fvals;

#endif