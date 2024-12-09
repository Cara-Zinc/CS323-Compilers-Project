#ifndef _ARGS_H_
#define _ARGS_H_

#include <stdio.h>
#include "../../mm/program_manager.h"
#include "../ast.h"

varlist *args_semantic(program_manager *pm, ASTNode *node);


#endif