#include "exp.h"
#include "../../utils/util.h"
#include <stdlib.h>

type_def *exp_semantic(program_manager *pm, ASTNode *node);

type_def *exp_id_semantic(program_manager *pm, ASTNode *node)
{
    // the node come in is the ID leaf node
    // check usage before declaration
    if (!program_manager_get_field(pm, node->text))
    {
        fprintf(stderr, "Error at line %zu: variable %s not declared\n", node->line, node->text);
    }
    type_id id_type = program_manager_get_field(pm, node->text)->type_spec->type_id;
    bool is_struct = program_manager_get_field(pm, node->text)->type_spec->is_struct;
    return type_def_new(id_type, is_struct);
}

type_def *exp_bi_op_semantic(program_manager *pm, ASTNode *left, char *op, ASTNode *right)
{
    bool error_node = false;
    if (!left || !right)
    {
        fprintf(stderr, "Error: Invalid binary operation, left operand or right operand contains error\n");
        error_node = true;
    }
    // first, check if left and right are EXP node with ID leaf node
    // check usage before declaration
    if (alist_get(left->children, 0) && !strcmp(alist_get(left->children, 0)->nodeType, "ID"))
    {
        if (!program_manager_get_field(pm, left->text))
        {
            fprintf(stderr, "Error at line %zu: variable %s not declared\n", left->line, left->text);
            error_node = true;
        }
    }
    if (alist_get(right->children, 0) && !strcmp(alist_get(right->children, 0)->nodeType, "ID"))
    {
        if (!program_manager_get_field(pm, right->text))
        {
            fprintf(stderr, "Error at line %zu: variable %s not declared\n", right->line, right->text);
            error_node = true;
        }
    }

    // check type compatibility for binary operations
    if (!strcmp(op, "="))
    {
        type_def *left_type = exp_semantic(pm, left);
        type_def *right_type = exp_semantic(pm, right);
        if (left_type && right_type)
        {
            // check type compatibility: arrays and structs are not supported
            if (left_type->is_array || left_type->is_struct || right_type->is_array || right_type->is_struct)
            {
                fprintf(stderr, "Error at line %zu: type mismatch, arrays and structs can neither be assigned to or assigned from\n", left->line);
                error_node = true;
            }

            if (left_type->type_id != right_type->type_id)
            {
                fprintf(stderr, "Error at line %zu: type mismatch, try to assign type '%s' to type '%s'\n", left->line, right->nodeType, left->nodeType);
                error_node = true;
            }
            if(error_node){
                return NULL;
            }
            return type_def_new(right_type->type_id, false);
        }
        else
        {
            fprintf(stderr, "Error: Invalid binary operation, left operand or right operand contains error\n");
            error_node = true;
        }
    }
    else if (!strcmp(op, "+") || !strcmp(op, "-") || !strcmp(op, "*"))
    {
        type_def *left_type = exp_semantic(pm, left);
        type_def *right_type = exp_semantic(pm, right);
        if (left_type->type_id != TYPE_INT && left_type->type_id != TYPE_FLOAT && left_type->type_id != TYPE_CHAR)
        {
            fprintf(stderr, "Error at line %zu: invalid type for operation %s on left expression\n", left->line, op);
            error_node = true;
        }
        if (right_type->type_id != TYPE_INT && right_type->type_id != TYPE_FLOAT && right_type->type_id, TYPE_CHAR)
        {
            fprintf(stderr, "Error at line %zu: invalid type for operation %s on right expression\n", right->line, op);
            error_node = true;
        }
        // CHAR + INT = CHAR, CHAR + FLOAT = FLOAT, CHAR + CHAR = CHAR, INT + CHAR = CHAR,INT + INT = INT, INT + FLOAT = FLOAT, FLOAT + FLOAT = FLOAT
        if(error_node){
            return NULL;
        }

        // if one of the operands is a float, the result is a float
        if (left_type->type_id == TYPE_FLOAT || right_type->type_id == TYPE_FLOAT)
        {
            return type_def_new(TYPE_FLOAT, false);
        }
        // if one of the operands is a int, the result is a int
        else if (left_type->type_id == TYPE_INT || right_type->type_id == TYPE_INT)
        {
            return type_def_new(TYPE_INT, false);
        }
        // if both operands are char, the result is a char
        else if (left_type->type_id == TYPE_CHAR && right_type->type_id == TYPE_CHAR)
        {
            return type_def_new(TYPE_CHAR, false);
        }
    }
    else if (!strcmp(op, "/"))
    {
        type_def *left_type = exp_semantic(pm, left);
        type_def *right_type = exp_semantic(pm, right);
        if (left_type->type_id != TYPE_INT && left_type->type_id != TYPE_FLOAT && left_type->type_id != TYPE_CHAR)
        {
            fprintf(stderr, "Error at line %zu: invalid type for operation %s on left expression\n", left->line, op);
            error_node = true;
        }
        if (right_type->type_id != TYPE_INT && right_type->type_id != TYPE_FLOAT && right_type->type_id != TYPE_CHAR)
        {
            fprintf(stderr, "Error at line %zu: invalid type for operation %s on right expression\n", right->line, op);
            error_node = true;
        }
        // check divide-by-zero, such as 0, 0.0f, .0f, 0.0, .0, 0x0
        if (!strcmp(right->text, "0") && !strcmp(right->text, "0.0f") && !strcmp(right->text, ".0f") && !strcmp(right->text, "0.0") && !strcmp(right->text, ".0") && !strcmp(right->text, "0x0"))
        {
            fprintf(stderr, "Error at line %zu: divide by zero\n", right->line);
            error_node = true;
        }
        if (error_node)
        {
            return NULL;
        }
        // CHAR / INT = CHAR, CHAR / FLOAT = FLOAT, CHAR / CHAR = CHAR, INT / CHAR = CHAR,INT / INT = INT, INT / FLOAT = FLOAT, FLOAT / FLOAT = FLOAT

        // if one of the operands is a float, the result is a float
        if (left_type->type_id == TYPE_FLOAT || right_type->type_id == TYPE_FLOAT)
        {
            return type_def_new(TYPE_FLOAT, false);
        }
        // if one of the operands is a int, the result is a int
        else if (left_type->type_id == TYPE_INT || right_type->type_id == TYPE_INT)
        {
            return type_def_new(TYPE_INT, false);
        }
        // if both operands are char, the result is a char
        else if (left_type->type_id == TYPE_CHAR && right_type->type_id == TYPE_CHAR)
        {
            return type_def_new(TYPE_CHAR, false);
        }
    }
    else if (!strcmp(op, "<") || !strcmp(op, ">") || !strcmp(op, "<=") || !strcmp(op, ">=") || !strcmp(op, "==") || !strcmp(op, "!="))
    {
        type_def *left_type = exp_semantic(pm, left);
        type_def *right_type = exp_semantic(pm, right);
        if (left_type->type_id != TYPE_INT && left_type->type_id != TYPE_FLOAT && left_type->type_id != TYPE_CHAR)
        {
            fprintf(stderr, "Error at line %zu: invalid type for operation %s on left expression\n", left->line, op);
            error_node = true;
        }
        if (right_type->type_id != TYPE_INT && right_type->type_id != TYPE_FLOAT && right_type->type_id != TYPE_CHAR)
        {
            fprintf(stderr, "Error at line %zu: invalid type for operation %s on right expression\n", right->line, op);
            error_node = true;
        }
        if (left_type->type_id != right_type->type_id)
        {
            fprintf(stderr, "Error at line %zu: type mismatch, try to compare type '%s' with type '%s'\n", left->line, right->nodeType, left->nodeType);
            error_node = true;
        }

        if (error_node)
        {
            return NULL;
        }
        return type_def_new(TYPE_INT, false);
    }
    else if (!strcmp(op, "&&") || !strcmp(op, "||"))
    {
        type_def *left_type = exp_semantic(pm, left);
        type_def *right_type = exp_semantic(pm, right);
        if (left_type->type_id != TYPE_INT)
        {
            fprintf(stderr, "Error at line %zu: invalid type for operation %s on left expression\n", left->line, op);
            error_node = true;
        }
        if (right_type->type_id != TYPE_INT)
        {
            fprintf(stderr, "Error at line %zu: invalid type for operation %s on right expression\n", right->line, op);
            error_node = true;
        }
        if (error_node)
        {
            return NULL;
        }
        return type_def_new(TYPE_INT, false);
    }
    if (error_node)
    {
        return NULL;
    }
    return type_def_new(TYPE_VOID, false);
}

type_def *exp_unary_op_semantic(program_manager *pm, char *op, ASTNode *child)
{
    bool error_node = false;
    if (!exp_semantic(pm, child))
    {
        fprintf(stderr, "Error: Invalid unary operation, child is NULL\n");
        error_node = true;
    }

    if (alist_get(child->children, 0) && !strcmp(alist_get(child->children, 0)->nodeType, "ID"))
    {
        if (!program_manager_get_field(pm, child->text))
        {
            fprintf(stderr, "Error at line %zu: variable %s not declared\n", child->line, child->text);
            error_node = true;
        }
        else
        {
            type_def *child_type = exp_semantic(pm, child);
            if (child_type->type_id != TYPE_INT && child_type->type_id != TYPE_FLOAT)
            {
                fprintf(stderr, "Error at line %zu: invalid type for operation %s on child expression\n", child->line, op);
                error_node = true;
            }
            // int a = ++a;
            // --a;
            // -++a;
            // +--a;
            // +a;
            // -a;
            return type_def_new(child_type->type_id, false);
        }
    }
    else
    {
        type_def *child_type = exp_semantic(pm, child);
        if (child_type->type_id != TYPE_INT && child_type->type_id != TYPE_FLOAT)
        {
            fprintf(stderr, "Error at line %zu: invalid type for operation %s on child expression\n", child->line, op);
            error_node = true;
            return NULL;
        }
        return type_def_new(child_type->type_id, false);
    }
}

type_def *exp_func_semantic(program_manager *pm, ASTNode *func_id, ASTNode *args)
{
    bool error_node = false;
    // check usage before declaration
    if (!program_manager_get_field(pm, func_id->text))
    {
        fprintf(stderr, "Error at line %zu: function %s not declared\n", func_id->line, func_id->text);
        error_node = true;
    }
    // check if the function is a function
    if (!program_manager_get_func(pm, func_id->text))
    {
        fprintf(stderr, "Error at line %zu: %s is not a function\n", func_id->line, func_id->text);
        error_node = true;
    }
    else
    {
        // check if the function is called with the correct number of arguments
        if (program_manager_get_func(pm, func_id->text)->args->count != alist_count(args))
        {
            // fprintf(stderr, "Error at line %zu: function %s is called with %zu arguments, but it expects %zu arguments\n", func_id->line, func_id->text, alist_count(args), program_manager_get_func(pm, func_id->text)->args_count);
            // error_node = true;
        }
        else
        {
            // check if the arguments are of the correct type
        }
    }
    if (error_node)
    {
        return NULL;
    }
    return type_def_new(program_manager_get_field(pm, func_id->text)->type_spec->type_id, false);
}

type_def *exp_array_semantic(program_manager *pm, ASTNode *exp1, ASTNode *exp2)
{
    bool error_node = false;
    // check usage before declaration
    if (!program_manager_get_field(pm, exp1->text))
    {
        fprintf(stderr, "Error at line %zu: variable %s not declared\n", exp1->line, exp1->text);
        error_node = true;
    }
    // check if the variable is an array
    if (!program_manager_get_field(pm, exp1->text)->type_spec->is_array)
    {
        fprintf(stderr, "Error at line %zu: %s is not an array\n", exp1->line, exp1->text);
        error_node = true;
    }
    // check if the index is an integer
    if (exp_semantic(pm, exp2)->type_id != TYPE_INT)
    {
        fprintf(stderr, "Error at line %zu: invalid type for array index\n", exp2->line);
        error_node = true;
    }

    if (error_node)
    {
        return NULL;
    }
    return type_def_new(program_manager_get_field(pm, exp1->text)->type_spec->type_id, false);
}

/**
 * @brief Perform semantic analysis for a struct member expression: struct.member
 * @param pm The program manager to manage scopes and symbols.
 * @param exp The AST node representing the struct expression.
 * @param id The name of the struct member.
 */

type_def *exp_struct_semantic(program_manager *pm, ASTNode *exp, char *id)
{
    bool error_node = false;
    // check usage before declaration
    if (!program_manager_get_struct_by_id(pm, id))
    {
        fprintf(stderr, "Error at line %zu: struct %s not declared\n", exp->line, id);
        error_node = true;
    }
    // check if id is a valid struct member
    if(!struct_def_get_field(program_manager_get_struct_by_id(pm, id), id)){
        fprintf(stderr, "Error at line %zu: struct %s does not have a member %s\n", exp->line, exp->text, id);
        error_node = true;
    }

    if (error_node)
    {
        return NULL;
    }
    return type_def_new(program_manager_get_field(pm, id)->type_spec->type_id, true);
}

type_def *exp_struct_func_semantic(program_manager *pm, ASTNode *exp, char *id, ASTNode *args);

type_def *exp_primitive_semantic(program_manager *pm, char *type, char *text);
