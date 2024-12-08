#include "exp.h"
#include "../../utils/util.h"
#include <stdlib.h>

type_def *exp_semantic(program_manager *pm, ASTNode *node);

type_def *exp_id_semantic(program_manager *pm, ASTNode *node)
{
    // check usage before declaration
    if (!program_manager_get_field(pm, node->text))
    {
        fprintf(stderr, "Error at line %zu: variable %s not declared\n", node->line, node->text);
    }
}

type_def *exp_bi_op_semantic(program_manager *pm, ASTNode *left, char *op, ASTNode *right)
{
    // check usage before declaration
    if (!strcmp(left->nodeType, "ID"))
    {
        if (!program_manager_get_field(pm, left->text))
        {
            fprintf(stderr, "Error at line %zu: variable %s not declared\n", left->line, left->text);
        }
    }
    if (!strcmp(right->nodeType, "ID"))
    {
        if (!program_manager_get_field(pm, right->text))
        {
            fprintf(stderr, "Error at line %zu: variable %s not declared\n", right->line, right->text);
        }
    }

    // check type compatibility for binary operations
    if (strcmp(op, "="))
    {
        type_def *left_type = exp_semantic(pm, left);
        type_def *right_type = exp_semantic(pm, right);
        if (left_type && right_type)
        {
            // check type compatibility: arrays and structs are not supported
            if (left_type->is_array || left_type->is_struct || right_type->is_array || right_type->is_struct)
            {
                fprintf(stderr, "Error at line %zu: type mismatch, arrays and structs can neither be assigned to or assigned from\n", left->line);
            }

            if (left_type->type_id != right_type->type_id)
            {
                fprintf(stderr, "Error at line %zu: type mismatch, try to assign type '%s' to type '%s'\n", left->line, right->nodeType, left->nodeType);
            }
        }
    }
    else if (strcmp(op, "+") || strcmp(op, "-") || strcmp(op, "*"))
    {
        type_def *left_type = exp_semantic(pm, left);
        type_def *right_type = exp_semantic(pm, right);
        if (left_type->type_id != TYPE_INT && left_type->type_id != TYPE_FLOAT && left_type->type_id != TYPE_CHAR)
        {
            fprintf(stderr, "Error at line %zu: invalid type for operation %s on left expression\n", left->line, op);
        }
        if (right_type->type_id != TYPE_INT && right_type->type_id != TYPE_FLOAT && right_type->type_id, TYPE_CHAR)
        {
            fprintf(stderr, "Error at line %zu: invalid type for operation %s on right expression\n", right->line, op);
        }
        // CHAR + INT = CHAR, CHAR + FLOAT = FLOAT, CHAR + CHAR = CHAR, INT + CHAR = CHAR,INT + INT = INT, INT + FLOAT = FLOAT, FLOAT + FLOAT = FLOAT
        // TODO: Type promotion
    }
    else if (strcmp(op, "/"))
    {
        type_def *left_type = exp_semantic(pm, left);
        type_def *right_type = exp_semantic(pm, right);
        if (left_type->type_id!= TYPE_INT && left_type->type_id!=TYPE_FLOAT && left_type->type_id != TYPE_CHAR)
        {
            fprintf(stderr, "Error at line %zu: invalid type for operation %s on left expression\n", left->line, op);
        }
        if (right_type->type_id!=TYPE_INT && right_type->type_id!=TYPE_FLOAT && right_type->type_id!= TYPE_CHAR)
        {
            fprintf(stderr, "Error at line %zu: invalid type for operation %s on right expression\n", right->line, op);
        }
        // check divide-by-zero, such as 0, 0.0f, .0f, 0.0, .0, 0x0
        if (strcmp(right->text, "0") && strcmp(right->text, "0.0f") && strcmp(right->text, ".0f") && strcmp(right->text, "0.0") && strcmp(right->text, ".0") && strcmp(right->text, "0x0"))
        {
            fprintf(stderr, "Error at line %zu: divide by zero\n", right->line);
        }
        // CHAR / INT = CHAR, CHAR / FLOAT = FLOAT, CHAR / CHAR = CHAR, INT / CHAR = CHAR,INT / INT = INT, INT / FLOAT = FLOAT, FLOAT / FLOAT = FLOAT
    }

    type_def *exp_unary_op_semantic(program_manager * pm, char *op, ASTNode *child);

    type_def *exp_assign_semantic(program_manager * pm, ASTNode * left, ASTNode * right);

    type_def *exp_func_semantic(program_manager * pm, ASTNode * func_id, ASTNode * args);

    type_def *exp_array_semantic(program_manager * pm, ASTNode * exp1, ASTNode * exp2);

    type_def *exp_struct_semantic(program_manager * pm, ASTNode * exp, char *id);

    type_def *exp_struct_func_semantic(program_manager * pm, ASTNode * exp, char *id, ASTNode *args);

    type_def *exp_primitive_semantic(program_manager * pm, char *type, char *text);
