#ifndef __SEM_SPEC_H__
#define __SEM_SPEC_H__

#include "../../mm/program_manager.h"
#include "../ast.h"

type_def *specifier_semantic(program_manager *pm, ASTNode *node);

#endif