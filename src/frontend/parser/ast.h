#ifndef __AST_H__
#define __AST_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>


// AST Node structure
typedef struct ASTNode {
    char *nodeType;          // Type of the node (e.g., "Program", "ExtDefList")
    char *text;              // Token text, if any (for leaves)
    int numChildren;         // Number of child nodes
    struct ASTNode **children; // Array of pointers to child nodes
} ASTNode;

// Function declarations
ASTNode *createASTNode(char *type, int numChildren, ...);
ASTNode *createASTLeaf(char *type, char *text);
void printAST(ASTNode *node, int level);
void freeAST(ASTNode *node);

// Helper function to create an AST node with variable number of children
ASTNode *createASTNode(char *type, int numChildren, ...) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->nodeType = strdup(type);
    node->text = NULL;
    node->numChildren = numChildren;
    node->children = (ASTNode **)malloc(numChildren * sizeof(ASTNode *));
    va_list args;
    va_start(args, numChildren);
    for (int i = 0; i < numChildren; i++) {
        node->children[i] = va_arg(args, ASTNode *);
    }
    va_end(args);
    return node;
}

// Helper function to create an AST leaf node
ASTNode *createASTLeaf(char *type, char *text) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->nodeType = strdup(type);
    node->text = text ? strdup(text) : NULL;
    node->numChildren = 0;
    node->children = NULL;
    return node;
}

// Function to print the AST
void printAST(ASTNode *node, int level) {
    if (!node) return;
    for (int i = 0; i < level; i++) printf("  ");
    printf("%s", node->nodeType);
    if (node->text) printf(" (%s)", node->text);
    printf("\n");
    for (int i = 0; i < node->numChildren; i++) {
        printAST(node->children[i], level + 1);
    }
}

// Function to free the AST
void freeAST(ASTNode *node) {
    if (!node) return;
    if (node->nodeType) free(node->nodeType);
    if (node->text) free(node->text);
    for (int i = 0; i < node->numChildren; i++) {
        freeAST(node->children[i]);
    }
    if (node->children) free(node->children);
    free(node);
}

#endif