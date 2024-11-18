#include "../../src/frontend/parser/ast.h"

int main() {
    // Create leaf nodes (e.g., INT, ID)
    ASTNode *intTypeNode1 = createASTLeaf("Type", "int");
    ASTNode *intTypeNode2 = createASTLeaf("Type", "int");
    ASTNode *idNode1 = createASTLeaf("ID", "x");
    ASTNode *idNode2 = createASTLeaf("ID", "y");
    ASTNode *intLiteralNode = createASTLeaf("INT", "10");

    // Create nodes representing a declaration (int x;)
    ASTNode *varDeclNode1 = createASTNode("VarDecl", 2, intTypeNode1, idNode1);

    // Create nodes representing another declaration (int y = 10;)
    ASTNode *varDeclNode2 = createASTNode("VarDecl", 3, intTypeNode2, idNode2, intLiteralNode);

    // Create a compound statement node that contains the variable declarations
    ASTNode *compoundStmtNode = createASTNode("CompoundStmt", 2, varDeclNode1, varDeclNode2);

    // Print the entire AST using rprintAST
    printf("Printing the AST using rprintAST:\n");
    rprintAST(compoundStmtNode, 0);

    // Free the entire AST to avoid memory leaks
    freeAST(compoundStmtNode);

    return 0;
}
