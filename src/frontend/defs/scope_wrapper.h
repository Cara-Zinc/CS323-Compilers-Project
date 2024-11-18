#ifndef __SCOPE_WRAPPER_H__
#define __SCOPE_WRAPPER_H__

#include <stdio.h>

#include "scope.h"

typedef enum {
    SCOPE,
    STRUCTURE,
    FUNC,
} scope_type;
/*
    * A wrapper for scope, struct_def, and func_def
    * Its type tells you why is this scope created
*/
typedef struct {
    scope_type type;
    union {
        scope *scope;
        struct_def *struct_def;
        func_def *func;
    };
} scope_wrapper;

scope_wrapper *scope_wrapper_new(scope_type type, void *data);

void scope_wrapper_free(scope_wrapper *sw);

void scope_wrapper_free_without_data(scope_wrapper *sw);

scope_wrapper *scope_wrapper_cpy(scope_wrapper *sw);

scope_wrapper *scope_wrapper_cpy_without_data(scope_wrapper *sw);

bool scope_wrapper_str(FILE *file, scope_wrapper *sw);

#define SNAME scwrapperlist
#define PFX scwlist
#define V scope_wrapper*
#include <cmc/list.h>
// @brief A list of scope_wrappers
typedef struct scwrapperlist scwrapperlist;

struct scwrapperlist_fval scwlist_fvals = {
    .cmp = NULL,
    .cpy = scope_wrapper_cpy,
    .str = scope_wrapper_str,
    .free = scope_wrapper_free,
    .hash = NULL,
    .pri = NULL,
};

struct scwrapperlist_fval scwlist_without_data_fvals = {
    .cmp = NULL,
    .cpy = scope_wrapper_cpy_without_data,
    .str = scope_wrapper_str,
    .free = scope_wrapper_free_without_data,
    .hash = NULL,
    .pri = NULL,
};

#endif