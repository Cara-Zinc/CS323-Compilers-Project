#include "../parser/ast.h"

#define SNAME ASTNodeList
#define PFX alist
#define V ASTNode*
#define CMC_EXT_STR
#include "list.h"

struct ASTNodeList_fval alist_fvals = {
    .cmp = NULL,
    .cpy = NULL,
    .str = NULL,
    .free = freeAST,
};