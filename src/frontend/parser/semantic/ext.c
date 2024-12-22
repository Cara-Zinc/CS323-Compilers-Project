#include "ext.h"
#include "declaration.h"
#include "spec.h"
#include "compst.h"
#include <stdlib.h>

void ext_def_semantic(program_manager *pm, ASTNode *ExtDef)
{
    if (ExtDef->numChildren == 2)
    {
        // Specifier ExtDecList SEMI
        type_def *type = specifier_semantic(pm, alist_get(ExtDef->children, 0));
        ext_dec_list_semantic(pm, alist_get(ExtDef->children, 1), type);
    }
    else if (ExtDef->numChildren == 1)
    {
        // Specifier SEMI
        specifier_semantic(pm, alist_get(ExtDef->children, 0));
    }
    else if (ExtDef->numChildren == 3)
    {
        // Specifier FunDec CompSt
        type_def *spec_type = specifier_semantic(pm, alist_get(ExtDef->children, 0));
        func_def *func = fundec_semantic(pm, alist_get(ExtDef->children, 1), spec_type);
        compst_semantic(pm, alist_get(ExtDef->children, 2), func);
        scope_wrapper *wrapper = program_manager_pop(pm);
        if (strcmp(wrapper->func->name, INVALID_FUNC_NAME) == 0)
        {
            scope_wrapper_free(wrapper);
            
        }
        else
        {
            scope_wrapper_free_without_data(wrapper);
        }

    }
    else
    {
        fprintf(stderr, "Error at line %zu: invalid syntax structure\n", ExtDef->line);
    }
}

void ext_def_list_semantic(program_manager *pm, ASTNode *ExtDefList)
{
    if (ExtDefList->numChildren == 0)
    {
        return;
    }
    else
    {
        ext_def_semantic(pm, alist_get(ExtDefList->children, 0));
        ext_def_list_semantic(pm, alist_get(ExtDefList->children, 1));
    }
}

void ext_dec_list_semantic(program_manager *pm, ASTNode *ExtDecList, type_def *type)
{
    if (ExtDecList->numChildren == 1)
    {
        vardec_semantic(pm, alist_get(ExtDecList->children, 0));
    }
    else
    {
        vardec_semantic(pm, alist_get(ExtDecList->children, 0));
        ext_dec_list_semantic(pm, alist_get(ExtDecList->children, 2), type);
    }
}

void program_semantic(program_manager *pm, ASTNode *Program)
{
    if (Program->numChildren == 0)
    {
        return;
    }
    else
    {
        ext_def_list_semantic(pm, alist_get(Program->children, 0));
    }
}
