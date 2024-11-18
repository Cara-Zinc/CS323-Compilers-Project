#include "../mm/program_manager.h"
#include "ast.h"

ASTNode *compst_deflist_stmtlist_handler(program_manager *pm, ASTNode *DefList, ASTNode *StmtList) {
    // return createASTNode("CompSt", 2, DefList, StmtList);
    if (DefList == NULL && StmtList == NULL) {
        return createASTNode("CompSt", 2, createASTLeaf("LC", "{"), createASTLeaf("RC", "}"));
    } else if (DefList == NULL) {
        return createASTNode("CompSt", 3, createASTLeaf("LC", "{"), StmtList, createASTLeaf("RC", "}"));
    } else if (StmtList == NULL) {
        return createASTNode("CompSt", 3, createASTLeaf("LC", "{"), DefList, createASTLeaf("RC", "}"));
    } else {
        return createASTNode("CompSt", 4, createASTLeaf("LC", "{"), DefList, StmtList, createASTLeaf("RC", "}"));
    }

}