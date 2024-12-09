#ifndef __SEM_DECLARATION_H__
#define __SEM_DECLARATION_H__

#include "../../mm/program_manager.h"
#include "../ast.h"

/**
 * A structure to store the name and sizes of a variable.
 */
typedef struct id_name_and_sizes {
    char *id;
    size_t size_count;
    size_t *sizes;
} id_name_and_sizes;

id_name_and_sizes *id_name_and_sizes_new(char *id);

void id_name_and_sizes_free(id_name_and_sizes *ins);

/**
 * Adds a size to the `sizes` array of an `id_name_and_sizes` object.
 *
 * @param ins A pointer to the `id_name_and_sizes` structure.
 * @param size The size value to be added to the array.
 */
void id_name_and_sizes_push_size(id_name_and_sizes *ins, size_t size);

/**
 * Performs semantic analysis for a variable declaration (VarDec).
 *
 * @param pm The program manager to manage scopes and symbols.
 * @param node The AST node representing the variable declaration.
 * @return A pointer to an `id_name_and_sizes` structure containing the variable's name and sizes.
 */
id_name_and_sizes *vardec_semantic(program_manager *pm, ASTNode *node);

func_def *fundef_semantic(program_manager *pm, ASTNode *node);

func_def *fundec_semantic(program_manager *pm, ASTNode *node, type_def *type);

varlist *varlist_semantic(program_manager *pm, ASTNode *node);

field_def *paramdec_semantic(program_manager *pm, ASTNode *node);

field_def *dec_semantic(program_manager *pm, ASTNode *node, type_def *type);

void declist_semantic(program_manager *pm, ASTNode *node, type_def *type);

void def_semantic(program_manager *pm, ASTNode *node);

void deflist_semantic(program_manager *pm, ASTNode *node);

#endif