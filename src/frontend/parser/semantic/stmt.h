#ifndef __SEM_STMT_H__
#define __SEM_STMT_H__

#include "../../mm/program_manager.h"
#include "../ast.h"

// plain semantic analysis of a statement
void stmt_semantic(program_manager *pm, ASTNode *node, func_def *func);

// semantic analysis of a return statement
void return_semantic(program_manager *pm, ASTNode *node, func_def *func);

// semantic analysis of a compound statement
void compst_semantic(program_manager *pm, ASTNode *node, func_def *func);

// semantic analysis of a definition list
void DefList_semantic(program_manager *pm, ASTNode *node);

// semantic analysis of a statement list
void StmtList_semantic(program_manager *pm, ASTNode *node, func_def *func);

// semantic analysis of an if statement
void if_semantic(program_manager *pm, ASTNode *node, func_def *func);

// semantic analysis of a while statement
void while_semantic(program_manager *pm, ASTNode *node, func_def *func);

#endif