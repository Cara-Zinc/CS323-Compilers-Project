#include "../mm/program_manager.h"
#include "ast.h"

ASTNode *stmt_exp_handler(program_manager *pm, ASTNode *Exp) {
    return createASTNode("Stmt", 1, Exp);
}

ASTNode *stmt_comp_handler(program_manager *pm, ASTNode *CompSt) {
    return createASTNode("Stmt", 1, CompSt);
}

ASTNode *stmt_return_handler(program_manager *pm, ASTNode *Exp) {
    return createASTNode("RETURN", 1, Exp);
}

ASTNode *stmt_if_handler(program_manager *pm, ASTNode *Exp, ASTNode *Stmt) {
    return createASTNode("IF", 2, Exp, Stmt);
}

ASTNode *stmt_if_else_handler(program_manager *pm, ASTNode *Exp, ASTNode *Stmt1, ASTNode *Stmt2) {
    return createASTNode("ELSE", 1, Stmt2), createASTNode("IF", 2, Exp, Stmt1);
}

ASTNode *stmt_while_handler(program_manager *pm, ASTNode *Exp, ASTNode *Stmt) {
    return createASTNode("WHILE", 2, Exp, Stmt);
}

ASTNode *stmtlist_stmt_handler(program_manager *pm, ASTNode *Stmt) {
    return createASTNode("StmtList", 1, Stmt);
}

ASTNode *stmtlist_stmt_stmtlist_handler(program_manager *pm, ASTNode *Stmt, ASTNode *StmtList) {
    return createASTNode("StmtList", 2, Stmt, StmtList);
}