#ifndef __EXP_H__
#define __EXP_H__

typedef struct explist explist;

#include "../type.h"
#include "op.h"
#include "func.h"
#include "array.h"
#include "struct.h"
#include "literal.h"
#include "id.h"

typedef enum exp_type {
    EXP_INVALID = 0,
    EXP_BI_OP,
    EXP_UNARY_OP,
    EXP_FUNC_CALL,
    EXP_ARRAY_ACCESS,
    EXP_STRUCT_ACCESS,
    EXP_LITERAL,
    EXP_ID,
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
        exp_id id;
    };
} exp;

exp *exp_new_invalid();

exp *exp_new_bi_op(type_def *result_type, exp_bi_op_enum op, exp *lhs, exp *rhs);

exp *exp_new_unary_op(type_def *result_type, exp_unary_op_enum op, exp *operand);

exp *exp_new_func_call(type_def *result_type, exp *lhs_exp, char *name, explist *exp_args);

exp *exp_new_array_access(type_def *result_type, exp *array_exp, exp *index_exp);

exp *exp_new_struct_access(type_def *result_type, exp *lhs_exp, char *field_name);

exp *exp_new_literal_int(int int_val);

exp *exp_new_literal_float(float float_val);

exp *exp_new_literal_char(char char_val);

exp *exp_new_id(type_def *result_type, char *name);

void exp_free(exp *e);

exp *exp_cpy(exp *e);

#define SNAME explist
#define PFX explist
#define V exp*
#include <../collections/list_h.h>

extern struct explist_fval explist_fvals;

#endif