#include "exp.h"
#include "../../utils/util.h"
#include <stdarg.h>

exp *exp_new_bi_op(type_def *result_type, exp_bi_op_enum op, exp *lhs, exp *rhs) {
    exp *e = new(exp);
    e->result_type = result_type;
    e->exp_type = EXP_BI_OP;
    e->bi_op.op = op;
    e->bi_op.lhs = lhs;
    e->bi_op.rhs = rhs;
    return e;
}

exp *exp_new_unary_op(type_def *result_type, exp_unary_op_enum op, exp *operand) {
    exp *e = new(exp);
    e->result_type = result_type;
    e->exp_type = EXP_UNARY_OP;
    e->unary_op.op = op;
    e->unary_op.operand = operand;
    return e;
}

exp *exp_new_func_call(type_def *result_type, exp *lhs_exp, char *name, int num_args, ...) {
    exp *e = new(exp);
    e->result_type = result_type;
    e->exp_type = EXP_FUNC_CALL;
    e->func.lhs_exp = lhs_exp;
    e->func.name = name;
    e->func.num_args = num_args;
    e->func.arg_exps = new_n(exp *, num_args);
    va_list args;
    va_start(args, num_args);
    for (int i = 0; i < num_args; i++) {
        e->func.arg_exps[i] = va_arg(args, exp *);
    }
    va_end(args);
    return e;
}

exp *exp_new_array_access(type_def *result_type, exp *array_exp, exp *index_exp) {
    exp *e = new(exp);
    e->result_type = result_type;
    e->exp_type = EXP_ARRAY_ACCESS;
    e->array.array_exp = array_exp;
    e->array.index_exp = index_exp;
    return e;
}

exp *exp_new_struct_access(type_def *result_type, exp *lhs_exp, char *field_name) {
    exp *e = new(exp);
    e->result_type = result_type;
    e->exp_type = EXP_STRUCT_ACCESS;
    e->struct_access.lhs_exp = lhs_exp;
    e->struct_access.field_name = field_name;
    return e;
}

exp *exp_new_literal_int(type_id type, int int_val) {
    exp *e = new(exp);
    type_def *type_def = type_def_new_primitive(type);
    e->result_type = type_def;
    e->exp_type = EXP_LITERAL;
    e->literal.type = type;
    e->literal.int_val = int_val;
    return e;
}

exp *exp_new_literal_float(type_id type, float float_val) {
    exp *e = new(exp);
    type_def *type_def = type_def_new_primitive(type);
    e->result_type = type_def;
    e->exp_type = EXP_LITERAL;
    e->literal.type = type;
    e->literal.float_val = float_val;
    return e;
}

exp *exp_new_literal_char(type_id type, char char_val) {
    exp *e = new(exp);
    type_def *type_def = type_def_new_primitive(type);
    e->result_type = type_def;
    e->exp_type = EXP_LITERAL;
    e->literal.type = type;
    e->literal.char_val = char_val;
    return e;
}

void exp_free(exp *e) {
    switch (e->exp_type) {
        case EXP_BI_OP:
            exp_free(e->bi_op.lhs);
            exp_free(e->bi_op.rhs);
            break;
        case EXP_UNARY_OP:
            exp_free(e->unary_op.operand);
            break;
        case EXP_FUNC_CALL:
            free(e->func.name);
            for (int i = 0; i < e->func.num_args; i++) {
                exp_free(e->func.arg_exps[i]);
            }
            free(e->func.arg_exps);
            break;
        case EXP_ARRAY_ACCESS:
            exp_free(e->array.array_exp);
            exp_free(e->array.index_exp);
            break;
        case EXP_STRUCT_ACCESS:
            if (e->struct_access.lhs_exp != NULL) {
                exp_free(e->struct_access.lhs_exp);
            }
            free(e->struct_access.field_name);
            break;
        case EXP_LITERAL:
            break;
    }
    type_def_free(e->result_type);
    free(e);
}

exp *exp_cpy(exp *e) {
    exp *cpy = new(exp);
    cpy->result_type = type_def_cpy(e->result_type);
    cpy->exp_type = e->exp_type;
    switch (e->exp_type) {
        case EXP_BI_OP:
            cpy->bi_op.op = e->bi_op.op;
            cpy->bi_op.lhs = exp_cpy(e->bi_op.lhs);
            cpy->bi_op.rhs = exp_cpy(e->bi_op.rhs);
            break;
        case EXP_UNARY_OP:
            cpy->unary_op.op = e->unary_op.op;
            cpy->unary_op.operand = exp_cpy(e->unary_op.operand);
            break;
        case EXP_FUNC_CALL:
            cpy->func.name = str_copy(e->func.name);
            cpy->func.num_args = e->func.num_args;
            cpy->func.arg_exps = new_n(exp *, e->func.num_args);
            for (int i = 0; i < cpy->func.num_args; i++) {
                cpy->func.arg_exps[i] = exp_cpy(e->func.arg_exps[i]);
            }
            break;
        case EXP_ARRAY_ACCESS:
            cpy->array.array_exp = exp_cpy(e->array.array_exp);
            cpy->array.index_exp = exp_cpy(e->array.index_exp);
            break;
        case EXP_STRUCT_ACCESS:
            cpy->struct_access.lhs_exp = exp_cpy(e->struct_access.lhs_exp);
            cpy->struct_access.field_name = str_copy(e->struct_access.field_name);
            break;
        case EXP_LITERAL:
            cpy->literal.type = e->literal.type;
            switch (e->literal.type) {
                case TYPE_INT:
                    cpy->literal.int_val = e->literal.int_val;
                    break;
                case TYPE_FLOAT:
                    cpy->literal.float_val = e->literal.float_val;
                    break;
                case TYPE_CHAR:
                    cpy->literal.char_val = e->literal.char_val;
                    break;
            }
            break;
    }
    return cpy;
}