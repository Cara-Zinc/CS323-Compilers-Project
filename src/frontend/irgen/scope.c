#include "scope.h"
#include "stmt.h"
#include "../defs/field.h"
#include <cmc/utl/foreach.h>

bool var_is_arg(field_def *var, varlist *args)
{
    for (int i = 0; i < vlist_count(args); i++)
    {
        field_def *arg = vlist_get(args, i);
        if (strcmp(arg->name, var->name) == 0)
            return true;
    }
    return false;
}

void func_ir_gen(func_def *f, IRContext *ctx)
{
    ir_context_append(ctx, "define %s @%s(", map_type_to_llvm(f->return_type, ctx->pm), f->name);
    for (int i = 0; i < vlist_count(f->args); i++)
    {
        field_def *arg = vlist_get(f->args, i);
        if (i > 0)
            ir_context_append(ctx, ", ");
        ir_context_append(ctx, "%s* %%%s", map_type_to_llvm(arg->type_spec, ctx->pm), arg->name);
    }
    ir_context_append(ctx, ") {\n");
    ir_context_append(ctx, "entry:\n");
    // reserve a variable for return value
    if (f->return_type != TYPE_VOID)
        ir_context_append(ctx, "  %%retval = alloca %s\n", map_type_to_llvm(f->return_type, ctx->pm));

    // generate IR for all variables
    varmap *fields = f->scope->fields;
    CMC_FOREACH(vmap, varmap, iter, fields)
    {
        field_def *var = vmap_iter_value(&iter);
        if (var_is_arg(var, f->args))
            continue;
        ir_context_append(ctx, "  %%%s = alloca %s\n", var->name, map_type_to_llvm(var->type_spec, ctx->pm));
    }
    // generate IR for all statements
    stmtlist_ir_gen(f->stmts, ctx);

    // return statement
    ir_context_append(ctx, "RETURN:\n");
    if (f->return_type != TYPE_VOID)
    {
        ir_context_append(ctx, "  %%return_value = load %s, %s* %%retval\n", map_type_to_llvm(f->return_type, ctx->pm), map_type_to_llvm(f->return_type, ctx->pm), map_type_to_llvm(f->return_type, ctx->pm));
        ir_context_append(ctx, "  ret %s %%return_value\n", map_type_to_llvm(f->return_type, ctx->pm));
    }
    else
    {
        ir_context_append(ctx, "  ret void\n");
    }
    ir_context_append(ctx, "}\n");
}

void struct_ir_gen(struct_def *sd, IRContext *ctx)
{
    ir_context_append(ctx, "%%struct.%s = type {", sd->name);
    int i = 0;
    CMC_FOREACH(vmap, varmap, iter, sd->scope->fields) {
        field_def *f = vmap_iter_value(&iter);
        if (i > 0)
            ir_context_append(ctx, ", ");
        ir_context_append(ctx, "%s", map_type_to_llvm(f->type_spec, ctx->pm));
        i++;
    }
    ir_context_append(ctx, "}\n");
}

void scope_ir_gen(scope *s, IRContext *ctx)
{
    // generate IR for all struct defs
    CMC_FOREACH(smap, structmap, iter, s->struct_defs) {
        struct_def *sd = smap_iter_value(&iter);
        struct_ir_gen(sd, ctx);
    }

    // generate IR for all fields
    // Please check!
    CMC_FOREACH(vmap, varmap, iter, s->fields) {
        field_def *f = vmap_iter_value(&iter);
        ir_context_append(ctx, "%%%s = alloca %s\n", f->name, map_type_to_llvm(f->type_spec, ctx->pm));
    }

    // generate IR for all funcs
    CMC_FOREACH(fmap, funcmap, iter, s->funcs) {
        func_def *f = fmap_iter_value(&iter);
        func_ir_gen(f, ctx);
    }

    // generate IR for all subscopes
    for (int i = 0; i < sclist_count(s->subscopes); i++)
    {
        scope_ir_gen(sclist_get(s->subscopes, i), ctx);
    }
}