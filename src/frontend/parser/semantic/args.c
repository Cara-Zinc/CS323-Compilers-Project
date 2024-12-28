#include "args.h"
#include "exp.h"

explist *args_semantic(program_manager *pm, ASTNode *node)
{
    explist *list = NULL;
    if (node->numChildren == 1)
    {
        list = explist_new(1, &explist_fvals);
    } else {
        list = args_semantic(pm, alist_get(node->children, 1));
    }
    exp *arg = exp_semantic(pm, alist_get(node->children, 0));
    explist_push_front(list, arg);
    return list;
}