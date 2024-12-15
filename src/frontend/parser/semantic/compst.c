#include "compst.h"
#include "stmt.h"
#include "declaration.h"

void compst_semantic(program_manager *pm, ASTNode *node, func_def *func) {
    program_manager_create_subscope(pm);
    if (node->numChildren == 2)
    {
        deflist_semantic(pm, alist_get(node->children, 0));
        stmtlist_semantic(pm, alist_get(node->children, 1), func);
    }
    else
    {
        stmtlist_semantic(pm, alist_get(node->children, 0), func);
    }
    program_manager_pop(pm);
}