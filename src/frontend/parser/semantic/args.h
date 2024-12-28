#ifndef __SEM_ARGS_H__
#define __SEM_ARGS_H__

#include <stdio.h>
#include "../../mm/program_manager.h"
#include "../ast.h"
#include "../../defs/exp/exp.h"

explist *args_semantic(program_manager *pm, ASTNode *node);

#endif