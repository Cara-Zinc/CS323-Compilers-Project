#include "stmt.h"
#include "../../utils/util.h"

stmt *stmt_new_invalid()
{
    stmt *s = new (stmt);
    s->stmt_type = STMT_INVALID;
    return s;
}

stmt *stmt_new_exp(exp *exp)
{
    stmt *s = new (stmt);
    s->stmt_type = STMT_EXP;
    s->exp.exp = exp;
    return s;
}

stmt *stmt_new_comp(struct stmtlist *stmts)
{
    stmt *s = new (stmt);
    s->stmt_type = STMT_COMP;
    s->comp.stmts = stmts;
    return s;
}

stmt *stmt_new_return(exp *exp)
{
    stmt *s = new (stmt);
    s->stmt_type = STMT_RETURN;
    s->return_.exp = exp;
    return s;
}

stmt *stmt_new_if(exp *predicate, stmt *if_stmt, stmt *else_stmt)
{
    stmt *s = new (stmt);
    s->stmt_type = STMT_IF;
    s->if_.predicate = predicate;
    s->if_.if_stmt = if_stmt;
    s->if_.else_stmt = else_stmt;
    return s;
}

stmt *stmt_new_while(exp *predicate, stmt *stmt)
{
    struct stmt *s = new (struct stmt);
    s->stmt_type = STMT_WHILE;
    s->while_.predicate = predicate;
    s->while_.stmt = stmt;
    return s;
}

void stmt_free(stmt *stmt)
{
    switch (stmt->stmt_type)
    {
    case STMT_INVALID:
        break;
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
        if (stmt->if_.else_stmt)
        {
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

stmt *stmt_cpy(stmt *s)
{
    stmt *cpy = new (stmt);
    cpy->stmt_type = s->stmt_type;
    switch (s->stmt_type)
    {
    case STMT_INVALID:
        break;
    case STMT_EXP:
        cpy->exp.exp = exp_cpy(s->exp.exp);
        break;
    case STMT_COMP:
        cpy->comp.stmts = stmtlist_copy_of(s->comp.stmts);
        break;
    case STMT_RETURN:
        cpy->return_.exp = exp_cpy(s->return_.exp);
        break;
    case STMT_IF:
        cpy->if_.predicate = exp_cpy(s->if_.predicate);
        cpy->if_.if_stmt = stmt_cpy(s->if_.if_stmt);
        if (s->if_.else_stmt)
        {
            cpy->if_.else_stmt = stmt_cpy(s->if_.else_stmt);
        }
        break;
    case STMT_WHILE:
        cpy->while_.predicate = exp_cpy(s->while_.predicate);
        cpy->while_.stmt = stmt_cpy(s->while_.stmt);
        break;
    }
    return cpy;
}
