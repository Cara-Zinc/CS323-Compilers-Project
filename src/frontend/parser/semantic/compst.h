#ifndef __SEM_COMPST_H__
#define __SEM_COMPST_H__

#include "../../mm/program_manager.h"
#include "../ast.h"

void compst_semantic(program_manager *pm, ASTNode *node, func_def *func);

#endif