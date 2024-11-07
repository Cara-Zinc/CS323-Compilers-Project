#ifndef __FUNC_H__
#define __FUNC_H__

#include "list.h"
#include "type.h"

typedef struct {
    char *name;
    type_id return_type;
    list *args;
} func_def;

#endif