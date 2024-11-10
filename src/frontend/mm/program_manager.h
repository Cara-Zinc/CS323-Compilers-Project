#ifndef __PROGRAM_MANAGER_H__
#define __PROGRAM_MANAGER_H__

#include "../defs/field.h"
#include "../defs/func.h"
#include "../defs/scope.h"
#include "../defs/scope_wrapper.h"
#include "../defs/struct.h"
#include "../defs/type.h"

typedef struct {
    type_id struct_def_cnt;
    typeid_structmap *struct_defs;
    scope *global_scope;
    scopelist *scope_stack;
    scwrapperlist *scope_wrapper_stack;
} program_manager;

program_manager *program_manager_new() {
    program_manager *res = new(program_manager);
    res->struct_def_cnt = 0;
    res->struct_defs = tsmap_new(16, 0.5, &tsmap_fkeys, &tsmap_fvals);
    res->global_scope = scope_new();
    res->scope_stack = sclist_new(4, &sclist_fvals);
    res->scope_wrapper_stack = scwlist_new(4, &scwlist_without_data_fvals);
    sclist_push_back(res->scope_stack, res->global_scope);
    return res;
}

void program_manager_free(program_manager *pm) {
    for (type_id id = 0; id < pm->struct_def_cnt; id++) {
        struct_def *sd = NULL;
        tsmap_remove(pm->struct_defs, id, &sd);
    }
    tsmap_free(pm->struct_defs);
    while (sclist_pop_back(pm->scope_stack)) {}
    sclist_free(pm->scope_stack);
    scwlist_free(pm->scope_wrapper_stack);
    scope_free(pm->global_scope);
    free(pm);
}

field_def *program_manager_create_field(program_manager *pm, char *name, type_def *type_spec) {
    field_def *res = field_def_new(name, type_spec);
    scope_add_field(sclist_back(pm->scope_stack), res);
    return res;
}

field_def *program_manager_get_field(program_manager *pm, char *name) {
    for (size_t i = pm->scope_stack->count - 1; i >= 0; i--) {
        scope *s = sclist_get(pm->scope_stack, i);
        field_def *res = scope_get_field(s, name);
        if (res) {
            return res;
        }
        if (i == 0) {
            break;
        }
    }
    return NULL;
}

func_def *program_manager_create_func(program_manager *pm, char *name, type_def *return_type) {
    func_def *res = func_def_new(name, return_type);
    scope_add_func(sclist_back(pm->scope_stack), res);
    sclist_push_back(pm->scope_stack, res->scope);
    scwlist_push_back(pm->scope_wrapper_stack, scope_wrapper_new(FUNC, res->scope));
    return res;
}

func_def *program_manager_get_func(program_manager *pm, char *name) {
    for (size_t i = pm->scope_stack->count - 1; i >= 0; i--) {
        scope *s = sclist_get(pm->scope_stack, i);
        func_def *res = scope_get_func(s, name);
        if (res) {
            return res;
        }
        if (i == 0) {
            break;
        }
    }
    return NULL;
}

struct_def *program_manager_create_struct(program_manager *pm, char *name) {
    type_id id = pm->struct_def_cnt++;
    struct_def *res = struct_def_new(id, name);
    tsmap_insert(pm->struct_defs, id, res);
    scope_add_struct(sclist_back(pm->scope_stack), res);
    sclist_push_back(pm->scope_stack, res->scope);
    scwlist_push_back(pm->scope_wrapper_stack, scope_wrapper_new(STRUCT, res->scope));
    return res;
}

struct_def *program_manager_get_struct(program_manager *pm, char *name) {
    for (size_t i = pm->scope_stack->count - 1; i >= 0; i--) {
        scope *s = sclist_get(pm->scope_stack, i);
        struct_def *res = scope_get_struct(s, name);
        if (res) {
            return res;
        }
        if (i == 0) {
            break;
        }
    }
    return NULL;
}

struct_def *program_manager_get_struct_by_id(program_manager *pm, type_id id) {
    return tsmap_get(pm->struct_defs, id);
}

scope *program_manager_create_subscope(program_manager *pm) {
    scope *res = scope_new();
    scope_add_subscope(sclist_back(pm->scope_stack), res);
    sclist_push_back(pm->scope_stack, res);
    scwlist_push_back(pm->scope_wrapper_stack, scope_wrapper_new(SCOPE, res));
    return res;
}

scope *program_manager_get_subscope(program_manager *pm, size_t index) {
    return scope_get_subscope(sclist_back(pm->scope_stack), index);
}

size_t program_manager_get_subscope_count(program_manager *pm) {
    return scope_get_subscope_count(sclist_back(pm->scope_stack));
}

scope_wrapper *program_manager_current(program_manager *pm) {
    return scwlist_back(pm->scope_wrapper_stack);
}

scope_wrapper *program_manager_pop(program_manager *pm) {
    if (sclist_count(pm->scope_stack) <= 0 || scwlist_count(pm->scope_wrapper_stack) <= 0) {
        return NULL;
    }

    scope_wrapper *res = scwlist_back(pm->scope_wrapper_stack);
    sclist_pop_back(pm->scope_stack);
    scwlist_pop_back(pm->scope_wrapper_stack);
    return res;
}

#endif