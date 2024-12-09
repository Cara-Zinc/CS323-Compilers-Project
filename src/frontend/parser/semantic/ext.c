#include "ext.h"
#include "spec.h"
#include <stdlib.h>

void ext_def_semantic(program_manager *pm, ASTNode *ExtDef)
{
    type_def *type = specifier_semantic(pm, alist_get(ExtDef->children, 0));
    ext_dec_list_semantic(pm, alist_get(ExtDef->children, 1), type);
    
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
        dec_semantic(pm, alist_get(ExtDecList->children, 0), type);
    }
    else 
    {
        dec_semantic(pm, alist_get(ExtDecList->children, 0), type);
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
