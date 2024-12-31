#include "ir_context.h"
#include <stdarg.h>

IRContext* ir_context_create(const char *filename) {
    IRContext *ctx = malloc(sizeof(IRContext));
    if(!ctx)
    {
        fprintf(stderr, "Failed to allocate memory for IRContext\n");
        exit(EXIT_FAILURE);
    }
    
    ctx->ir_size = 0;
    ctx->ir_file = fopen(filename, "w");
    if(!ctx->ir_file)
    {
        fprintf(stderr, "Failed to open file %s\n", filename);
        free(ctx);
        exit(EXIT_FAILURE);
    }
    ctx->temp_count = 1;
    ctx->sym_table = symbol_table_create();
    return ctx;
}

void ir_context_free(IRContext *ctx) {
    if (!ctx) return;
    if(ctx->ir_file)
    {
        fclose(ctx->ir_file);
    }
    symbol_table_free(ctx->sym_table);
    free(ctx);
}

// append a llvm IR to IR Context
void ir_context_append(IRContext *ctx, const char *fmt, ...) {
    if(!ctx || !ctx->ir_file) return;
    va_list args;
    va_start(args, fmt);   
    vfprintf(ctx->ir_file, fmt, args);
    va_end(args);

    // to check the file content live, use fflush
    fflush(ctx->ir_file);
}

// generate a new temporary variable name
char* ir_context_new_temp(IRContext *ctx) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "temp_%d", ctx->temp_count++);
    return strdup(buffer);
}

char *map_type_to_llvm(type_def *type)
{
    if(type->is_array || type->is_struct)
    {
        return "i32*";
    }

    switch (type->type_id)
    {
    case TYPE_INT:
        return "i32";
    case TYPE_FLOAT:
        return "float";
    case TYPE_CHAR:
        return "i8";
    case TYPE_VOID:
        return "void";
    default:
        return "i32*";
    }
}