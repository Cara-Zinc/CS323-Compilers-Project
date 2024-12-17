#ifndef __IR_GEN_EXP_H__
#define __IR_GEN_EXP_H__

#include "ir_context.h"
#include "../defs/exp/exp.h"


char *exp_bi_op_ir_gen(exp *e, IRContext *ctx);

char *exp_unary_op_ir_gen(exp *e, IRContext *ctx);

char *exp_func_call_ir_gen(exp *e, IRContext *ctx);

char *exp_array_access_ir_gen(exp *e, IRContext *ctx);

char *exp_struct_access_ir_gen(exp *e, IRContext *ctx);

char *exp_literal_ir_gen(exp *e, IRContext *ctx);

char *exp_id_ir_gen(exp *e, IRContext *ctx);

char *exp_ir_gen(exp *e, IRContext *ctx);

#endif