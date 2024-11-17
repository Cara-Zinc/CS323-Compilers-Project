#include "../mm/program_manager.h"
#include "ast.h"

ASTNode *stmt_exp_handler(program_manager *pm, ASTNode *Exp) {
    return createASTNode("Stmt", 1, Exp);
}

ASTNode *stmt_comp_handler(program_manager *pm, ASTNode *CompSt) {
    return createASTNode("Stmt", 1, CompSt);
}

ASTNode *stmt_if_handler(program_manager *pm, ASTNode *Exp, ASTNode *Stmt) {
    return createASTNode("Stmt", 2, Exp, Stmt);
}

ASTNode *stmt_if_else_handler(program_manager *pm, ASTNode *Exp, ASTNode *Stmt1, ASTNode *Stmt2) {
    return createASTNode("Stmt", 3, Exp, Stmt1, Stmt2);
}

ASTNode *stmt_while_handler(program_manager *pm, ASTNode *Exp, ASTNode *Stmt) {
    return createASTNode("Stmt", 2, Exp, Stmt);
}