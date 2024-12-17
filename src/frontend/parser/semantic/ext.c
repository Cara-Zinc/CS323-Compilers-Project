#include "ext.h"
#include "declaration.h"
#include "spec.h"
#include <stdlib.h>

void ext_def_semantic(program_manager *pm, ASTNode *ExtDef)
{
    if(ExtDef->numChildren==2)
    {
        // Specifier ExtDecList SEMI
        type_def *type = specifier_semantic(pm, alist_get(ExtDef->children, 0));
        ext_dec_list_semantic(pm, alist_get(ExtDef->children, 1), type);
    }
    else if (ExtDef->numChildren==1)
    {
        // Specifier SEMI
        specifier_semantic(pm, alist_get(ExtDef->children, 0));
    }
    else if(ExtDef->numChildren==3)
    {
        // Specifier FunDec CompSt

        func_def *func = fundef_semantic(pm, alist_get(ExtDef->children, 1));
        type_def *spec_type = specifier_semantic(pm, alist_get(ExtDef->children, 0));
        type_def *func_type = func->return_type;
        if(type_def_cmp(spec_type, func_type) != 0)
        {
            fprintf(stderr, "Error at line %zu: function %s has return type %s, but it is declared as %s\n", alist_get(ExtDef->children, 1)->line, alist_get(ExtDef->children, 1)->text, type_def_name(pm, spec_type), type_def_name(pm, func_type));
        }
        else
        {
            program_manager_create_func(pm, func->name, spec_type);
        }

    }
    else
    {
        fprintf(stderr, "Error at line %zu: invalid syntax structure\n", ExtDef->line);
    }


}

void ext_def_list_semantic(program_manager *pm, ASTNode *ExtDefList)
{
    if(ExtDefList->numChildren == 0)
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
    if(ExtDecList->numChildren == 1)
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
    if(Program->numChildren == 0)
    {
        return;
    }
    else
    {
        ext_def_list_semantic(pm, alist_get(Program->children, 0));
    }
}
