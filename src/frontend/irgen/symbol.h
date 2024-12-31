#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A struct to store the symbol name and its LLVM IR name
typedef struct Symbol {
    char *name;          // variable name in structra code
    char *llvm_name;     // variable name in LLVM IR
} Symbol;

// A symbol table
typedef struct SymbolTable {
    Symbol **symbols;    // 动态数组的符号表
    size_t size;         // 当前符号数量
    size_t capacity;     // 符号表容量
} SymbolTable;

SymbolTable* symbol_table_create();

void symbol_table_free(SymbolTable *sym_table);

void symbol_table_add(SymbolTable *sym_table, const char *name, const char *llvm_name);

const char* symbol_table_get(SymbolTable *sym_table, const char *name);

// get the LLVM IR name of a variable
char* get_variable_llvm_ptr(const char *var_name, SymbolTable *sym_table);

#endif
