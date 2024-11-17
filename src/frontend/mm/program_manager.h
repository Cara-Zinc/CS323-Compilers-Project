#ifndef __PROGRAM_MANAGER_H__
#define __PROGRAM_MANAGER_H__

#include "../defs/field.h"
#include "../defs/func.h"
#include "../defs/scope.h"
#include "../defs/scope_wrapper.h"
#include "../defs/struct.h"
#include "../defs/type.h"
// A manager of the program. It manages all the scopes, structs, and functions
typedef struct {
    type_id struct_def_cnt;
    typeid_structmap *struct_defs;
    scope *global_scope;
    scopelist *scope_stack;
    scwrapperlist *scope_wrapper_stack;
} program_manager;

program_manager *program_manager_new();

void program_manager_free(program_manager *pm);

field_def *program_manager_create_field(program_manager *pm, char *name, type_def *type_spec);

field_def *program_manager_get_field(program_manager *pm, char *name);

func_def *program_manager_create_func(program_manager *pm, char *name, type_def *return_type);

func_def *program_manager_get_func(program_manager *pm, char *name);

struct_def *program_manager_create_struct(program_manager *pm, char *name);

struct_def *program_manager_get_struct(program_manager *pm, char *name);

struct_def *program_manager_get_struct_by_id(program_manager *pm, type_id id);

scope *program_manager_create_subscope(program_manager *pm);

scope *program_manager_get_subscope(program_manager *pm, size_t index);

size_t program_manager_get_subscope_count(program_manager *pm);

scope_wrapper *program_manager_current(program_manager *pm);

scope_wrapper *program_manager_pop(program_manager *pm);

#endif