#include "ast.h"
#include "../utils/copy.h"

ASTNode *createASTNode(char *type, int numChildren, ...) {
    ASTNode *node = new(ASTNode);
    node->nodeType = str_copy(type);
    node->text = NULL;
    node->numChildren = numChildren;
    node->children = alist_new(2, &alist_fvals);
    va_list args;
    va_start(args, numChildren);
    for (int i = 0; i < numChildren; i++) {
        alist_push_back(node->children, va_arg(args, ASTNode *));
    }
    va_end(args);
    return node;
}

ASTNode *createASTLeaf(char *type, char *text) {
    ASTNode *node = new(ASTNode);
    node->nodeType = str_copy(type);
    node->text = text ? str_copy(text) : NULL;
    node->numChildren = 0;
    node->children = NULL;
    return node;
}

void printAST(ASTNode *node, int level) {
    if (!node) return;
    for (int i = 0; i < level; i++) printf("  ");
    printf("%s", node->nodeType);
    if (node->text) printf(" (%s)", node->text);
    printf("\n");
    alist_print(node->children, stdout, " ", " ", " ");
}

void rprintAST(ASTNode *node, int level) {
    if (!node) return;

    // Print the current node with appropriate indentation
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("%s", node->nodeType);
    if(strcmp(node->nodeType, "ID") == 0 || strcmp(node->nodeType, "INT") == 0 || strcmp(node->nodeType, "FLOAT") == 0 || strcmp(node->nodeType, "CHAR") == 0) {
        printf(":");
    }
    
    if (node->text) {
        printf(" (%s)", node->text);
    }
    printf("\n");

    // Recursively print each child node with increased indentation level
    if (node->children) {
        for (size_t i = 0; i < alist_count(node->children); i++) {
            ASTNode *child = alist_get(node->children, i);
            rprintAST(child, level + 1);
        }
    }
}

void freeAST(ASTNode *node) {
    if (!node) return;
    if (node->nodeType)
    {
        printf("freeing %s\n", node->nodeType);
        free(node->nodeType);
        node->nodeType = NULL;
    } 
    if (node->text){
        free(node->text);
        node->text = NULL;
    } 
    if(node->children) {
       alist_free(node->children);
    }
    free(node);
}