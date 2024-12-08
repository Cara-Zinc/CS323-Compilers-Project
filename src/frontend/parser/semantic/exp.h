#ifndef __SEM_EXP_H__
#define __SEM_EXP_H__

#include "../../mm/program_manager.h"
#include "../ast.h"

type_def *exp_semantic(program_manager *pm, ASTNode *node);

#endif