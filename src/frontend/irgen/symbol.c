#include "symbol.h"

#define INITIAL_CAPACITY 16

SymbolTable* symbol_table_create() {
    SymbolTable *sym_table = malloc(sizeof(SymbolTable));
    if (!sym_table) {
        fprintf(stderr, "Failed to allocate memory for SymbolTable\n");
        return NULL;
    }
    sym_table->size = 0;
    sym_table->capacity = INITIAL_CAPACITY;
    sym_table->symbols = malloc(sizeof(Symbol *) * sym_table->capacity);
    return sym_table;
}

void symbol_table_free(SymbolTable *sym_table) {
    if (!sym_table) return;
    for (size_t i = 0; i < sym_table->size; i++) {
        free(sym_table->symbols[i]->name);
        free(sym_table->symbols[i]->llvm_name);
        free(sym_table->symbols[i]);
    }
    free(sym_table->symbols);
    free(sym_table);
}

void symbol_table_add(SymbolTable *sym_table, const char *name, const char *llvm_name) {
    if (!sym_table) return;
    if (sym_table->size >= sym_table->capacity) {
        sym_table->capacity = sym_table->capacity * 2 + 1;
        sym_table->symbols = realloc(sym_table->symbols, sizeof(Symbol *) * sym_table->capacity);
    }
    Symbol *sym = malloc(sizeof(Symbol));
    sym->name = malloc(strlen(name) + 1);
    strcpy(sym->name, name);
    sym->llvm_name = malloc(strlen(llvm_name) + 1);
    strcpy(sym->llvm_name, llvm_name);
    sym_table->symbols[sym_table->size++] = sym;
}

const char* symbol_table_get(SymbolTable *sym_table, const char *name) {
    if (!sym_table) return NULL;
    for (size_t i = 0; i < sym_table->size; i++) {
        if (strcmp(sym_table->symbols[i]->name, name) == 0) {
            return sym_table->symbols[i]->llvm_name;
        }
    }
    return NULL;
}

char* get_variable_llvm_ptr(const char *var_name, SymbolTable *sym_table) {
    const char *llvm_name = symbol_table_get(sym_table, var_name);
    if (!llvm_name) {
        fprintf(stderr, "Variable %s not found in symbol table\n", var_name);
        return NULL;
    }
    char *result = malloc(strlen(llvm_name) + 2);
    sprintf(result, "%%%s", llvm_name);
    return result;
}