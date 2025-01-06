#include "declaration.h"
#include "spec.h"
#include "exp.h"
#include "compst.h"
#include "../../utils/util.h"
#include "../../utils/error.h"
#include <stdlib.h>

id_name_and_sizes *id_name_and_sizes_new(char *id) {
    id_name_and_sizes *ins = malloc(sizeof(id_name_and_sizes));
    ins->id = id;
    ins->size_count = 0;
    ins->sizes = NULL;
    return ins;
}

void id_name_and_sizes_free(id_name_and_sizes *ins) {
    free(ins->id);
    if (ins->sizes != NULL) {
        free(ins->sizes);
    }
    free(ins);
}

void id_name_and_sizes_push_size(id_name_and_sizes *ins, size_t size) {
    ins->size_count++;
    ins->sizes = realloc(ins->sizes, ins->size_count * sizeof(size_t));
    ins->sizes[ins->size_count - 1] = size;
}

id_name_and_sizes *vardec_semantic(program_manager *pm, ASTNode *node) {
    if (!strcmp(node->nodeType, "ID")) {
        return id_name_and_sizes_new(str_copy(node->text));
    }

    if(!node->children)
    {
        efprintf(stderr, "Error at line %zu: invalid variable declaration.\n", node->line);
        return id_name_and_sizes_new("INVALID");
    }

    id_name_and_sizes *ins = vardec_semantic(pm, alist_get(node->children, 0));
    if (node->numChildren == 1)
    {
        // check if it's VarDec -> ID
        return ins;
    }
    int size = atoi(alist_get(node->children, 2)->text);
    if (size < 0) {
        efprintf(stderr, "Error at line %zu: array size must not be negative.\n", alist_get(node->children, 2)->line);
        size = 0;
    }
    id_name_and_sizes_push_size(ins, size);
    return ins;
}

func_def *fundef_semantic(program_manager *pm, ASTNode *node) {
    type_def *return_type = specifier_semantic(pm, alist_get(node->children, 0));
    func_def *func = fundec_semantic(pm, alist_get(node->children, 1), return_type);
    
    if (node->numChildren == 3)
    {
        func_entry_semantic(pm, alist_get(node->children, 2), func);
    }

    scope_wrapper *wrapper = program_manager_pop(pm);
    if (strcmp(wrapper->func->name, INVALID_FUNC_NAME) == 0) {
        scope_wrapper_free(wrapper);
        return NULL;
    }

    scope_wrapper_free_without_data(wrapper);
    return func;
}

func_def *fundec_semantic(program_manager *pm, ASTNode *node, type_def *type) {
    if (strcmp(node->nodeType, "Error") == 0) {
        return program_manager_create_func_invalid(pm, type);
    }

    func_def *func = NULL;
    char *func_name = alist_get(node->children, 0)->text;
    if (program_manager_get_func_local(pm, func_name) != NULL) {
        efprintf(stderr, "Error at line %zu: redeclaration of function %s.\n", alist_get(node->children, 0)->line, func_name);
        func = program_manager_create_func_invalid(pm, type);
    } else {
        func = program_manager_create_func(pm, str_copy(func_name), type);
    }

    if (node->numChildren == 2) {
        varlist *list = varlist_semantic(pm, alist_get(node->children, 1));
        while (!vlist_empty(list)) {
            field_def *param = vlist_back(list);
            vlist_pop_back(list);
            func_def_add_arg(func, param);
        }
        vlist_free(list);
    }

    return func;
}

varlist *varlist_semantic(program_manager *pm, ASTNode *node) {
    varlist *list = NULL;
    if (node->numChildren == 1) {
        list = vlist_new(4, &vlist_fvals);
    } else {
        list = varlist_semantic(pm, alist_get(node->children, 1));
    }
    field_def *param = paramdec_semantic(pm, alist_get(node->children, 0));
    for (size_t i = 0; i < vlist_count(list); i++) {
        if (strcmp(vlist_get(list, i)->name, param->name) == 0) {
            efprintf(stderr, "Error at line %zu: redeclaration of variable %s.\n", alist_get(node->children, 0)->line, param->name);
            field_def_free(param);
            return list;
        }
    }
    vlist_push_back(list, param);
    return list;
}

field_def *paramdec_semantic(program_manager *pm, ASTNode *node) {
    type_def *type = specifier_semantic(pm, alist_get(node->children, 0));
    id_name_and_sizes *ins = vardec_semantic(pm, alist_get(node->children, 1));
    if (ins->size_count > 0) {
        for (size_t i = ins->size_count - 1; i >= 0; i--) {
            type = type_def_new_array(type, ins->sizes[i]);
            if (i == 0) {
                break;
            }
        }
    }
    field_def *field = field_def_new(str_copy(ins->id), type);
    id_name_and_sizes_free(ins);
    return field;
}

field_def *dec_semantic(program_manager *pm, ASTNode *node, type_def *specifier_type) {
    type_def *type = type_def_cpy(specifier_type);
    // set the id and size from vardec_semantic to a new char*, then create a field_def
    id_name_and_sizes *ins = vardec_semantic(pm, alist_get(node->children, 0));
    char *name = str_copy(ins->id);
    if (ins->size_count > 0) {
        for (size_t i = ins->size_count - 1; i >= 0; i--) {
            type = type_def_new_array(type, ins->sizes[i]);
            if (i == 0) {
                break;
            }
        }
    }

    id_name_and_sizes_free(ins);
    ins = NULL;

    exp *assign_exp = NULL;
    // vardec assign exp
    if (node->numChildren == 3)
    {
        assign_exp = exp_semantic(pm, alist_get(node->children, 2));
        type_def *exp_type = assign_exp->result_type;
        bool error = false;
        if (!type_def_is_operable(exp_type)) {
            error = true;
            efprintf(stderr, "Error at line %zu: cannot assign array, struct or void type to variable.\n", alist_get(node->children, 0)->line);
        }
        if (type_def_cmp(type, exp_type) != 0) {
            error = true;
            char *expected_type_name = type_def_name(pm, type);
            char *actual_type_name = type_def_name(pm, exp_type);
            efprintf(stderr, "Error at line %zu: type mismatch, trying to assign type %s to type %s.\n", alist_get(node->children, 0)->line, actual_type_name, expected_type_name);
            str_free(expected_type_name);
            str_free(actual_type_name);
        }
        if (error) {
            exp_free(assign_exp);
            assign_exp = exp_new_invalid();
        }
    }

    field_def *field = program_manager_create_field(pm, name, type);
    field->assign_exp = assign_exp;
    return field;
}

void declist_semantic(program_manager *pm, ASTNode *node, type_def *type) {
    if (node->numChildren == 1)
    {
        dec_semantic(pm, alist_get(node->children, 0), type);
    }
    else
    {
        dec_semantic(pm, alist_get(node->children, 0), type);
        declist_semantic(pm, alist_get(node->children, 1), type);
    }
}

void def_semantic(program_manager *pm, ASTNode *node) {
    type_def *type = specifier_semantic(pm, alist_get(node->children, 0));
    declist_semantic(pm, alist_get(node->children, 1), type);
}

void deflist_semantic(program_manager *pm, ASTNode *node) {
    if (node->numChildren == 2)
    {
        def_semantic(pm, alist_get(node->children, 0));
        deflist_semantic(pm, alist_get(node->children, 1));
    }
}