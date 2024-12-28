#include "stmt.h"
#include "exp.h"
#include "compst.h"

stmt *stmt_semantic(program_manager *pm, ASTNode *node, func_def *func)
{
    if (node->numChildren == 1)
    {
        ASTNode *child = alist_get(node->children, 0);
        if (strcmp(child->nodeType, "RETURN") == 0)
        {
            return return_semantic(pm, child, func);
        }
        else if (strcmp(child->nodeType, "CompSt") == 0)
        {
            return compst_semantic(pm, child, func);
        }
        else if (strcmp(child->nodeType, "IF") == 0)
        {
            return if_semantic(pm, child, func);
        }
        else if (strcmp(child->nodeType, "WHILE") == 0)
        {
            return while_semantic(pm, child, func);
        }
        else if (strcmp(child->nodeType, "Exp") == 0)
        {
            return stmt_new_exp(exp_semantic(pm, child));
        }
    }
    else if (node->numChildren == 2)
    {
        return if_else_semantic(pm, node, func);
    }

    fprintf(stderr, "Error at line %zu: invalid statement, whose children amount is %d\n", node->line, node->numChildren);
    return stmt_new_invalid();
}

stmt *return_semantic(program_manager *pm, ASTNode *node, func_def *func)
{
    ASTNode *exp_node = alist_get(node->children, 0);
    exp *return_exp = exp_semantic(pm, exp_node);

    if (type_def_cmp(return_exp->result_type, func->return_type) != 0)
    {
        char *exp_type_name = type_def_name(pm, return_exp->result_type);
        char *func_type_name = type_def_name(pm, func->return_type);
        fprintf(stderr, "Error at line %zu: return type mismatch, expected %s but got %s.\n", node->line, func_type_name, exp_type_name);
        str_free(exp_type_name);
        str_free(func_type_name);
        exp_free(return_exp);
        return stmt_new_invalid();
    }

    return stmt_new_return(return_exp);
}

stmt *compst_semantic(program_manager *pm, ASTNode *node, func_def *func)
{
    program_manager_create_subscope(pm);
    stmtlist *stmts = NULL;
    if (node->numChildren == 2)
    {
        deflist_semantic(pm, alist_get(node->children, 0));
        stmts = stmtlist_semantic(pm, alist_get(node->children, 1), func);
    }
    else
    {
        stmts = stmtlist_semantic(pm, alist_get(node->children, 0), func);
    }
    scope_wrapper *wrapper = program_manager_pop(pm);
    scope_wrapper_free_without_data(wrapper);
    return stmt_new_comp(stmts);
}

stmt *if_semantic(program_manager *pm, ASTNode *node, func_def *func)
{
    ASTNode *predicate_node = alist_get(node->children, 0);
    exp *predicate_exp = exp_semantic(pm, predicate_node);
    type_def *exp_type = predicate_exp->result_type;
    if (!(type_def_is_primitive(exp_type) && exp_type->type_id == TYPE_INT))
    {
        fprintf(stderr, "Error at line %zu: condition of if statement must be of type int.\n", predicate_node->line);
        exp_free(predicate_exp);
        return stmt_new_invalid();
    }
    stmt *if_stmt = stmt_semantic(pm, alist_get(node->children, 1), func);
    return stmt_new_if(predicate_exp, if_stmt, NULL);
}

stmt *while_semantic(program_manager *pm, ASTNode *node, func_def *func)
{
    exp *predicate_exp = exp_semantic(pm, alist_get(node->children, 0));
    type_def *exp_type = predicate_exp->result_type;
    if (!(type_def_is_primitive(exp_type) && exp_type->type_id == TYPE_INT))
    {
        fprintf(stderr, "Error at line %zu: condition of while statement must be of type int.\n", node->line);
        exp_free(predicate_exp);
        return stmt_new_invalid();
    }
    stmt *while_stmt = stmt_semantic(pm, alist_get(node->children, 1), func);
    return stmt_new_while(predicate_exp, while_stmt);
}

stmt *if_else_semantic(program_manager *pm, ASTNode *node, func_def *func)
{
    ASTNode *if_node = alist_get(node->children, 0);
    ASTNode *else_node = alist_get(node->children, 1);
    exp *predicate_exp = exp_semantic(pm, alist_get(if_node->children, 0));
    type_def *exp_type = predicate_exp->result_type;
    if (!(type_def_is_primitive(exp_type) && exp_type->type_id == TYPE_INT))
    {
        fprintf(stderr, "Error at line %zu: condition of if statement must be of type int.\n", node->line);
        exp_free(predicate_exp);
        return stmt_new_invalid();
    }
    stmt *if_stmt = stmt_semantic(pm, alist_get(if_node->children, 1), func);
    stmt *else_stmt = stmt_semantic(pm, alist_get(else_node->children, 0), func);
    return stmt_new_if(predicate_exp, if_stmt, else_stmt);
}