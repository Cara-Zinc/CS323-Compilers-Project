#ifndef __SEM_ARGS_H__
#define __SEM_ARGS_H__

#include <stdio.h>
#include "../../mm/program_manager.h"
#include "../ast.h"

varlist *args_semantic(program_manager *pm, ASTNode *node);


#endif