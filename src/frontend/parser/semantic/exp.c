#include "args.h"
#include "exp.h"
#include "../../utils/util.h"
#include "../../utils/error.h"
#include <stdlib.h>

exp *exp_semantic(program_manager *pm, ASTNode *node)
{
    if (node->numChildren == 1)
    {
        ASTNode *child = alist_get(node->children, 0);
        if (strcmp(child->nodeType, "ID") == 0)
        {
            return exp_id_semantic(pm, child);
        }
        return exp_primitive_semantic(pm, child);
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
    efprintf(stderr, "Error at line %zu: invalid expression with node type '%s' and %d children\n", node->line, node->nodeType, node->numChildren);
    return exp_new_invalid();
}

exp *exp_id_semantic(program_manager *pm, ASTNode *node)
{
    // the node come in is the ID leaf node
    // check usage before declaration
    char *name = node->text;
    field_def *field = program_manager_get_field(pm, name);
    if (field == NULL)
    {
        efprintf(stderr, "Error at line %zu: variable %s not declared\n", node->line, name);
        return exp_new_invalid();
    }
    return exp_new_id(type_def_cpy(field->type_spec), str_copy(name));
}

exp *exp_bi_op_semantic(program_manager *pm, ASTNode *left, char *op, ASTNode *right)
{
    exp *left_exp = exp_semantic(pm, left);
    exp *right_exp = exp_semantic(pm, right);

    if (left_exp->exp_type == EXP_INVALID || right_exp->exp_type == EXP_INVALID)
    {
        exp_free(left_exp);
        exp_free(right_exp);
        return exp_new_invalid();
    }

    type_def *left_type = left_exp->result_type;
    type_def *right_type = right_exp->result_type;
    if (!type_def_is_operable(left_type))
    {
        char *left_type_name = type_def_name(pm, left_type);
        efprintf(stderr, "Error at line %zu: left expression of type %s is not int, float or char\n", left->line, left_type_name);
        str_free(left_type_name);
        exp_free(left_exp);
        exp_free(right_exp);
        return exp_new_invalid();
    }
    if (!type_def_is_operable(right_type))
    {
        char *right_type_name = type_def_name(pm, right_type);
        efprintf(stderr, "Error at line %zu: right expression of type %s is not int, float or char\n", right->line, right_type_name);
        str_free(right_type_name);
        exp_free(left_exp);
        exp_free(right_exp);
        return exp_new_invalid();
    }

    // check type compatibility for binary operations
    if (strcmp(op, "=") == 0)
    {
        if (type_def_cmp(left_type, right_type) != 0)
        {
            char *left_type_name = type_def_name(pm, left_type);
            char *right_type_name = type_def_name(pm, right_type);
            efprintf(stderr, "Error at line %zu: type mismatch, trying to assign type '%s' to type '%s'\n", left->line, right_type_name, left_type_name);
            str_free(left_type_name);
            str_free(right_type_name);
            exp_free(left_exp);
            exp_free(right_exp);
            return exp_new_invalid();
        }
        return exp_new_bi_op(type_def_new_primitive(TYPE_VOID), BI_OP_ASSIGN, left_exp, right_exp);
    }
    else if (!strcmp(op, "+") || !strcmp(op, "-") || !strcmp(op, "*"))
    {
        // CHAR + INT = CHAR, CHAR + FLOAT = FLOAT, CHAR + CHAR = CHAR, INT + CHAR = CHAR,INT + INT = INT, INT + FLOAT = FLOAT, FLOAT + FLOAT = FLOAT
        type_id result_type_id = TYPE_VOID;
        // if one of the operands is a float, the result is a float
        if (left_type->type_id == TYPE_FLOAT || right_type->type_id == TYPE_FLOAT)
        {
            result_type_id = TYPE_FLOAT;
        }
        // if one of the operands is a int, the result is a int
        else if (left_type->type_id == TYPE_INT || right_type->type_id == TYPE_INT)
        {
            result_type_id = TYPE_INT;
        }
        // if both operands are char, the result is a char
        else if (left_type->type_id == TYPE_CHAR && right_type->type_id == TYPE_CHAR)
        {
            result_type_id = TYPE_CHAR;
        }

        exp_bi_op_enum op_enum = BI_OP_PLUS;
        if (!strcmp(op, "+"))
        {
            op_enum = BI_OP_PLUS;
        }
        else if (!strcmp(op, "-"))
        {
            op_enum = BI_OP_MINUS;
        }
        else if (!strcmp(op, "*"))
        {
            op_enum = BI_OP_MUL;
        }

        return exp_new_bi_op(type_def_new_primitive(result_type_id), op_enum, left_exp, right_exp);
    }
    else if (!strcmp(op, "/"))
    {
        // CHAR / INT = CHAR, CHAR / FLOAT = FLOAT, CHAR / CHAR = CHAR, INT / CHAR = CHAR,INT / INT = INT, INT / FLOAT = FLOAT, FLOAT / FLOAT = FLOAT
        type_id result_type_id = TYPE_VOID;
        // if one of the operands is a float, the result is a float
        if (left_type->type_id == TYPE_FLOAT || right_type->type_id == TYPE_FLOAT)
        {
            result_type_id = TYPE_FLOAT;
        }
        // if one of the operands is a int, the result is a int
        else if (left_type->type_id == TYPE_INT || right_type->type_id == TYPE_INT)
        {
            result_type_id = TYPE_INT;
        }
        // if both operands are char, the result is a char
        else if (left_type->type_id == TYPE_CHAR && right_type->type_id == TYPE_CHAR)
        {
            result_type_id = TYPE_CHAR;
        }
        return exp_new_bi_op(type_def_new_primitive(result_type_id), BI_OP_DIV, left_exp, right_exp);
    }
    else if (!strcmp(op, "<") || !strcmp(op, ">") || !strcmp(op, "<=") || !strcmp(op, ">=") || !strcmp(op, "==") || !strcmp(op, "!="))
    {
        if (type_def_cmp(left_type, right_type) != 0)
        {
            efprintf(stderr, "Error at line %zu: type mismatch, try to compare type '%s' with type '%s'\n", left->line, right->nodeType, left->nodeType);
            exp_free(left_exp);
            exp_free(right_exp);
            return exp_new_invalid();
        }

        exp_bi_op_enum op_enum = BI_OP_EQUAL;
        if (!strcmp(op, "<"))
        {
            op_enum = BI_OP_LESS_THAN;
        }
        else if (!strcmp(op, ">"))
        {
            op_enum = BI_OP_GREATER_THAN;
        }
        else if (!strcmp(op, "<="))
        {
            op_enum = BI_OP_LESS_EQUAL;
        }
        else if (!strcmp(op, ">="))
        {
            op_enum = BI_OP_GREATER_EQUAL;
        }
        else if (!strcmp(op, "=="))
        {
            op_enum = BI_OP_EQUAL;
        }
        else if (!strcmp(op, "!="))
        {
            op_enum = BI_OP_NOT_EQUAL;
        }

        return exp_new_bi_op(type_def_new_primitive(TYPE_INT), op_enum, left_exp, right_exp);
    }
    else if (!strcmp(op, "&&") || !strcmp(op, "||"))
    {
        if (left_type->type_id != TYPE_INT)
        {
            efprintf(stderr, "Error at line %zu: invalid type for operation %s on left expression\n", left->line, op);
            exp_free(left_exp);
            exp_free(right_exp);
            return exp_new_invalid();
        }
        if (right_type->type_id != TYPE_INT)
        {
            efprintf(stderr, "Error at line %zu: invalid type for operation %s on right expression\n", right->line, op);
            exp_free(left_exp);
            exp_free(right_exp);
            return exp_new_invalid();
        }

        if (!strcmp(op, "&&"))
        {
            return exp_new_bi_op(type_def_new_primitive(TYPE_INT), BI_OP_AND, left_exp, right_exp);
        }
        else
        {
            return exp_new_bi_op(type_def_new_primitive(TYPE_INT), BI_OP_OR, left_exp, right_exp);
        }
    }
    eprintf("Error at line %zu: Invalid binary operator %s\n", right->line, op);
    return exp_new_invalid();
}

exp *exp_unary_op_semantic(program_manager *pm, char *op, ASTNode *child)
{
    exp *child_exp = exp_semantic(pm, child);
    if (child_exp->exp_type == EXP_INVALID)
    {
        exp_free(child_exp);
        return exp_new_invalid();
    }

    type_def *child_type = child_exp->result_type;
    if (!type_def_is_primitive(child_type) || (child_type->type_id != TYPE_INT && child_type->type_id != TYPE_FLOAT))
    {
        efprintf(stderr, "Error at line %zu: invalid type for operation %s on child expression\n", child->line, op);
        exp_free(child_exp);
        return exp_new_invalid();
    }

    exp_unary_op_enum op_enum = UNARY_OP_MINUS;
    if (strcmp(op, "-") == 0)
    {
        op_enum = UNARY_OP_MINUS;
    }
    else if (strcmp(op, "!") == 0)
    {
        op_enum = UNARY_OP_NOT;
    }
    else if (strcmp(op, "PLUS") == 0)
    {
        op_enum = UNARY_OP_PLUS;
    }

    return exp_new_unary_op(type_def_cpy(child_type), op_enum, child_exp);
}

exp *exp_func_semantic(program_manager *pm, ASTNode *func_id, ASTNode *args)
{
    func_def *func = program_manager_get_func(pm, func_id->text);
    // check if the function is a function
    if (func == NULL)
    {
        efprintf(stderr, "Error at line %zu: %s is not a function\n", func_id->line, func_id->text);
        return exp_new_invalid();
    }

    // check if the function is called with the correct number of arguments
    if(strcmp(args->nodeType,"EMPTY_ARGS")==0)
    {
        if(vlist_count(func->args) != 0)
        {
            efprintf(stderr, "Error at line %zu: function %s is called with 0 arguments, but it expects %zu arguments\n", func_id->line, func_id->text, vlist_count(func->args));
            return exp_new_invalid();
        }
        return exp_new_func_call(type_def_cpy(func->return_type), NULL, str_copy(func->name), explist_new(1, &explist_fvals));
    }

    explist *args_list = args_semantic(pm, args);
    size_t func_args_cnt = vlist_count(func->args);

    if (func_args_cnt != explist_count(args_list))
    {
        efprintf(stderr, "Error at line %zu: function %s is called with %zu arguments, but it expects %zu arguments\n", func_id->line, func_id->text, explist_count(args_list), func_args_cnt);
        explist_free(args_list);
        return exp_new_invalid();
    }

    // check if the arguments are of the correct type
    for (int i = 0; i < func_args_cnt; i++)
    {
        type_def *expected_type = vlist_get(func->args, i)->type_spec;
        type_def *actual_type = explist_get(args_list, i)->result_type;
        if (type_def_cmp(expected_type, actual_type) != 0)
        {
            char *expected_type_name = type_def_name(pm, expected_type);
            char *actual_type_name = type_def_name(pm, actual_type);
            efprintf(stderr, "Error at line %zu: argument %d of function %s is of type %s, but it expects type %s\n", args->line, i, func->name, actual_type_name, expected_type_name);
            str_free(expected_type_name);
            str_free(actual_type_name);
            explist_free(args_list);
            return exp_new_invalid();
        }
    }

    return exp_new_func_call(type_def_cpy(func->return_type), NULL, str_copy(func->name), args_list);
}

exp *exp_array_semantic(program_manager *pm, ASTNode *exp1, ASTNode *exp2)
{
    bool error_node = false;
    exp *array_exp = exp_semantic(pm, exp1);
    exp *index_exp = exp_semantic(pm, exp2);

    // check if the variable is an array
    if (!array_exp->result_type->is_array)
    {
        efprintf(stderr, "Error at line %zu: expression is not an array\n", exp1->line);
        error_node = true;
    }
    // check if the index is an integer
    if (!(type_def_is_primitive(index_exp->result_type) && index_exp->result_type->type_id == TYPE_INT))
    {
        efprintf(stderr, "Error at line %zu: index of array is not int\n", exp2->line);
        error_node = true;
    }

    if (error_node)
    {
        exp_free(array_exp);
        exp_free(index_exp);
        return exp_new_invalid();
    }

    return exp_new_array_access(type_def_cpy(array_exp->result_type->array_type), array_exp, index_exp);
}

/**
 * @brief Perform semantic analysis for a struct member expression: struct.member
 * @param pm The program manager to manage scopes and symbols.
 * @param struct_exp_node The AST node representing the struct expression.
 * @param id_node The name of the struct member.
 */
exp *exp_struct_semantic(program_manager *pm, ASTNode *struct_exp_node, ASTNode *id_node)
{
    // check if exp node itself is a struct
    exp *struct_exp = exp_semantic(pm, struct_exp_node);
    type_def *struct_type = struct_exp->result_type;
    if (!struct_type->is_struct)
    {
        efprintf(stderr, "Error at line %zu: expression is not a struct\n", struct_exp_node->line);
        exp_free(struct_exp);
        return exp_new_invalid();
    }

    // get the struct definition
    struct_def *struct_def = program_manager_get_struct_by_id(pm, struct_type->type_id);
    if (struct_def == NULL)
    {
        efprintf(stderr, "Error at line %zu: struct does not exist, this shouldn't happen!\n", struct_exp_node->line);
        exp_free(struct_exp);
        return exp_new_invalid();
    }

    // check if the struct has the field
    char *field_name = id_node->text;
    field_def *field = struct_def_get_field(struct_def, field_name);
    if (field == NULL)
    {
        char *struct_name = type_def_name(pm, struct_type);
        efprintf(stderr, "Error at line %zu: struct %s does not have field %s\n", id_node->line, struct_name, field_name);
        str_free(struct_name);
        exp_free(struct_exp);
        return exp_new_invalid();
    }

    return exp_new_struct_access(type_def_cpy(field->type_spec), struct_exp, str_copy(field_name));
}

exp *exp_struct_func_semantic(program_manager *pm, ASTNode *struct_exp_node, ASTNode *id_node, ASTNode *args_node)
{
    // check if exp node itself is a struct
    exp *struct_exp = exp_semantic(pm, struct_exp_node);
    type_def *struct_type = struct_exp->result_type;
    if (!struct_type->is_struct)
    {
        efprintf(stderr, "Error at line %zu: expression is not a struct\n", struct_exp_node->line);
        exp_free(struct_exp);
        return exp_new_invalid();
    }

    // get the struct definition
    struct_def *struct_def = program_manager_get_struct_by_id(pm, struct_type->type_id);
    if (struct_def == NULL)
    {
        efprintf(stderr, "Error at line %zu: struct does not exist, this shouldn't happen!\n", struct_exp_node->line);
        exp_free(struct_exp);
        return exp_new_invalid();
    }

    // check if the struct has the function
    char *func_name = id_node->text;
    func_def *func = struct_def_get_func(struct_def, func_name);
    if (func == NULL)
    {
        char *struct_name = type_def_name(pm, struct_type);
        efprintf(stderr, "Error at line %zu: struct %s does not have function %s\n", id_node->line, struct_name, func_name);
        str_free(struct_name);
        exp_free(struct_exp);
        return exp_new_invalid();
    }

    // check if the function is called with the correct number of arguments
    if (func->args->count == 0 && args_node != NULL)
    {
        efprintf(stderr, "Error at line %zu: function %s is called with arguments, but it expects no arguments\n", id_node->line, func_name);
        exp_free(struct_exp);
        return exp_new_invalid();
    }
    else if (func->args->count != 0 && args_node == NULL)
    {
        efprintf(stderr, "Error at line %zu: function %s is called with no arguments, but it expects arguments\n", id_node->line, func_name);
        exp_free(struct_exp);
        return exp_new_invalid();
    }
    else if (func->args->count == 0 && args_node == NULL)
    {
        return exp_new_func_call(type_def_cpy(func->return_type), struct_exp, str_copy(func->name), NULL);
    }


    explist *args_list = args_semantic(pm, args_node);
    size_t func_args_cnt = vlist_count(func->args);

    if (func_args_cnt != explist_count(args_list))
    {
        efprintf(stderr, "Error at line %zu: function %s is called with %zu arguments, but it expects %zu arguments\n", id_node->line, func_name, explist_count(args_list), func_args_cnt);
        exp_free(struct_exp);
        explist_free(args_list);
        return exp_new_invalid();
    }

    // check if the arguments are of the correct type
    for (int i = 0; i < func_args_cnt; i++)
    {
        type_def *expected_type = vlist_get(func->args, i)->type_spec;
        type_def *actual_type = explist_get(args_list, i)->result_type;
        if (type_def_cmp(expected_type, actual_type) != 0)
        {
            char *expected_type_name = type_def_name(pm, expected_type);
            char *actual_type_name = type_def_name(pm, actual_type);
            efprintf(stderr, "Error at line %zu: argument %d of function %s is of type %s, but it expects type %s\n", args_node->line, i, func->name, actual_type_name, expected_type_name);
            str_free(expected_type_name);
            str_free(actual_type_name);
            exp_free(struct_exp);
            explist_free(args_list);
            return exp_new_invalid();
        }
    }

    return exp_new_func_call(type_def_cpy(func->return_type), struct_exp, str_copy(func->name), args_list);
}

exp *exp_primitive_semantic(program_manager *pm, ASTNode *node)
{
    if (strcmp(node->nodeType, "INT") == 0)
    {
        int val = atoi(node->text);
        return exp_new_literal_int(val);
    }
    else if (strcmp(node->nodeType, "FLOAT") == 0)
    {
        float val = atof(node->text);
        return exp_new_literal_float(val);
    }
    else if (strcmp(node->nodeType, "CHAR") == 0)
    {
        char val = node->text[0];
        return exp_new_literal_char(val);
    }
    else
    {
        efprintf(stderr, "Error at line %zu: invalid primitive expression\n", node->line);
        return exp_new_invalid();
    }
}