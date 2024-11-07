#ifndef __FUNC_H__
#define __FUNC_H__

#include "list.h"
#include "type.h"
#include "util.h"
#include "field.h"

typedef struct {
    char *name;
    type_id return_type;
    list *args;
} func_def;

func_def *func_def_new(char *name, type_id return_type) {
    func_def *res = new(func_def);
    res->name = name;
    res->return_type = return_type;
    res->args = list_new(field_def);
    return res;
}

#endif