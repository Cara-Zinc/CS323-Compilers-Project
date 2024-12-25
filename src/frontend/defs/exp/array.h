#ifndef __EXP_ARRAY_H__
#define __EXP_ARRAY_H__

#include "exp.h"

typedef struct exp_array {
    exp *array_exp;
    exp *index_exp;
} exp_array;

#endif