#include "args.h"
#include "exp.h"

varlist *args_semantic(program_manager *pm, ASTNode *node)
{
    if (!node->numChildren)
    {
        return NULL;
    }
    varlist *vlist = vlist_new(alist_count(node->children), &vlist_fvals);

    ASTNode *child = alist_get(node->children, 0);
    if (!child->numChildren)
    {
        fprintf(stderr, "Error at line %zu: invalid argument with no children\n", child->line);
        field_def *field = field_def_new("invalid", type_def_new(TYPE_VOID, false));
        vlist_push_back(vlist, field);
        return vlist;
    }

    char *name = alist_get(child->children, 0)->text;

    // check if the EXP node is a valid expression
    if (!exp_semantic(pm, child))
    {
        fprintf(stderr, "Error at line %zu: invalid argument\n", child->line);
        field_def *field = field_def_new(name, type_def_new(TYPE_VOID, false));
        vlist_push_back(vlist, field);
        return vlist;
    }

    field_def *field = field_def_new(name, exp_semantic(pm, child));
    if (field)
    {
        vlist_push_back(vlist, field);
    }

    if (alist_count(node->children) == 2)
    {
        varlist *child_vlist = args_semantic(pm, alist_get(node->children, 1));
        for (size_t i = 0; i < vlist_count(child_vlist); i++)
        {
            field_def *field = vlist_get(child_vlist, i);
            vlist_push_back(vlist, field);
        }
    }

    return vlist;
}