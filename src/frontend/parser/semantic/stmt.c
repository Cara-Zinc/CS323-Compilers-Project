#include "stmt.h"
#include "exp.h"

void stmt_semantic(program_manager *pm, ASTNode *node, func_def *func)
{
    if (node->numChildren == 1)
    {
        if (strcmp(node->nodeType, "Stmt") == 0)
        {
            if (strcmp(alist_get(node->children, 0)->nodeType, "CompSt") == 0)
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
    type_def *exp_type = exp_semantic(pm, node);
    char* exp_type_name = type_def_name(pm, exp_type);
    char* func_type_name = type_def_name(pm, func->return_type);

    if (strcmp(exp_type_name, func_type_name) != 0) {
        fprintf(stderr, "Error at line %zu: return type mismatch, expected %s but got %s.\n", node->line, func_type_name, exp_type_name);
    }
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

void stmtlist_semantic(program_manager *pm, ASTNode *node, func_def *func) {
    if (node->numChildren == 0) {
        return;
    }
    stmt_semantic(pm, alist_get(node->children, 0), func);
    stmtlist_semantic(pm, alist_get(node->children, 1), func);
}