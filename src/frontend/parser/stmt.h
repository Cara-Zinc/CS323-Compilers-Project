#include "../mm/program_manager.h"
#include "ast.h"

ASTNode *stmt_exp_handler(program_manager *pm, ASTNode *Exp) {
    return createASTNode("Stmt", 1, Exp);
}

ASTNode *stmt_comp_handler(program_manager *pm, ASTNode *CompSt) {
    return createASTNode("Stmt", 1, CompSt);
}

ASTNode *stmt_return_handler(program_manager *pm, ASTNode *Exp) {
    return createASTNode("RETURN", 2, Exp, createASTLeaf("SEMI", ";"));
}

ASTNode *stmt_if_handler(program_manager *pm, ASTNode *Exp, ASTNode *Stmt) {
    return createASTNode("IF", 6, createASTLeaf("LP", "("), Exp, createASTLeaf("RP", ")"), createASTLeaf("LC", "{"), Stmt, createASTLeaf("RC", "}"));
}

ASTNode *stmt_if_else_handler(program_manager *pm, ASTNode *Exp, ASTNode *Stmt1, ASTNode *Stmt2) {
    ASTNode *ifNode = createASTNode("IF", 6, createASTLeaf("LP", "("), Exp, createASTLeaf("RP", ")"), createASTLeaf("LC", "{"), Stmt1, createASTLeaf("RC", "}"));
    ASTNode *elseNode = createASTNode("ELSE", 3, createASTLeaf("LC", "{"), Stmt2, createASTLeaf("RC", "}"));
    return createASTNode("Stmt", 2, ifNode, elseNode);
}

ASTNode *stmt_while_handler(program_manager *pm, ASTNode *Exp, ASTNode *Stmt) {
    return createASTNode("WHILE", 6, createASTLeaf("LP", "("), Exp, createASTLeaf("RP", ")"), createASTLeaf("LC", "{"), Stmt, createASTLeaf("RC", "}"));
}

ASTNode *stmtlist_stmt_stmtlist_handler(program_manager *pm, ASTNode *Stmt, ASTNode *StmtList) {
    if (StmtList == NULL && Stmt == NULL)
    {
        return createASTLeaf("StmtList", "EMPTY");
    }
    else if (StmtList == NULL)
    {
        return createASTNode("StmtList", 1, Stmt);
    }
    else if (Stmt == NULL)
    {
        return StmtList;
    } else
    {
        return createASTNode("StmtList", 2, Stmt, StmtList);
    }
}