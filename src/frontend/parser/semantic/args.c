#include "args.h"
#include "exp.h"

varlist *args_semantic(program_manager *pm, ASTNode *node)
{
    if (!node->numChildren)
    {
        return NULL;
    }
    varlist *vlist = vlist_new(alist_count(node->children), &vlist_fvals);
    // iterate through the EXP children of this Args node
    for (size_t i = 0; i < alist_count(node->children); i++)
    {
        ASTNode *child = alist_get(node->children, i);
        if (!child->numChildren)
        {
            fprintf(stderr, "Error at line %zu: invalid argument with no children\n", child->line);
            field_def *field = field_def_new("invalid", type_def_new(TYPE_VOID, false));
            vlist_push_back(vlist, field);
            continue;
        }
        // check if the argument contains an ID leaf node
        if (strcmp(alist_get(child->children, 0)->nodeType, "ID") != 0)
        {
            fprintf(stderr, "Error at line %zu: invalid argument with no ID\n", child->line);
            field_def *field = field_def_new("invalid", type_def_new(TYPE_VOID, false));
            vlist_push_back(vlist, field);
            continue;
        }
        char *name = alist_get(child->children, 0)->text;

        // check if the EXP node is a valid expression
        if (!exp_semantic(pm, child))
        {
            fprintf(stderr, "Error at line %zu: invalid argument\n", child->line);
            field_def *field = field_def_new(name, type_def_new(TYPE_VOID, false));
            vlist_push_back(vlist, field);
            continue;
        }

        field_def *field = field_def_new(name, exp_semantic(pm, child));
        if (field)
        {
            vlist_push_back(vlist, field);
        }

    }
    return vlist;
}