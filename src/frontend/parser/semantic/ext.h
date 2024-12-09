#ifndef __SEM_EXT_H__
#define __SEM_EXT_H__

#include "../mm/program_manager.h"
#include "declaration.h"
#include "ast.h"

void ext_def_semantic(program_manager *pm, ASTNode *ExtDef);

id_name_and_sizes *ext_dec_list_semantic(program_manager *pm, ASTNode *ExtDecList);

#endif