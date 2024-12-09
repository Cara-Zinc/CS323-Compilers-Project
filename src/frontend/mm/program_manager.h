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

// crate a new program manager
program_manager *program_manager_new();

// free a program manager
void program_manager_free(program_manager *pm);

// create a new field/variable/member on current scope
field_def *program_manager_create_field(program_manager *pm, char *name, type_def *type_spec);

// get a field/variable/member by name
field_def *program_manager_get_field(program_manager *pm, char *name);

// get a field/variable/member by name on current scope
field_def *program_manager_get_field_local(program_manager *pm, char *name);

// create a new function definition on current scope
func_def *program_manager_create_func(program_manager *pm, char *name, type_def *return_type);

// create a new invalid function definition on current scope
// the manager will not save this function to current scope, only keep it on the stack for checking
func_def *program_manager_create_func_invalid(program_manager *pm, type_def *return_type);

// get a function definition by name
func_def *program_manager_get_func(program_manager *pm, char *name);

// get a function definition by name on current scope
func_def *program_manager_get_func_local(program_manager *pm, char *name);

// create a new struct definition on current scope
struct_def *program_manager_create_struct(program_manager *pm, char *name);

// create a new invalid struct definition on current scope
// the manager will not save this struct to current scope, only keep it on the stack for checking
struct_def *program_manager_create_struct_invalid(program_manager *pm);

// get a struct definition by name
struct_def *program_manager_get_struct(program_manager *pm, char *name);

// get a struct definition by name on current scope
struct_def *program_manager_get_struct_local(program_manager *pm, char *name);

// get a struct definition by id
struct_def *program_manager_get_struct_by_id(program_manager *pm, type_id id);

// create a new subscope on current scope
scope *program_manager_create_subscope(program_manager *pm);

// get a subscope by index on current scope
scope *program_manager_get_subscope(program_manager *pm, size_t index);

// get the count of subscopes on current scope
size_t program_manager_get_subscope_count(program_manager *pm);

// get the wrapper of current scope on the stack
scope_wrapper *program_manager_current(program_manager *pm);

// get the current scope the manager is in
scope *program_manager_current_scope(program_manager *pm);

// get the current(nearest) function the manager is in
func_def *program_manager_current_func(program_manager *pm);

// get the current(nearest) struct the manager is in
struct_def *program_manager_current_struct(program_manager *pm);

// exit current scope and return the wrapper of the scope
// note to free the wrapper after using it
scope_wrapper *program_manager_pop(program_manager *pm);

#endif