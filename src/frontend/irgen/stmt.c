#include "stmt.h"

void stmt_ir_gen(stmt *s, IRContext *ctx)
{
    switch (s->stmt_type)
    {
    case STMT_INVALID:
        break;
    case STMT_EXP:
    {
        exp_ir_gen(s->exp.exp, ctx);
        break;
    }
    case STMT_COMP:
    {
        compst_ir_gen(s->comp.stmts, ctx);
        break;
    }
    case STMT_RETURN:
    {
        char *exp = exp_ir_gen(s->return_.exp, ctx);
        if (s->return_.exp->exp_type == EXP_LITERAL || s->return_.exp->exp_type == EXP_BI_OP || s->return_.exp->exp_type == EXP_UNARY_OP || s->return_.exp->exp_type == EXP_FUNC_CALL)
        {
            ir_context_append(ctx, "  store %s %s, %s* %%retval\n", map_type_to_llvm(s->return_.exp->result_type, ctx->pm), exp, map_type_to_llvm(s->return_.exp->result_type, ctx->pm));
            if (ctx->last_op != 1)
            {
                ir_context_append(ctx, "  br label %%RETURN\n");
                ctx->last_op = 1;
            }
            break;
        }
        char *retval = ir_context_new_temp(ctx);
        // load value from exp*
        char *type = map_type_to_llvm(s->return_.exp->result_type, ctx->pm);
        ir_context_append(ctx, "  %s = load %s, %s* %s\n", retval, type, type, exp);
        ir_context_append(ctx, "  store %s %s, %s* %%retval\n", map_type_to_llvm(s->return_.exp->result_type, ctx->pm), retval, map_type_to_llvm(s->return_.exp->result_type, ctx->pm));
        // ir_context_append(ctx, "  ret %s %s\n", map_type_to_llvm(s->return_.exp->result_type, ctx->pm), exp);
        if (ctx->last_op != 1)
        {
            ir_context_append(ctx, "  br label %%RETURN\n");
            ctx->last_op = 1;
        }
        break;
    }
    case STMT_IF:
    {
        char *if_label = ir_context_new_label(ctx, "IF");
        char *end_label = ir_context_new_label(ctx, "END");
        char *predicate_tmp = exp_ir_gen(s->if_.predicate, ctx);
        char *else_label = NULL;
        bool use_end_label = false;
        if (s->if_.else_stmt)
        {
            else_label = ir_context_new_label(ctx, "ELSE");
            if (ctx->last_op != 1)
            {
                ir_context_append(ctx, "  br i1 %s, label %%%s, label %%%s\n", predicate_tmp, if_label, else_label);
                ctx->last_op = 1;
            }
        }
        else
        {
            if (ctx->last_op != 1)
            {
                ir_context_append(ctx, "  br i1 %s, label %%%s, label %%%s\n", predicate_tmp, if_label, end_label);
                use_end_label = true;
                ctx->last_op = 1;
            }
        }
        ir_context_append(ctx, "%s:\n", if_label);
        stmt_ir_gen(s->if_.if_stmt, ctx);
        if (ctx->last_op != 1)
        {
            ir_context_append(ctx, "  br label %%%s\n", end_label);
            use_end_label = true;
            ctx->last_op = 1;
        }
        if (s->if_.else_stmt)
        {
            ir_context_append(ctx, "%s:\n", else_label);
            stmt_ir_gen(s->if_.else_stmt, ctx);
            free(else_label);
        }
        if (use_end_label)
        {
            ir_context_append(ctx, "  br label %%%s\n", end_label);
            ir_context_append(ctx, "%s:\n", end_label);
            char *nop = ir_context_new_temp(ctx);
            ir_context_append(ctx, "  %s = add i32 0, 0\n", nop);
        }

        free(predicate_tmp);
        free(if_label);
        free(end_label);
        break;
    }
    case STMT_WHILE:
    {
        char *while_label = ir_context_new_label(ctx, "WHILE");
        char *execute_label = ir_context_new_label(ctx, "EXECUTE");
        char *exit_label = ir_context_new_label(ctx, "EXIT");
        bool use_exit_label = false;
        if (ctx->last_op != 1)
        {
            ir_context_append(ctx, "  br label %%%s\n", while_label);
            ctx->last_op = 1;
        }
        ir_context_append(ctx, "%s:\n", while_label);
        char *predicate_tmp = exp_ir_gen(s->while_.predicate, ctx);
        if (ctx->last_op != 1)
        {
            ir_context_append(ctx, "  br i1 %s, label %%%s, label %%%s\n", predicate_tmp, execute_label, exit_label);
            use_exit_label = true;
            ctx->last_op = 1;
        }
        ir_context_append(ctx, "%s:\n", execute_label);
        stmt_ir_gen(s->while_.stmt, ctx);
        if (ctx->last_op != 1)
        {
            ir_context_append(ctx, "  br label %%%s\n", while_label);
            ctx->last_op = 1;
        }
        if (use_exit_label)
        {
            ir_context_append(ctx, "  br label %%%s\n", exit_label);
            ir_context_append(ctx, "%s:\n", exit_label);
            char *nop = ir_context_new_temp(ctx);
            ir_context_append(ctx, "  %s = add i32 0, 0\n", nop);
        }
        free(while_label);
        free(execute_label);
        free(exit_label);
        free(predicate_tmp);
        break;
    }
    }
}

void compst_ir_gen(struct stmtlist *stmts, IRContext *ctx)
{
    stmtlist_ir_gen(stmts, ctx);
}

void stmtlist_ir_gen(struct stmtlist *stmts, IRContext *ctx)
{
    for (int i = 0; i < stmtlist_count(stmts); i++)
    {
        stmt_ir_gen(stmtlist_get(stmts, i), ctx);
    }
}
