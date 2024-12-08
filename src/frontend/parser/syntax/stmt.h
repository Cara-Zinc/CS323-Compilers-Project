#include "../mm/program_manager.h"
#include "ast.h"

ASTNode *stmt_exp_handler(program_manager *pm, ASTNode *Exp, size_t line) {
    return createASTNode("Stmt", line, 1, Exp);
}

ASTNode *stmt_comp_handler(program_manager *pm, ASTNode *CompSt, size_t line) {
    return createASTNode("Stmt", line, 1, CompSt);
}

ASTNode *stmt_return_handler(program_manager *pm, ASTNode *Exp, size_t line) {
    return createASTNode("RETURN", line, 1, Exp);
}

ASTNode *stmt_if_handler(program_manager *pm, ASTNode *Exp, ASTNode *Stmt, size_t line) {
    return createASTNode("IF", line, 2, Exp, Stmt);
}

ASTNode *stmt_if_else_handler(program_manager *pm, ASTNode *Exp, ASTNode *Stmt1, ASTNode *Stmt2, size_t line) {
    ASTNode *ifNode = createASTNode("IF", line, 2, Exp, Stmt1);
    ASTNode *elseNode = createASTNode("ELSE", line, 1, Stmt2);
    return createASTNode("Stmt", line, 2, ifNode, elseNode);
}

ASTNode *stmt_while_handler(program_manager *pm, ASTNode *Exp, ASTNode *Stmt, size_t line) {
    return createASTNode("WHILE", line, 2, Exp, Stmt);
}

ASTNode *stmtlist_stmt_stmtlist_handler(program_manager *pm, ASTNode *Stmt, ASTNode *StmtList, size_t line) {
    if (StmtList == NULL && Stmt == NULL)
    {
        return createASTLeaf("StmtList", line, NULL);
    }
    else if (StmtList == NULL)
    {
        return createASTNode("StmtList", line, 1, Stmt);
    }
    else if (Stmt == NULL)
    {
        return StmtList;
    } else
    {
        return createASTNode("StmtList", line, 2, Stmt, StmtList);
    }
}