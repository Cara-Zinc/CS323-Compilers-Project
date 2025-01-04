#include "scope.h"

void func_ir_gen(func_def *f, IRContext *ctx) {
    ir_context_append(ctx, "define %s @%s(", map_type_to_llvm(f->ret_type, ctx->pm), f->name);
}

void scope_ir_gen(scope *s, IRContext *ctx) {
    
    // generate IR for all struct defs
    
    // generate IR for all fields

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