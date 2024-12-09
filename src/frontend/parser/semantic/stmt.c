#include "stmt.h"

void stmt_semantic(program_manager *pm, ASTNode *node, func_def *func) {
    //
    if (strcmp(node->nodeType, "Exp") == 0) {
        exp_semantic(pm, node);
    } else if (strcmp(node->nodeType, "CompSt") == 0) {
        compst_semantic(pm, node, func);
    } else if (strcmp(node->nodeType, "RETURN") == 0) {
        return_semantic(pm, node, func);
    } else if (strcmp(node->nodeType, "IF") == 0) {
        if_semantic(pm, node, func);
    } else if (strcmp(node->nodeType, "WHILE") == 0) {
        while_semantic(pm, node, func);
    } else {
        fprintf(stderr, "Error at line %zu: unknown statement type %s.\n", node->line, node->nodeType);
    }
}

void return_semantic(program_manager *pm, ASTNode *node, func_def *func) {
    type_def *return_type = func->return_type;
    if (alist_size(node->children) == 1) {
        type_def *exp_type = exp_semantic(pm, alist_get(node->children, 0));
        if (!type_def_equal(return_type, exp_type)) {
            fprintf(stderr, "Error at line %zu: return type mismatch.\n", node->line);
        }
    } else {
        if (!type_def_equal(return_type, type_def_new("void"))) {
            fprintf(stderr, "Error at line %zu: return type mismatch.\n", node->line);
        }
    }
}

void compst_semantic(program_manager *pm, ASTNode *node, func_def *func) {
    program_manager_push(pm);
    DefList_semantic(pm, alist_get(node->children, 0));
    StmtList_semantic(pm, alist_get(node->children, 1), func);
    program_manager_pop(pm);
}

void DefList_semantic(program_manager *pm, ASTNode *node) {
    if (node == NULL) {
        return;
    }

    DefList_semantic(pm, alist_get(node->children, 0));
    dec_semantic(pm, alist_get(node->children, 1));
}

void StmtList_semantic(program_manager *pm, ASTNode *node, func_def *func) {
    if (node == NULL) {
        return;
    }

    StmtList_semantic(pm, alist_get(node->children, 0), func);
    stmt_semantic(pm, alist_get(node->children, 1), func);
}

void if_semantic(program_manager *pm, ASTNode *node, func_def *func) {
    type_def *exp_type = exp_semantic(pm, alist_get(node->children, 2));
    if (!type_def_equal(exp_type, type_def_new("int"))) {
        fprintf(stderr, "Error at line %zu: if condition must be of type int.\n", node->line);
    }

    stmt_semantic(pm, alist_get(node->children, 4), func);
    if (alist_size(node->children) == 7) {
        stmt_semantic(pm, alist_get(node->children, 6), func);
    }
}

void while_semantic(program_manager *pm, ASTNode *node, func_def *func) {
    type_def *exp_type = exp_semantic(pm, alist_get(node->children, 2));
    if (!type_def_equal(exp_type, type_def_new("int"))) {
        fprintf(stderr, "Error at line %zu: while condition must be of type int.\n", node->line);
    }

    stmt_semantic(pm, alist_get(node->children, 4), func);
}