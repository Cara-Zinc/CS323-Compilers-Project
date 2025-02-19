#ifndef __IR_CONTEXT_H__
#define __IR_CONTEXT_H__
typedef struct IRContext IRContext;

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
#include "scope.h"
#include <cmc/utl/foreach.h>

// A struct to store the context of IR code and symbols
struct IRContext {
    FILE *ir_file;           // file pointer, store the generated IR code
    size_t ir_size;          // current length of IR code
    int temp_count;          // count of temporary variables
    int last_op;             // last operation, if it is a 'br', last_op = 1, else last_op = 0
    SymbolTable *sym_table;  // symbol table
    program_manager *pm;     // program manager
};

IRContext *ir_context_create(const char *filename, program_manager *pm);

void ir_context_free(IRContext *ctx);

// append a string to IR code
void ir_context_append(IRContext *ctx, const char *fmt, ...);

// generate a new temporary variable name
char* ir_context_new_temp(IRContext *ctx);

// generate a new label
char *ir_context_new_label(IRContext *ctx, char *type);

char *map_type_to_llvm(type_def *type, program_manager *pm);

void ir_gen(IRContext *ctx);

#endif
