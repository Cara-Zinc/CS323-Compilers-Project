#ifndef __AST_H__
#define __AST_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "../utils/util.h"

// AST Node structure
typedef struct ASTNode {
    char *nodeType;          // Type of the node (e.g., "Program", "ExtDefList")
    char *text;              // Token text, if any (for leaves)
    int numChildren;         // Number of child nodes
    ASTNodeList *children;   // List of child nodes (ASTNode *)
} ASTNode;

#define SNAME ASTNodeList
#define PFX alist
#define V ASTNode*
#define CMC_EXT_STR
#include <cmc/list.h>

// Function declarations
ASTNode *createASTNode(char *type, int numChildren, ...);
ASTNode *createASTLeaf(char *type, char *text);
void printAST(ASTNode *node, int level);
void freeAST(ASTNode *node);

typedef struct ASTNodeList ASTNodeList;

struct ASTNodeList_fval alist_fvals = {
    .cmp = NULL,
    .cpy = NULL,
    .str = NULL,
    .free = freeAST,
};

// Helper function to create an AST node with variable number of children
ASTNode *createASTNode(char *type, int numChildren, ...) {
    ASTNode *node = new(ASTNode);
    node->nodeType = strdup(type);
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

// Helper function to create an AST leaf node
ASTNode *createASTLeaf(char *type, char *text) {
    ASTNode *node = new(ASTNode);
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
    alist_print(node->children, stdout, " ", " ", " ");
}

// Function to print the whole AST recursively
void rprintAST(ASTNode *node, int level) {
    if (!node) return;

    // Print the current node with appropriate indentation
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("%s", node->nodeType);
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


// Function to free the AST
void freeAST(ASTNode *node) {
    if (!node) return;
    if (node->nodeType) free(node->nodeType);
    if (node->text) free(node->text);
    alist_free(node->children);
    free(node);
}

#endif