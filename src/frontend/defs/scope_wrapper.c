#include "scope_wrapper.h"

scope_wrapper *scope_wrapper_new(scope_type type, void *data) {
    scope_wrapper *res = new(scope_wrapper);
    res->type = type;
    switch (type) {
        case SCOPE:
            res->scope = data;
            break;
        case STRUCTURE:
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
        case STRUCTURE:
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
        case STRUCTURE:
            return scope_wrapper_new(STRUCTURE, struct_def_cpy(sw->struct_def));
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
        case STRUCTURE:
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
        case STRUCTURE:
            return struct_def_str(file, sw->struct_def);
        case FUNC:
            return func_def_str(file, sw->func);
        default:
            return false;
    }
}