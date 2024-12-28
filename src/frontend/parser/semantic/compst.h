#ifndef __SEM_COMPST_H__
#define __SEM_COMPST_H__

#include "../../mm/program_manager.h"
#include "../ast.h"
#include "../../defs/stmt/stmt.h"

// plain semantic analysis of function entry
void func_entry_semantic(program_manager *pm, ASTNode *node, func_def *func);

// semantic analysis of a statement list
stmtlist *stmtlist_semantic(program_manager *pm, ASTNode *node, func_def *func);

#endif