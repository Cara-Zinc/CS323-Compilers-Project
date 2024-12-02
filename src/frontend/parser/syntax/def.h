#include "../mm/program_manager.h"
#include "ast.h"

ASTNode *def_specifier_declist_handler(program_manager *pm, ASTNode *Specifier, ASTNode *DecList) {
    return createASTNode("Def", 2, Specifier, DecList);
}

ASTNode *deflist_def_deflist_handler(program_manager *pm, ASTNode *Def, ASTNode *DefList) {
    if (DefList == NULL && Def == NULL)
    {
        return createASTLeaf("DefList", "EMPTY");
    }
    else if (DefList == NULL)
    {
        return createASTNode("DefList", 1, Def);
    }
    else if (Def == NULL)
    {
        return DefList;
    } else
    {
        return createASTNode("DefList", 2, Def, DefList);
    }
}