#include "../mm/program_manager.h"
#include "ast.h"

ASTNode *compst_deflist_stmtlist_handler(program_manager *pm, ASTNode *DefList, ASTNode *StmtList, size_t line) {
    // return createASTNode("CompSt", 2, DefList, StmtList);
    if (DefList == NULL && StmtList == NULL) {
        return createASTLeaf("CompSt", line, "Empty");
    } else if (DefList == NULL) {
        return createASTNode("CompSt", line, 1, StmtList);
    } else if (StmtList == NULL) {
        return createASTNode("CompSt", line, 1, DefList);
    } else {
        return createASTNode("CompSt", line, 2, DefList, StmtList);
    }

}