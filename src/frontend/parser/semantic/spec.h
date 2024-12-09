#ifndef __SEM_SPEC_H__
#define __SEM_SPEC_H__

#include "../../mm/program_manager.h"
#include "../ast.h"
#include "declaration.h"

type_def *specifier_semantic(program_manager *pm, ASTNode *node);
type_def *struct_specifier_semantic(program_manager *pm, ASTNode *node);
void struct_def_list_semantic(program_manager *pm, ASTNode *struct_def_list, type_id struct_id);
void struct_def_semantic(program_manager *pm, ASTNode *struct_def);

#endif