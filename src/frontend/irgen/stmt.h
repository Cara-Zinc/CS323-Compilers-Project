#ifndef __IR_GEN_STMT_H__
#define __IR_GEN_STMT_H__

#include "ir_context.h"
#include "exp.h"
#include "../defs/stmt/stmt.h"

void stmt_ir_gen(stmt *s, IRContext *ctx);

void compst_ir_gen(struct stmtlist *stmts, IRContext *ctx);

void stmtlist_ir_gen(struct stmtlist *stmts, IRContext *ctx);

#endif