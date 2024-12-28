#ifndef __SEM_EXP_H__
#define __SEM_EXP_H__

#include "../../mm/program_manager.h"
#include "../ast.h"
#include "../../defs/exp/exp.h"

exp *exp_semantic(program_manager *pm, ASTNode *node);

exp *exp_id_semantic(program_manager *pm, ASTNode *node);

exp *exp_bi_op_semantic(program_manager *pm, ASTNode *left, char *op, ASTNode *right);

exp *exp_unary_op_semantic(program_manager *pm, char *op, ASTNode *child);

exp *exp_func_semantic(program_manager *pm, ASTNode *func_id, ASTNode *args);

exp *exp_array_semantic(program_manager *pm, ASTNode *exp1, ASTNode *exp2);

exp *exp_struct_semantic(program_manager *pm, ASTNode *struct_exp_node, ASTNode *id_node);

exp *exp_struct_func_semantic(program_manager *pm, ASTNode *struct_exp_node, ASTNode *id_node, ASTNode *args_node);

exp *exp_primitive_semantic(program_manager *pm, ASTNode *node);

#endif