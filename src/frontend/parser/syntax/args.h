#include "../mm/program_manager.h"
#include "ast.h"

ASTNode *args_handler(program_manager *pm, ASTNode *Exp, ASTNode *args, size_t line)
{
    if (args == NULL) {
        return createASTNode("Args", line, 1, Exp);
    }
    return createASTNode("Args", line, 2, Exp, args);
}