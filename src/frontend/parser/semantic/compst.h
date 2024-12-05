#ifndef __SEM_COMPST_H__
#define __SEM_COMPST_H__

#include "../../mm/program_manager.h"
#include "../ast.h"

// returns the predicted return type of the function
// in case we can't predict the return type, returns NULL
type_def *compst_func_semantic(program_manager *pm, ASTNode *node, func_def *func);

#endif