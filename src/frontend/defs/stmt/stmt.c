#include "stmt.h"
#include "../../utils/util.h"

stmt *stmt_new_exp(exp *exp) {
    stmt *s = new(stmt);
    s->stmt_type = STMT_EXP;
    s->exp.exp = exp;
    return s;
}

stmt *stmt_new_comp() {
    stmt *s = new(stmt);
    s->stmt_type = STMT_COMP;
    s->comp.stmts = stmtlist_new(1, &stmtlist_fvals);
    return s;
}

stmt *stmt_new_return(exp *exp) {
    stmt *s = new(stmt);
    s->stmt_type = STMT_RETURN;
    s->return_.exp = exp;
    return s;
}

stmt *stmt_new_if(exp *predicate, stmt *if_stmt, stmt *else_stmt) {
    stmt *s = new(stmt);
    s->stmt_type = STMT_IF;
    s->if_.predicate = predicate;
    s->if_.if_stmt = if_stmt;
    s->if_.else_stmt = else_stmt;
    return s;
}

stmt *stmt_new_while(exp *predicate, stmt *stmt) {
    struct stmt *s = new(struct stmt);
    s->stmt_type = STMT_WHILE;
    s->while_.predicate = predicate;
    s->while_.stmt = stmt;
    return s;
}

void stmt_free(stmt *stmt) {
    switch (stmt->stmt_type) {
        case STMT_EXP:
            exp_free(stmt->exp.exp);
            break;
        case STMT_COMP:
            stmtlist_free(stmt->comp.stmts);
            break;
        case STMT_RETURN:
            exp_free(stmt->return_.exp);
            break;
        case STMT_IF:
            exp_free(stmt->if_.predicate);
            stmt_free(stmt->if_.if_stmt);
            if (stmt->if_.else_stmt) {
                stmt_free(stmt->if_.else_stmt);
            }
            break;
        case STMT_WHILE:
            exp_free(stmt->while_.predicate);
            stmt_free(stmt->while_.stmt);
            break;
    }
    free(stmt);
}