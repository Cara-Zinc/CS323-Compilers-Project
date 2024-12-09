#include "stmt.h"
#include "exp.h"

void stmt_semantic(program_manager *pm, ASTNode *node, func_def *func)
{
    if (node->numChildren == 1)
    {
        if (strcmp(alist_get(node->children, 0)->nodeType, "Stmt") == 0)
        {
            if (strcmp(alist_get(alist_get(node->children, 0)->children, 0)->nodeType, "Compst"))
            {
                compst_semantic(pm, alist_get(node->children, 0), func);
            }
            else
            {
                exp_semantic(pm, alist_get(node->children, 0));
            }
        }
        else
        {
            return_semantic(pm, alist_get(node->children, 0), func);
        }
    }
    else if (node->numChildren == 2)
    {
        if (strcmp(node->nodeType, "IF") == 0)
        {
            if_semantic(pm, node, func);
        }
        else
        {
            while_semantic(pm, node, func);
        }
    }
    else
    {
        if_else_semantic(pm, alist_get(node->children, 0), func);
    }

}

void return_semantic(program_manager *pm, ASTNode *node, func_def *func) {
    type_def *exp_type = exp_semantic(pm, alist_get(node->children, 0));
    char* exp_type_name = type_def_name(pm, exp_type);
    char* func_type_name = type_def_name(pm, func->return_type);

    if (strcmp(exp_type_name, func_type_name) != 0) {
        fprintf(stderr, "Error at line %zu: return type mismatch, expected %s but got %s.\n", node->line, func_type_name, exp_type_name);
    }
}

void compst_semantic(program_manager *pm, ASTNode *node, func_def *func) {
    program_manager_create_subscope(pm);
    DefList_semantic(pm, alist_get(node->children, 0));
    StmtList_semantic(pm, alist_get(node->children, 1), func);
    program_manager_pop(pm);
}

void if_semantic(program_manager *pm, ASTNode *node, func_def *func) {
    type_def *exp_type = exp_semantic(pm, alist_get(node->children, 0));
    if (strcmp(type_def_name(pm, exp_type), "int") != 0) {
        fprintf(stderr, "Error at line %zu: condition of if statement must be of type int.\n", node->line);
    }
    stmt_semantic(pm, alist_get(node->children, 1), func);
}

void while_semantic(program_manager *pm, ASTNode *node, func_def *func) {
    type_def *exp_type = exp_semantic(pm, alist_get(node->children, 0));
    if (strcmp(type_def_name(pm, exp_type), "int") != 0) {
        fprintf(stderr, "Error at line %zu: condition of while statement must be of type int.\n", node->line);
    }
    stmt_semantic(pm, alist_get(node->children, 1), func);
}

void if_else_semantic(program_manager *pm, ASTNode *node, func_def *func) {
    type_def *exp_type = exp_semantic(pm, alist_get(node->children, 0));
    if (strcmp(type_def_name(pm, exp_type), "int") != 0) {
        fprintf(stderr, "Error at line %zu: condition of if statement must be of type int.\n", node->line);
    }
    stmt_semantic(pm, alist_get(node->children, 1), func);
    stmt_semantic(pm, alist_get(node->children, 2), func);
}

void stmtList_semantic(program_manager *pm, ASTNode *node, func_def *func) {
    if (node->numChildren == 0) {
        return;
    }
    stmt_semantic(pm, alist_get(node->children, 0), func);
    stmtList_semantic(pm, alist_get(node->children, 1), func);
}