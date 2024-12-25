#ifndef __EXP_STRUCT_H__
#define __EXP_STRUCT_H__

#include "exp.h"

typedef struct exp_struct_access {
    struct exp *lhs_exp;
    char *field_name;
} exp_struct_access;

#endif