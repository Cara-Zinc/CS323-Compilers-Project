#include "../mm/program_manager.h"
#include "ast.h"

ASTNode *def_specifier_declist_handler(program_manager *pm, ASTNode *Specifier, ASTNode *DecList, size_t line) {
    return createASTNode("Def", line, 2, Specifier, DecList);
}

ASTNode *deflist_def_deflist_handler(program_manager *pm, ASTNode *Def, ASTNode *DefList, size_t line) {
    if (DefList == NULL && Def == NULL)
    {
        return createASTLeaf("DefList", line, NULL);
    }
    else if (DefList == NULL)
    {
        return createASTNode("DefList", line, 1, Def);
    }
    else if (Def == NULL)
    {
        return DefList;
    } else
    {
        return createASTNode("DefList", line, 2, Def, DefList);
    }
}