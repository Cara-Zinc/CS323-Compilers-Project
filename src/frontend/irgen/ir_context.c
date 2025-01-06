#include "ir_context.h"
#include <stdarg.h>

IRContext* ir_context_create(const char *filename, program_manager *pm) {
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
    ctx->pm = pm;
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
    snprintf(buffer, sizeof(buffer), "%%temp_%d", ctx->temp_count++);
    return strdup(buffer);
}

// generate a new temporary variable name
char* ir_context_new_label(IRContext *ctx, char *type) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%s_%d", type, ctx->temp_count++);
    return strdup(buffer);
}

char *map_type_to_llvm(type_def *type, program_manager *pm)
{
    if(type->is_array)
    {
        // example: [type.array_size x type.array_type]
        char *array_type = map_type_to_llvm(type->array_type, pm);
        char *array_size = malloc(32);
        snprintf(array_size, 32, "%zu", type->array_size);
        char *result = malloc(strlen(array_type) + strlen(array_size) + 4);
        sprintf(result, "[%s x %s]", array_size, array_type);
        free(array_type);
        free(array_size);
        return result;
    }
    if(type->is_struct)
    {
        struct_def *s = program_manager_get_struct_by_id(pm, type->type_id);

        // traverse the fields(variables) in the struct definition
        varmap *fields = s->scope->fields;
        char *result = malloc(1024);
        char *tmp = malloc(1024);
        strcpy(result, "{ ");
        int vmap_count = 0;
        CMC_FOREACH(vmap, varmap, iter, fields)
        {
            if(vmap_count > 0)
            {
                // add , only if there is more than one field
                strcat(result, ", ");
            }
            field_def *field = vmap_iter_value(&iter);
            char *field_type = map_type_to_llvm(field->type_spec, pm);
            sprintf(tmp, "%s", field_type);
            strcat(result, tmp);
            free(field_type);
            vmap_count++;
        } 
        
        strcat(result, " }");
        free(tmp);
        return result;
    }

    switch (type->type_id)
    {
    case TYPE_INT:
        return strdup("i32");
    case TYPE_FLOAT:
        return strdup("float");
    case TYPE_CHAR:
        return strdup("i8");
    case TYPE_VOID:
        return strdup("void");
    default:
        return strdup("i32*");
    }
}

void ir_gen(IRContext *ctx)
{
    scope *global_scope = ctx->pm->global_scope;
    scope_ir_gen(global_scope, ctx);
}