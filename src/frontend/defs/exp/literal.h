#ifndef __EXP_LITERAL_H__
#define __EXP_LITERAL_H__

#include "exp.h"
#include "../type.h"

typedef struct exp_literal {
    type_id type;
    union {
        int int_val;
        float float_val;
        char char_val;
    };
} exp_literal;

#endif