#ifndef __EXP_H__
#define __EXP_H__

#include "../type.h"
#include "op.h"
#include "func.h"
#include "array.h"
#include "struct.h"
#include "literal.h"

typedef enum exp_type {
    EXP_BI_OP,
    EXP_UNARY_OP,
    EXP_FUNC_CALL,
    EXP_ARRAY_ACCESS,
    EXP_STRUCT_ACCESS,
    EXP_LITERAL,
} exp_type;

typedef struct exp {
    type_def *result_type;
    exp_type exp_type;
    union {
        exp_bi_op bi_op;
        exp_unary_op unary_op;
        exp_func func;
        exp_array array;
        exp_struct_access struct_access;
        exp_literal literal;
    };
} exp;

exp *exp_new_bi_op(type_def *result_type, exp_bi_op_enum op, exp *lhs, exp *rhs);

exp *exp_new_unary_op(type_def *result_type, exp_unary_op_enum op, exp *operand);

exp *exp_new_func_call(type_def *result_type, exp *lhs_exp, char *name, int num_args, ...);

exp *exp_new_array_access(type_def *result_type, exp *array_exp, exp *index_exp);

exp *exp_new_struct_access(type_def *result_type, exp *lhs_exp, char *field_name);

exp *exp_new_literal_int(type_id type, int int_val);

exp *exp_new_literal_float(type_id type, float float_val);

exp *exp_new_literal_char(type_id type, char char_val);

void exp_free(exp *e);

exp *exp_cpy(exp *e);

#endif