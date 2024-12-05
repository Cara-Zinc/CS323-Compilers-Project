#ifndef __SEM_DECLARATION_H__
#define __SEM_DECLARATION_H__

#include "../../mm/program_manager.h"
#include "../ast.h"

typedef struct id_name_and_sizes {
    char *id;
    size_t size_count;
    size_t *sizes;
} id_name_and_sizes;

id_name_and_sizes *id_name_and_sizes_new(char *id);

void id_name_and_sizes_free(id_name_and_sizes *ins);

void id_name_and_sizes_push_size(id_name_and_sizes *ins, size_t size);

id_name_and_sizes *vardec_semantic(program_manager *pm, ASTNode *node);

func_def *fundef_semantic(program_manager *pm, ASTNode *node);

func_def *fundec_semantic(program_manager *pm, ASTNode *node, type_def *type);

varlist *varlist_semantic(program_manager *pm, ASTNode *node);

field_def *paramdec_semantic(program_manager *pm, ASTNode *node);

field_def *dec_semantic(program_manager *pm, ASTNode *node);

#endif