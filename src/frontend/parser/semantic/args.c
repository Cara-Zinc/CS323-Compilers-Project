#include "args.h"
#include "exp.h"

varlist *args_semantic(program_manager *pm, ASTNode *node)
{
    varlist *vlist = vlist_new(alist_count(node->children), &vlist_fvals);
    // iterate through the EXP children of this Args node
    for (size_t i = 0; i < alist_count(node->children); i++)
    {
        // check if the argument contains an ID leaf node
        if (strcmp(alist_get(alist_get(node->children, i)->children, 0)->nodeType, "ID"))
        {
            fprintf(stderr, "Error at line %zu: invalid argument\n", alist_get(node->children, i)->line);
            field_def *field = field_def_new("invalid", type_def_new(TYPE_VOID, false));
            vlist_push_back(vlist, field);
            continue;
        }
        char *name = alist_get(alist_get(node->children, i)->children, 0)->text;

        // check if the EXP node is a valid expression
        if (!exp_semantic(pm, alist_get(node->children, i)))
        {
            fprintf(stderr, "Error at line %zu: invalid argument\n", alist_get(node->children, i)->line);
            field_def *field = field_def_new(name, type_def_new(TYPE_VOID, false));
            vlist_push_back(vlist, field);
            continue;
        }

        field_def *field = field_def_new(name, exp_semantic(pm, alist_get(node->children, i)));
        if (field)
        {
            vlist_push_back(vlist, field);
        }

    }
    return vlist;
}