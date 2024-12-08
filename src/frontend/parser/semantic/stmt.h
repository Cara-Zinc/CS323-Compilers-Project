#ifndef __SEM_STMT_H__
#define __SEM_STMT_H__

#include "../../mm/program_manager.h"
#include "../ast.h"

// plain semantic analysis of a statement
void *stmt_semantic(program_manager *pm, ASTNode *node, func_def *func);

#endif