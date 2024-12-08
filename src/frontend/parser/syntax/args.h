#include "../mm/program_manager.h"
#include "ast.h"

ASTNode *args_handler(program_manager *pm, ASTNode *Exp, ASTNode *args) {
    if (args == NULL) {
        return createASTNode("Args", 1, Exp);
    }
    return createASTNode("Args", 2, Exp, args);
}