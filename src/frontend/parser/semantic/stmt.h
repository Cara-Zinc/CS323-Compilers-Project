#ifndef __SEM_STMT_H__
#define __SEM_STMT_H__

#include "../../mm/program_manager.h"
#include "../ast.h"
#include "declaration.h"
#include "../../defs/stmt/stmt.h"

// plain semantic analysis of a statement
stmt *stmt_semantic(program_manager *pm, ASTNode *node, func_def *func);

// semantic analysis of a return statement
stmt *return_semantic(program_manager *pm, ASTNode *node, func_def *func);

// semantic analysis of a compound statement
stmt *compst_semantic(program_manager *pm, ASTNode *node, func_def *func);

// semantic analysis of an if statement
stmt *if_semantic(program_manager *pm, ASTNode *node, func_def *func);

// semantic analysis of a while statement
stmt *while_semantic(program_manager *pm, ASTNode *node, func_def *func);

// semantic analysis of an if-else statement
stmt *if_else_semantic(program_manager *pm, ASTNode *node, func_def *func);

#endif