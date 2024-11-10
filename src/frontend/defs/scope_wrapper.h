#ifndef __SCOPE_WRAPPER_H__
#define __SCOPE_WRAPPER_H__

#include "scope.h"

typedef enum {
    SCOPE,
    STRUCT,
    FUNC,
} scope_type;

typedef struct {
    scope_type type;
    union {
        scope *scope;
        struct_def *struct_def;
        func_def *func;
    };
} scope_wrapper;

scope_wrapper *scope_wrapper_new(scope_type type, void *data) {
    scope_wrapper *res = new(scope_wrapper);
    res->type = type;
    switch (type) {
        case SCOPE:
            res->scope = data;
            break;
        case STRUCT:
            res->struct_def = data;
            break;
        case FUNC:
            res->func = data;
            break;
        default:
            free(res);
            return NULL;
    }
    return res;
}

void scope_wrapper_free(scope_wrapper *sw) {
    switch (sw->type) {
        case SCOPE:
            scope_free(sw->scope);
            break;
        case STRUCT:
            struct_def_free(sw->struct_def);
            break;
        case FUNC:
            func_def_free(sw->func);
            break;
    }
    free(sw);
}

void scope_wrapper_free_without_data(scope_wrapper *sw) {
    free(sw);
}

scope_wrapper *scope_wrapper_cpy(scope_wrapper *sw) {
    switch (sw->type) {
        case SCOPE:
            return scope_wrapper_new(SCOPE, scope_cpy(sw->scope));
        case STRUCT:
            return scope_wrapper_new(STRUCT, struct_def_cpy(sw->struct_def));
        case FUNC:
            return scope_wrapper_new(FUNC, func_def_cpy(sw->func));
        default:
            return NULL;
    }
}

scope_wrapper *scope_wrapper_cpy_without_data(scope_wrapper *sw) {
    scope_wrapper *res = new(scope_wrapper);
    res->type = sw->type;
    switch (sw->type) {
        case SCOPE:
            res->scope = sw->scope;
        case STRUCT:
            res->struct_def = sw->struct_def;
        case FUNC:
            res->func = sw->func;
        default:
            free(res);
            return NULL;
    }
}

bool scope_wrapper_str(FILE *file, scope_wrapper *sw) {
    switch (sw->type) {
        case SCOPE:
            return scope_str(file, sw->scope);
        case STRUCT:
            return struct_def_str(file, sw->struct_def);
        case FUNC:
            return func_def_str(file, sw->func);
        default:
            return false;
    }
}

#define SNAME scwrapperlist
#define PFX scwlist
#define V scope_wrapper*
#include <cmc/list.h>

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