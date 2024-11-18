#ifndef __AST_H__
#define __AST_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "../utils/util.h"

typedef struct ASTNodeList ASTNodeList;

// AST Node structure
typedef struct ASTNode {
    char *nodeType;          // Type of the node (e.g., "Program", "ExtDefList")
    char *text;              // Token text, if any (for leaves)
    int numChildren;         // Number of child nodes
    ASTNodeList *children;   // List of child nodes (ASTNode *)
} ASTNode;

// Function declarations

// Helper function to create an AST node with variable number of children
ASTNode *createASTNode(char *type, int numChildren, ...);

// Helper function to create an AST leaf node
ASTNode *createASTLeaf(char *type, char *text);

// Function to print the AST
void printAST(ASTNode *node, int level);

// Function to print the whole AST recursively
void rprintAST(ASTNode *node, int level);

// Function to free the AST
void freeAST(ASTNode *node);

#define SNAME ASTNodeList
#define PFX alist
#define V ASTNode*
#define CMC_EXT_STR
#include "../collections/list_h.h"

extern struct ASTNodeList_fval alist_fvals;

#endif