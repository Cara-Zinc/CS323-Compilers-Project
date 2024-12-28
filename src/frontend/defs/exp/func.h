#ifndef __EXP_FUNC_H__
#define __EXP_FUNC_H__

#include "exp.h"

typedef struct exp_func {
    struct exp *lhs_exp; // the struct expression that contains the function, if any
    char *name; // name of the function
    explist *arg_exps; // list of arguments
} exp_func;

#endif