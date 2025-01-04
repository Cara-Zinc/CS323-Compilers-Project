#ifndef __IR_CONTEXT_H__
#define __IR_CONTEXT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "symbol.h"
#include "../defs/type.h"
#include "../defs/field.h"
#include "../defs/struct.h"
#include "../defs/func.h"
#include "../mm/program_manager.h"
#include "../utils/util.h"

// A struct to store the context of IR code and symbols
typedef struct IRContext {
    FILE *ir_file;           // file pointer, store the generated IR code
    size_t ir_size;          // current length of IR code   
    int temp_count;          // count of temporary variables
    SymbolTable *sym_table;  // symbol table
    program_manager *pm;     // program manager
} IRContext;


IRContext* ir_context_create(const char *filename, program_manager *pm);

void ir_context_free(IRContext *ctx);

// append a string to IR code   
void ir_context_append(IRContext *ctx, const char *fmt, ...);

// generate a new temporary variable name
char* ir_context_new_temp(IRContext *ctx);

char *map_type_to_llvm(type_def *type);

#endif
