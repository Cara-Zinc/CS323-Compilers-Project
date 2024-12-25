#ifndef __EXP_FUNC_H__
#define __EXP_FUNC_H__

#include "exp.h"

typedef struct exp_func {
    struct exp *lhs_exp;
    char *name;
    int num_args;
    struct exp **arg_exps;
} exp_func;

#endif