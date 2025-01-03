#include "exp.h"
#include "../../utils/util.h"
#include <stdarg.h>
#include "../../irgen/ir_context.h"

exp *exp_new_invalid()
{
    exp *e = new (exp);
    e->result_type = type_def_new_primitive(TYPE_VOID);
    e->exp_type = EXP_INVALID;
    return e;
}

exp *exp_new_bi_op(type_def *result_type, exp_bi_op_enum op, exp *lhs, exp *rhs)
{
    exp *e = new (exp);
    e->result_type = result_type;
    e->exp_type = EXP_BI_OP;
    e->bi_op.op = op;
    e->bi_op.lhs = lhs;
    e->bi_op.rhs = rhs;
    return e;
}

exp *exp_new_unary_op(type_def *result_type, exp_unary_op_enum op, exp *operand)
{
    exp *e = new (exp);
    e->result_type = result_type;
    e->exp_type = EXP_UNARY_OP;
    e->unary_op.op = op;
    e->unary_op.operand = operand;
    return e;
}

exp *exp_new_func_call(type_def *result_type, exp *lhs_exp, char *name, explist *exp_args)
{
    exp *e = new (exp);
    e->result_type = result_type;
    e->exp_type = EXP_FUNC_CALL;
    e->func.lhs_exp = lhs_exp;
    e->func.name = name;
    e->func.arg_exps = exp_args;
    return e;
}

exp *exp_new_array_access(type_def *result_type, exp *array_exp, exp *index_exp)
{
    exp *e = new (exp);
    e->result_type = result_type;
    e->exp_type = EXP_ARRAY_ACCESS;
    e->array.array_exp = array_exp;
    e->array.index_exp = index_exp;
    return e;
}

exp *exp_new_struct_access(type_def *result_type, exp *lhs_exp, char *field_name)
{
    exp *e = new (exp);
    e->result_type = result_type;
    e->exp_type = EXP_STRUCT_ACCESS;
    e->struct_access.lhs_exp = lhs_exp;
    e->struct_access.field_name = field_name;
    return e;
}

exp *exp_new_literal_int(int int_val)
{
    exp *e = new (exp);
    type_def *type_def = type_def_new_primitive(TYPE_INT);
    e->result_type = type_def;
    e->exp_type = EXP_LITERAL;
    e->literal.type = TYPE_INT;
    e->literal.int_val = int_val;
    return e;
}

exp *exp_new_literal_float(float float_val)
{
    exp *e = new (exp);
    type_def *type_def = type_def_new_primitive(TYPE_FLOAT);
    e->result_type = type_def;
    e->exp_type = EXP_LITERAL;
    e->literal.type = TYPE_FLOAT;
    e->literal.float_val = float_val;
    return e;
}

exp *exp_new_literal_char(char char_val)
{
    exp *e = new (exp);
    type_def *type_def = type_def_new_primitive(TYPE_CHAR);
    e->result_type = type_def;
    e->exp_type = EXP_LITERAL;
    e->literal.type = TYPE_CHAR;
    e->literal.char_val = char_val;
    return e;
}

exp *exp_new_id(type_def *result_type, char *name)
{
    exp *e = new (exp);
    e->result_type = result_type;
    e->exp_type = EXP_ID;
    e->id.name = name;
    return e;
}

void exp_free(exp *e)
{
    switch (e->exp_type)
    {
    case EXP_BI_OP:
        exp_free(e->bi_op.lhs);
        exp_free(e->bi_op.rhs);
        break;
    case EXP_UNARY_OP:
        exp_free(e->unary_op.operand);
        break;
    case EXP_FUNC_CALL:
        if (e->func.lhs_exp != NULL)
        {
            exp_free(e->func.lhs_exp);
        }
        str_free(e->func.name);
        explist_free(e->func.arg_exps);
        break;
    case EXP_ARRAY_ACCESS:
        exp_free(e->array.array_exp);
        exp_free(e->array.index_exp);
        break;
    case EXP_STRUCT_ACCESS:
        exp_free(e->struct_access.lhs_exp);
        str_free(e->struct_access.field_name);
        break;
    case EXP_LITERAL:
        break;
    case EXP_ID:
        str_free(e->id.name);
        break;
    case EXP_INVALID:
        break;
    }
    type_def_free(e->result_type);
    free(e);
}

exp *exp_cpy(exp *e)
{
    exp *cpy = new (exp);
    cpy->result_type = type_def_cpy(e->result_type);
    cpy->exp_type = e->exp_type;
    switch (e->exp_type)
    {
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
        if (e->func.lhs_exp != NULL)
        {
            cpy->func.lhs_exp = exp_cpy(e->func.lhs_exp);
        }
        cpy->func.name = str_copy(e->func.name);
        cpy->func.arg_exps = explist_copy_of(e->func.arg_exps);
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
        switch (e->literal.type)
        {
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
    case EXP_ID:
        cpy->id.name = str_copy(e->id.name);
        break;
    case EXP_INVALID:
        break;
    }
    return cpy;
}

char *exp_ir_gen(exp *e, IRContext *ctx)
{
    switch (e->exp_type)
    {
    case EXP_BI_OP:
        return exp_bi_op_ir_gen(e, ctx);
    case EXP_UNARY_OP:
        return exp_unary_op_ir_gen(e, ctx);
    case EXP_FUNC_CALL:
        return exp_func_call_ir_gen(e, ctx);
    case EXP_ARRAY_ACCESS:
        return exp_array_access_ir_gen(e, ctx);
    case EXP_STRUCT_ACCESS:
        return exp_struct_access_ir_gen(e, ctx);
    case EXP_LITERAL:
        return exp_literal_ir_gen(e, ctx);
    case EXP_ID:
        return exp_id_ir_gen(e, ctx);
    case EXP_INVALID:
        return NULL;
    }
    return NULL;
}

static const char *get_icmp_predicate(exp_bi_op_enum op)
{
    switch (op)
    {
    case BI_OP_EQUAL:
        return "eq";
    case BI_OP_NOT_EQUAL:
        return "ne";
    case BI_OP_LESS_THAN:
        return "slt";
    case BI_OP_LESS_EQUAL:
        return "sle";
    case BI_OP_GREATER_THAN:
        return "sgt";
    case BI_OP_GREATER_EQUAL:
        return "sge";
    default:
        fprintf(stderr, "Invalid integer comparison operator\n");
        return "eq";
    }
}

static const char *get_fcmp_predicate(exp_bi_op_enum op)
{
    switch (op)
    {
    case BI_OP_EQUAL:
        return "oeq";
    case BI_OP_NOT_EQUAL:
        return "one";
    case BI_OP_LESS_THAN:
        return "olt";
    case BI_OP_LESS_EQUAL:
        return "ole";
    case BI_OP_GREATER_THAN:
        return "ogt";
    case BI_OP_GREATER_EQUAL:
        return "oge";
    default:
        fprintf(stderr, "Invalid flop comparison operator\n");
        return "oeq";
    }
}

char *exp_bi_op_ir_gen(exp *e, IRContext *ctx)
{
    char *lhs_tmp = exp_ir_gen(e->bi_op.lhs, ctx);
    char *rhs_tmp = exp_ir_gen(e->bi_op.rhs, ctx);
    char *ir = NULL;

    char *tmp = ir_context_new_temp(ctx);
    char *type = map_type_to_llvm(e->result_type);

    switch (e->bi_op.op)
    {
    case BI_OP_ASSIGN:
        // TODO: fetch address from ptr and store value
        /*
         * 赋值通常需要：
         *   - 左值是一个指针(比如变量指针)
         *   - 把右值 store 到左值所在的内存
         *   - 最终结果常常是右值本身(或被赋予的值)
         *
         * 示例(假设 lhs_tmp 是指针):
         *   store i32 %rhs, i32* %lhs
         *   ; result_var = %rhs (可选)
         */
        ir_context_append(ctx,
                          "  store %s %s, %s* %s\n",
                          type, rhs_tmp,
                          type, lhs_tmp);
        /* 赋值表达式的值, 大多数情况下 C/C++ 语义是 “被赋值后的值” */
        // 这里简单返回右值
        free(tmp); // 如果我们不需要再分配新的临时寄存器，就可以释放或不生成
        return rhs_tmp;

    case BI_OP_PLUS:
        ir_context_append(ctx, "  %s = add %s %s, %s\n", tmp, type, lhs_tmp, rhs_tmp);
        break;
    case BI_OP_MINUS:
        ir_context_append(ctx, "  %s = sub %s %s, %s\n", tmp, type, lhs_tmp, rhs_tmp);
        break;
    case BI_OP_MUL:
        ir_context_append(ctx, "  %s = mul %s %s, %s\n", tmp, type, lhs_tmp, rhs_tmp);
        break;
    case BI_OP_DIV:
        if (e->result_type->type_id == TYPE_INT)
        {
            ir_context_append(ctx, "  %s = sdiv %s %s, %s\n", tmp, type, lhs_tmp, rhs_tmp);
        }
        else if (e->result_type->type_id == TYPE_FLOAT)
        {
            ir_context_append(ctx, "  %s = fdiv %s %s, %s\n", tmp, type, lhs_tmp, rhs_tmp);
        }
        break;
    case BI_OP_AND:
        ir_context_append(ctx, "  %s = and %s %s, %s\n", tmp, type, lhs_tmp, rhs_tmp);
        break;
    case BI_OP_OR:
        ir_context_append(ctx, "  %s = or %s %s, %s\n", tmp, type, lhs_tmp, rhs_tmp);
        break;

    case BI_OP_EQUAL:
    case BI_OP_NOT_EQUAL:
    case BI_OP_LESS_THAN:
    case BI_OP_LESS_EQUAL:
    case BI_OP_GREATER_THAN:
    case BI_OP_GREATER_EQUAL:
    {
        if (e->result_type->type_id == TYPE_INT ||
            e->result_type->type_id == TYPE_CHAR)
        {
            /* int/char：icmp */
            const char *predicate = get_icmp_predicate(e->bi_op.op);
            ir_context_append(ctx, "  %s = icmp %s %s %s, %s\n", tmp, predicate, type, lhs_tmp, rhs_tmp);
            // @TODO convert icmp result to i32

        }
        else if (e->result_type->type_id == TYPE_FLOAT)
        {
            /* float：fcmp */
            const char *predicate = get_fcmp_predicate(e->bi_op.op);
            ir_context_append(ctx, "  %s = fcmp %s %s %s, %s\n", tmp, predicate, type, lhs_tmp, rhs_tmp);
        }
        break;
    }
    }
    free(lhs_tmp);
    free(rhs_tmp);
    return tmp;
}

char *exp_unary_op_ir_gen(exp *e, IRContext *ctx)
{
    char *operand_tmp = exp_ir_gen(e->unary_op.operand, ctx);
    char *tmp = ir_context_new_temp(ctx);
    char *type = map_type_to_llvm(e->result_type);

    switch (e->unary_op.op)
    {
    case UNARY_OP_MINUS:
        if (e->result_type->type_id == TYPE_INT)
        {
            ir_context_append(ctx, "  %s = sub %s 0, %s\n", tmp, type, operand_tmp);
        }
        else if (e->result_type->type_id == TYPE_FLOAT)
        {
            ir_context_append(ctx, "  %s = fsub %s 0.0, %s\n", tmp, type, operand_tmp);
        }
        break;
    case UNARY_OP_NOT:
        ir_context_append(ctx, "  %s = xor %s %s, 1\n", tmp, type, operand_tmp);
        break;
    }
    free(operand_tmp);
    return tmp;
}