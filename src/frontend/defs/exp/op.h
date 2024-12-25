#ifndef __EXP_OP_H__
#define __EXP_OP_H__

#include "exp.h"

typedef enum exp_bi_op_enum {
    BI_OP_INVALID = 0,
    BI_OP_ASSIGN,
    BI_OP_AND,
    BI_OP_OR,
    BI_OP_LESS_THAN,
    BI_OP_LESS_EQUAL,
    BI_OP_GREATER_THAN,
    BI_OP_GREATER_EQUAL,
    BI_OP_NOT_EQUAL,
    BI_OP_EQUAL,
    BI_OP_PLUS,
    BI_OP_MINUS,
    BI_OP_MUL,
    BI_OP_DIV,
} exp_bi_op_enum;

typedef enum exp_unary_op_enum {
    UNARY_OP_INVALID = 0,
    UNARY_OP_MINUS,
    UNARY_OP_PLUS,
    UNARY_OP_NOT,
} exp_unary_op_enum;

typedef struct exp_bi_op {
    exp_bi_op_enum op;
    exp *lhs;
    exp *rhs;
} exp_bi_op;

typedef struct exp_unary_op {
    exp_unary_op_enum op;
    exp *operand;
} exp_unary_op;

#endif