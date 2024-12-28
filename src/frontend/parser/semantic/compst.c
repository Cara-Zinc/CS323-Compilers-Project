#include "compst.h"
#include "stmt.h"
#include "declaration.h"

// plain semantic analysis of function entry
void func_entry_semantic(program_manager *pm, ASTNode *node, func_def *func) {
    stmtlist *stmts = NULL;
    if (node->numChildren == 2)
    {
        deflist_semantic(pm, alist_get(node->children, 0));
        stmts = stmtlist_semantic(pm, alist_get(node->children, 1), func);
    }
    else
    {
        stmts = stmtlist_semantic(pm, alist_get(node->children, 0), func);
    }
    stmtlist_free(func->stmts);
    func->stmts = stmts;
}

// semantic analysis of a statement list
stmtlist *stmtlist_semantic(program_manager *pm, ASTNode *node, func_def *func)
{
    if (node->numChildren == 0)
    {
        return stmtlist_new(4, &stmtlist_fvals);
    }
    stmt *stmt = stmt_semantic(pm, alist_get(node->children, 0), func);
    stmtlist *list = stmtlist_semantic(pm, alist_get(node->children, 1), func);
    stmtlist_push_front(list, stmt);
    return list;
}