#include "exp.h"

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
        char *gep_tmp = exp_array_access_ir_gen(e, ctx);
        char *tmp = ir_context_new_temp(ctx);
        char *type = map_type_to_llvm(e->result_type, ctx->pm);
        ir_context_append(ctx, "  %s = load %s, ptr %s\n", tmp, type, gep_tmp);
        free(gep_tmp);
        return tmp;
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

    char *tmp = ir_context_new_temp(ctx);
    char *type = map_type_to_llvm(e->result_type, ctx->pm);

    switch (e->bi_op.op)
    {
    case BI_OP_INVALID:
        break;
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
        }
        else if (e->result_type->type_id == TYPE_FLOAT)
        {
            /* float：fcmp */
            const char *predicate = get_fcmp_predicate(e->bi_op.op);
            ir_context_append(ctx, "  %s = fcmp %s %s %s, %s\n", tmp, predicate, type, lhs_tmp, rhs_tmp);
        }
        // @TODO convert icmp result to i32
        char *tmp1 = ir_context_new_temp(ctx);
        ir_context_append(ctx, "  %s = zext i1 %s to i32\n", tmp1, tmp);
        free(tmp);
        return tmp1;
    }
    case BI_OP_ASSIGN:
        // TODO: fetch address from ptr and store value
        /*
         * 赋值通常需要：
         *   - 左值是一个指针(比如变量指针)
         *   - 把右值 store 到左值所在的内存
         *   - 最终结果常常是右值本身(或被赋予的值)
         *   - 如果左值指向数组或者结构体，他们本来就是指针，所以不需要额外处理
         *
         * 示例(假设 lhs_tmp 是指针):
         *   store i32 %rhs, i32* %lhs
         *   ; result_var = %rhs (可选)
         */
        char *lhs_tmp = NULL;
        if (e->bi_op.lhs->exp_type == EXP_ID)
        {
            lhs_tmp = malloc(strlen(e->bi_op.lhs->id.name) + 2);
            sprintf(lhs_tmp, "%%%s", e->bi_op.lhs->id.name);
        }
        else
        {
            lhs_tmp = exp_ir_gen(e->bi_op.lhs, ctx);
        }
        char *rhs_tmp = exp_ir_gen(e->bi_op.rhs, ctx);
        char *rhs_type = map_type_to_llvm(e->bi_op.rhs->result_type, ctx->pm);
        char *lhs_type = map_type_to_llvm(e->bi_op.lhs->result_type, ctx->pm);

        ir_context_append(ctx,
                          "  store %s %s, %s* %s\n",
                          rhs_type, rhs_tmp,
                          lhs_type, lhs_tmp);

        free(rhs_tmp);
        free(lhs_tmp);
        /* 赋值表达式的值, 大多数情况下 C/C++ 语义是 “被赋值后的值” */
        // 这里简单返回右值
        free(tmp); // 如果我们不需要再分配新的临时寄存器，就可以释放或不生成
        return NULL;
    }
    free(lhs_tmp);
    free(rhs_tmp);
    return tmp;
}

char *exp_unary_op_ir_gen(exp *e, IRContext *ctx)
{
    char *operand_tmp = exp_ir_gen(e->unary_op.operand, ctx);
    char *tmp = ir_context_new_temp(ctx);
    char *type = map_type_to_llvm(e->result_type, ctx->pm);

    switch (e->unary_op.op)
    {
    case UNARY_OP_INVALID:
        break;
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
        char *tmp1 = ir_context_new_temp(ctx);
        ir_context_append(ctx, "  %s = icmp eq %s %s, 0\n", tmp1, type, operand_tmp);
        ir_context_append(ctx, "  %s = zext i1 %s to i32\n", tmp, tmp1);
        free(tmp1);
        break;
    case UNARY_OP_PLUS:
        // do nothing, in structura '+' before number is just a placeholder
        break;
    }

    free(operand_tmp);
    return tmp;
}

char *concat_args(explist *args, IRContext *ctx)
{
    if (args == NULL)
    {
        char *empty = malloc(1);
        if (!empty)
        {
            fprintf(stderr, "Failed to allocate memory for empty arguments string.\n");
            exit(EXIT_FAILURE);
        }
        empty[0] = '\0';
        return empty;
    }

    size_t buffer_size = 256;
    char *args_str = malloc(buffer_size);
    if (!args_str)
    {
        fprintf(stderr, "Failed to allocate memory for arguments string.\n");
        exit(EXIT_FAILURE);
    }
    size_t current_length = 0;

    for (int i = 0; i < explist_count(args); i++)
    {
        exp *arg_exp = explist_get(args, i);
        if (!arg_exp)
        {
            fprintf(stderr, "Encountered NULL expression in argument list.\n");
            free(args_str);
            exit(EXIT_FAILURE);
        }

        char *arg_tmp = exp_ir_gen(arg_exp, ctx);
        if (!arg_tmp)
        {
            fprintf(stderr, "Failed to generate IR for function argument.\n");
            free(args_str);
            exit(EXIT_FAILURE);
        }

        const char *arg_type = map_type_to_llvm(arg_exp->result_type, ctx->pm);
        if (!arg_type)
        {
            fprintf(stderr, "Failed to map type to LLVM IR type string.\n");
            free(arg_tmp);
            free(args_str);
            exit(EXIT_FAILURE);
        }

        // 计算当前参数部分的长度："type %var, " -> strlen(type) + 1 + strlen(var) + 2
        size_t arg_part_length = strlen(arg_type) + 1 + strlen(arg_tmp) + 2;

        if (current_length + arg_part_length >= buffer_size)
        {
            buffer_size *= 2; // 扩展缓冲区大小
            char *new_buffer = realloc(args_str, buffer_size);
            if (!new_buffer)
            {
                fprintf(stderr, "Failed to realloc memory for arguments string.\n");
                free(arg_tmp);
                free(args_str);
                exit(EXIT_FAILURE);
            }
            args_str = new_buffer;
        }

        // 直接写入缓冲区
        memcpy(args_str + current_length, arg_type, strlen(arg_type));
        current_length += strlen(arg_type);
        args_str[current_length++] = ' ';
        memcpy(args_str + current_length, arg_tmp, strlen(arg_tmp));
        current_length += strlen(arg_tmp);
        memcpy(args_str + current_length, ", ", 2);
        current_length += 2;

        free(arg_tmp);
    }

    // 移除最后的 ", "
    if (current_length >= 2)
    {
        args_str[current_length - 2] = '\0';
    }
    else
    {
        args_str[0] = '\0';
    }

    return args_str;
}

char *exp_func_call_ir_gen(exp *e, IRContext *ctx)
{
    //@TODO: implement function call
    // example: for a C code: int foo(int a, int b, int c);
    // %1 = call i32 @foo(i32 %a, i32 %b, i32 %c)
    char *tmp = ir_context_new_temp(ctx);
    char *type = map_type_to_llvm(e->result_type, ctx->pm);

    //@TODO: get the function name from the symbol table
    char *func_name = e->func.name;
    explist *args = e->func.arg_exps;
    char *args_str = concat_args(args, ctx);
    ir_context_append(ctx, "  %s = call %s @%s(%s)\n", tmp, type, func_name, args_str);
    free(args_str);
    return tmp;
}

char *exp_array_access_ir_gen(exp *e, IRContext *ctx)
{
    char *array_ptr = exp_ir_gen(e->array.array_exp, ctx);
    char *array_type = map_type_to_llvm(e->array.array_exp->result_type, ctx->pm);
    char *index = exp_ir_gen(e->array.index_exp, ctx);
    char *gep_tmp = ir_context_new_temp(ctx);

    // <result> = getelementptr <type>, ptr <ptrval>, i32 0, i32 <index>
    ir_context_append(ctx, "  %s = getelementptr %s, ptr %s, i32 0, i32 %s\n", gep_tmp, array_type, array_ptr, index);
    return gep_tmp;
    // example: for a C code: s[1].Z.B[5][13]
    //  struct RT {
    // char A;
    // int B[10][20];
    // char C;
    // };
    // struct ST {
    // int X;
    // double Y;
    // struct RT Z;
    // };
    // int *foo(struct ST *s) {
    //     return &s[1].Z.B[5][13];
    // }
    // %arrayidx = getelementptr inbounds %struct.ST, ptr %s, i64 1, i32 2, i32 1, i64 5, i64 13
    // is equivalent to:
    //   %t1 = getelementptr %struct.ST, ptr %s, i32 1                  ; s[1], a pointer
    //   %t2 = getelementptr %struct.ST, ptr %t1, i32 0, i32 2          ; s[1].Z, a pointer
    //   %t3 = getelementptr %struct.RT, ptr %t2, i32 0, i32 1          ; s[1].Z.B, a pointer
    //   %t4 = getelementptr [10 x [20 x i32]], ptr %t3, i32 0, i32 5
    //   %t5 = getelementptr [20 x i32], ptr %t4, i32 0, i32 13
}

static int get_field_index(struct_def *s, char *field_name)
{
    varmap *fields = s->scope->fields;
    int field_idx = 0;
    CMC_FOREACH(vmap, varmap, iter, fields)
    {
        char *field = vmap_iter_value(&iter)->name;
        if (strcmp(field, field_name) == 0)
        {
            return field_idx;
        }
        field_idx++;
    }
    return -1;
}

char *exp_struct_access_ir_gen(exp *e, IRContext *ctx)
{
    char *lhs_tmp = exp_ir_gen(e->struct_access.lhs_exp, ctx);
    char *tmp = ir_context_new_temp(ctx);
    char *type = map_type_to_llvm(e->struct_access.lhs_exp->result_type, ctx->pm);
    struct_def *s = program_manager_get_struct_by_id(ctx->pm, e->struct_access.lhs_exp->result_type->type_id);
    // @TODO: find out the member's field index in the struct based on e->struct_access.field_name
    int field_idx = get_field_index(s, e->struct_access.field_name);
    // <result> = getelementptr <type>, ptr <ptrval>, i32 0, i32 <index>
    ir_context_append(ctx, "  %s = getelementptr %s, ptr %s, i64 0, i32 %d\n", tmp, type, lhs_tmp, field_idx);
    free(lhs_tmp);
    return tmp;
}

char *exp_literal_ir_gen(exp *e, IRContext *ctx)
{
    char *tmp = ir_context_new_temp(ctx);
    char *type = map_type_to_llvm(e->result_type, ctx->pm);
    switch (e->literal.type)
    {
    case TYPE_INT:
        ir_context_append(ctx, "  %s = add %s 0, %d\n", tmp, type, e->literal.int_val);
        break;
    case TYPE_FLOAT:
        ir_context_append(ctx, "  %s = fadd %s 0.0, %f\n", tmp, type, e->literal.float_val);
        break;
    case TYPE_CHAR:
        ir_context_append(ctx, "  %s = add %s 0, %d\n", tmp, type, e->literal.char_val);
        break;
    }
    return tmp;
}

char *exp_id_ir_gen(exp *e, IRContext *ctx)
{
    type_def *t = e->result_type;
    if (t->is_array || t->is_struct)
    {
        char *var = malloc(strlen(e->id.name) + 2);
        if (!var)
        {
            fprintf(stderr, "Failed to allocate memory for variable name.\n");
            exit(EXIT_FAILURE);
        }
        sprintf(var, "%%%s", e->id.name);
        return var;
    }
    else
    {
        char *tmp = ir_context_new_temp(ctx);
        char *type = map_type_to_llvm(e->result_type, ctx->pm);
        ir_context_append(ctx, "  %s = load %s, %s* %%%s\n", tmp, type, type, e->id.name);
        return tmp;
    }
}