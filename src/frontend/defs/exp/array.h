#ifndef __EXP_ARRAY_H__
#define __EXP_ARRAY_H__

#include "exp.h"

typedef struct exp_array {
    struct exp *array_exp;
    struct exp *index_exp;
} exp_array;

#endif