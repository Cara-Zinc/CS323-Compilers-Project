#include "stmt.h"

void stmt_semantic(program_manager *pm, ASTNode *node, func_def *func) {
    if (strcmp(node->nodeType, "Exp") == 0) {
        exp_semantic(pm, node);
    } else if (strcmp(node->nodeType, "CompSt") == 0) {
        return compst_semantic(pm, node, func);
    } else if (strcmp(node->nodeType, "RETURN") == 0) {
        return return_semantic(pm, node, func);
    } else if (strcmp(node->nodeType, "IF") == 0) {
        return if_semantic(pm, node, func);
    } else if (strcmp(node->nodeType, "WHILE") == 0) {
        return while_semantic(pm, node, func);
    } else {
        fprintf(stderr, "Error at line %zu: unknown statement type %s.\n", node->line, node->nodeType);
        return NULL;
    }
}