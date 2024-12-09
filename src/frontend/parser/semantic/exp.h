#ifndef __SEM_EXP_H__
#define __SEM_EXP_H__

#include "../../mm/program_manager.h"
#include "../ast.h"

type_def *exp_semantic(program_manager *pm, ASTNode *node);

type_def *exp_id_semantic(program_manager *pm, ASTNode *node);

type_def *exp_bi_op_semantic(program_manager *pm, ASTNode *left, char *op, ASTNode *right);

type_def *exp_unary_op_semantic(program_manager *pm, char *op, ASTNode *child);

type_def *exp_assign_semantic(program_manager *pm, ASTNode *left, ASTNode *right);

type_def *exp_func_semantic(program_manager *pm, ASTNode *func_id, ASTNode *args);

type_def *exp_array_semantic(program_manager *pm, ASTNode *exp1, ASTNode *exp2);

type_def *exp_struct_semantic(program_manager *pm, ASTNode *exp, char *id);

type_def *exp_struct_func_semantic(program_manager *pm, ASTNode *exp, char *id, ASTNode *args);

type_def *exp_primitive_semantic(program_manager *pm, char *type, char *text);

#endif