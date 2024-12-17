#include "args.h"
#include "exp.h"
#include "../../utils/util.h"
#include <stdlib.h>

type_def *exp_semantic(program_manager *pm, ASTNode *node)
{
    if (node->numChildren == 1)
    {
        if (strcmp(alist_get(node->children, 0)->nodeType, "ID") == 0)
        {
            return exp_id_semantic(pm, alist_get(node->children, 0));
        }
        else if (strcmp(alist_get(node->children, 0)->nodeType, "INT") == 0)
        {
            return type_def_new(TYPE_INT, false);
        }
        else if (strcmp(alist_get(node->children, 0)->nodeType, "FLOAT") == 0)
        {
            return type_def_new(TYPE_FLOAT, false);
        }
        else if (strcmp(alist_get(node->children, 0)->nodeType, "CHAR") == 0)
        {
            return type_def_new(TYPE_CHAR, false);
        }
    }
    else if (node->numChildren == 2)
    {
        if (strcmp(alist_get(node->children, 0)->nodeType, "op") == 0)
        {
            return exp_unary_op_semantic(pm, node->text, alist_get(node->children, 0));
        }
        else if (strcmp(alist_get(node->children, 0)->nodeType, "ID") == 0)
        {
            return exp_func_semantic(pm, alist_get(node->children, 0), alist_get(node->children, 1));
        }
        else if (strcmp(alist_get(node->children, 0)->nodeType, "Exp") == 0)
        {
            return exp_struct_semantic(pm, alist_get(node->children, 0), alist_get(node->children, 1));
        }
    }
    else if (node->numChildren == 3)
    {
        if (strcmp(alist_get(node->children, 1)->nodeType, "operator") == 0)
        {
            return exp_bi_op_semantic(pm, alist_get(node->children, 0), alist_get(node->children, 1)->text, alist_get(node->children, 2));
        }
        else if (strcmp(alist_get(node->children, 1)->nodeType, "op") == 0)
        {
            return exp_bi_op_semantic(pm, alist_get(node->children, 0), alist_get(node->children, 1)->text, alist_get(node->children, 2));
        }
        else if (strcmp(alist_get(node->children, 1)->nodeType, "ID") == 0)
        {
            return exp_struct_func_semantic(pm, alist_get(node->children, 0), alist_get(node->children, 1), alist_get(node->children, 2));
        }
    }
    else if (node->numChildren == 4)
    {
        return exp_array_semantic(pm, alist_get(node->children, 0), alist_get(node->children, 2));
    }
    fprintf(stderr, "Error at line %zu: invalid expression with node type '%s' and %d children\n", node->line, node->nodeType, node->numChildren);
    return type_def_new(TYPE_VOID, false);
}

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
        ASTNode *child = alist_get(left->children, 0);
        // assume this is a valid ID node
        if (!program_manager_get_field(pm, child->text))
        {
            fprintf(stderr, "Error at line %zu: variable %s not declared\n", left->line, left->text);
            error_node = true;
        }
    }
    if (alist_get(right->children, 0) && !strcmp(alist_get(right->children, 0)->nodeType, "ID"))
    {
        ASTNode *child = alist_get(right->children, 0);
        // assume this is a valid ID node
        if (!program_manager_get_field(pm, child->text))
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
            if (error_node)
            {
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
        if (right_type->type_id != TYPE_INT && right_type->type_id != TYPE_FLOAT && right_type->type_id != TYPE_CHAR)
        {
            fprintf(stderr, "Error at line %zu: invalid type for operation %s on right expression\n", right->line, op);
            error_node = true;
        }
        // CHAR + INT = CHAR, CHAR + FLOAT = FLOAT, CHAR + CHAR = CHAR, INT + CHAR = CHAR,INT + INT = INT, INT + FLOAT = FLOAT, FLOAT + FLOAT = FLOAT
        if (error_node)
        {
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
        return NULL;
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
            if(error_node) {
                return NULL;
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
    if(error_node) {
        return NULL;
    }
    return type_def_new(TYPE_VOID, false);
}

type_def *exp_func_semantic(program_manager *pm, ASTNode *func_id, ASTNode *args)
{
    bool error_node = false;
    // check if the function is a function
    if (!program_manager_get_func(pm, func_id->text))
    {
        fprintf(stderr, "Error at line %zu: %s is not a function\n", func_id->line, func_id->text);
        error_node = true;
    }
    else
    {
        // check if the function is called with the correct number of arguments
        varlist *args_list = args_semantic(pm, args);
        func_def *func = program_manager_get_func(pm, func_id->text);
        int fun_args_cnt = vlist_count(func->args);

        if (fun_args_cnt != vlist_count(args_list))
        {
            fprintf(stderr, "Error at line %zu: function %s is called with %zu arguments, but it expects %d arguments\n", func_id->line, func_id->text, vlist_count(args_list), fun_args_cnt);
            error_node = true;
        }

        // check if the arguments are of the correct type
        // vlist = args_semantic(pm, args);
        for (int i = 0; i < fun_args_cnt; i++)
        {
            // @TODO: use util function to get name of a type from its type_id
            if (type_def_cmp(vlist_get(args_list, i)->type_spec, vlist_get(func->args, i)->type_spec) != 0)
            {
                // fprintf(stderr, "Error at line %zu: argument %d of function %s is of type %s, but it expects type %s\n", func_id->line, i, func_id->text)
                error_node = true;
            }
        }
        if (error_node)
        {
            return NULL;
        }

        return program_manager_get_func(pm, func_id->text)->return_type;
    }

    return type_def_new(TYPE_VOID, false);
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
    if (exp_semantic(pm, exp2)->type_id != TYPE_INT || exp_semantic(pm, exp2)->is_array || exp_semantic(pm, exp2)->is_struct)
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
type_def *exp_struct_semantic(program_manager *pm, ASTNode *exp, ASTNode *id)
{
    bool error_node = false;
    // check if exp node itself is a struct
    type_def *exp_type = exp_semantic(pm, exp);
    if (!exp_type->is_struct)
    {
        fprintf(stderr, "Error at line %zu: expression is not a struct\n", exp->line);
        error_node = true;
    }
    // check if the struct has the member
    if (!program_manager_get_struct_by_id(pm, exp_type->type_id))
    {
        fprintf(stderr, "Error at line %zu: struct does not have member %s\n", exp->line, id->nodeType);
        error_node = true;
    }
    if (error_node)
    {
        return NULL;
    }
    struct_def *s = program_manager_get_struct_by_id(pm, exp_type->type_id);
    if (!struct_def_get_field(s, id->text))
    {
        fprintf(stderr, "Error at line %zu: struct does not have member %s\n", id->line, id->text);
        return NULL;
    }
    else
    {
        field_def *f = struct_def_get_field(s, id->text);
        return f->type_spec;
    }
}

type_def *exp_struct_func_semantic(program_manager *pm, ASTNode *exp, ASTNode *id, ASTNode *args)
{
    // @TODO: implement this function
    bool error_node = false;
    // check if exp node itself is a struct
    type_def *exp_type = exp_semantic(pm, exp);
    if (!exp_type->is_struct)
    {
        fprintf(stderr, "Error at line %zu: expression is not a struct\n", exp->line);
        error_node = true;
    }
    // check if the struct has the member
    if (!program_manager_get_struct_by_id(pm, exp_type->type_id))
    {
        fprintf(stderr, "Error at line %zu: struct does not have member %s\n", exp->line, id->nodeType);
        error_node = true;
    }
    if (error_node)
    {
        return NULL;
    }
    struct_def *s = program_manager_get_struct_by_id(pm, exp_type->type_id);
    if (!struct_def_get_field(s, id->text))
    {
        fprintf(stderr, "Error at line %zu: struct does not have member %s\n", id->line, id->text);
        return NULL;
    }
    else if (!struct_def_get_func(s, id->text))
    {
        fprintf(stderr, "Error at line %zu: struct does not have function %s\n", id->line, id->text);
        return NULL;
    }
    else
    {
        func_def *f = struct_def_get_func(s, id->text);
        // check if the function is called with the correct number of arguments
        varlist *args_list = args_semantic(pm, args);
        int fun_args_cnt = vlist_count(f->args);
        if (fun_args_cnt != vlist_count(args_list))
        {
            fprintf(stderr, "Error at line %zu: function %s is called with %zu arguments, but it expects %d arguments\n", id->line, id->text, vlist_count(args_list), fun_args_cnt);
            error_node = true;
        }
        // check if the arguments are of the correct type
        for (int i = 0; i < fun_args_cnt; i++)
        {
            type_def *t1 = vlist_get(args_list, i)->type_spec;
            type_def *t2 = vlist_get(f->args, i)->type_spec;
            if (type_def_cmp(t1, t2) != 0)
            {
                fprintf(stderr, "Error at line %zu: argument %d of function %s is of type %s, but it expects type %s\n", id->line, i, id->text, type_def_name(pm, t1), type_def_name(pm, t2));
                error_node = true;
            }
        }
        if (error_node)
        {
            return NULL;
        }
        return f->return_type;
    }
}
