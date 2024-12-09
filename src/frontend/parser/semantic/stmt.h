#ifndef __SEM_STMT_H__
#define __SEM_STMT_H__

#include "../../mm/program_manager.h"
#include "../ast.h"
#include "declaration.h"

// plain semantic analysis of a statement
void stmt_semantic(program_manager *pm, ASTNode *node, func_def *func);

// semantic analysis of a return statement
void return_semantic(program_manager *pm, ASTNode *node, func_def *func);

// semantic analysis of a compound statement
void compst_semantic(program_manager *pm, ASTNode *node, func_def *func);

// semantic analysis of a statement list
void stmtlist_semantic(program_manager *pm, ASTNode *node, func_def *func);

// semantic analysis of an if statement
void if_semantic(program_manager *pm, ASTNode *node, func_def *func);

// semantic analysis of a while statement
void while_semantic(program_manager *pm, ASTNode *node, func_def *func);

// semantic analysis of an if-else statement
void if_else_semantic(program_manager *pm, ASTNode *node, func_def *func);

#endif