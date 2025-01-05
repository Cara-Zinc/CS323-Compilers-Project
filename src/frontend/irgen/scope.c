#include "scope.h"
#include "stmt.h"
#include "../defs/field.h"

bool var_is_arg(field_def *var, varlist *args) {
    for(int i=0; i<varlist_count(args); i++) {
        field_def *arg = varlist_get(args, i);
        if(strcmp(arg->name, var->name) == 0) return true;
    }
    return false;
}

void func_ir_gen(func_def *f, IRContext *ctx) {
    ir_context_append(ctx, "define %s @%s(", map_type_to_llvm(f->return_type, ctx->pm), f->name);
    for(int i=0; i<varlist_count(f->args); i++) {
        field_def *arg = varlist_get(f->args, i);
        if(i > 0) ir_context_append(ctx, ", ");
        ir_context_append(ctx, "%s %%%s", map_type_to_llvm(arg->type_spec, ctx->pm), arg->name);
    }
    ir_context_append(ctx, ") {\n");
    ir_context_append(ctx, "entry:\n");
    // generate IR for all variables
    for(int i=0; i<vmap_count(f->scope->fields); i++) {
        field_def *var = vmap_get(f->scope->fields, i);
        if (var_is_arg(var, f->args)) continue;
        ir_context_append(ctx, "%%%s = alloca %s\n", var->name, map_type_to_llvm(var->type_spec, ctx->pm));
    }
    // generate IR for all statements
    stmtlist_ir_gen(f->stmts, ctx);
    ir_context_append(ctx, "}\n");
}

void struct_ir_gen(struct_def *sd, IRContext *ctx) {
    ir_context_append(ctx, "%%struct.%s = type {", sd->name);
    for(int i=0; i<varlist_count(sd->scope->fields); i++) {
        field_def *f = varlist_get(sd->scope->fields, i);
        if(i > 0) ir_context_append(ctx, ", ");
        ir_context_append(ctx, "%s", map_type_to_llvm(f->type_spec, ctx->pm));
    }
    ir_context_append(ctx, "}\n");
}

void scope_ir_gen(scope *s, IRContext *ctx) {
    // generate IR for all struct defs
    for(int i=0; i<smap_count(s->struct_defs); i++) {
        struct_def *sd = smap_get(s->struct_defs, i);
        struct_ir_gen(sd, ctx);
    }
    
    // generate IR for all fields
    // Please check!
    for(int i=0; i<vmap_count(s->fields); i++) {
        field_def *f = vmap_get(s->fields, i);
        ir_context_append(ctx, "%%%s = alloca %s\n", f->name, map_type_to_llvm(f->type_spec, ctx->pm));
    }

    // generate IR for all funcs
    for(int i=0; i<fmap_count(s->funcs); i++) {
        func_def *f = fmap_get(s->funcs, i);
        func_ir_gen(f, ctx);
    }

    // generate IR for all subscopes
    for(int i=0; i<sclist_count(s->subscopes); i++) {
        scope_ir_gen(sclist_get(s->subscopes, i), ctx);
    }
}