#include "declaration.h"
#include "spec.h"
#include "exp.h"
#include "compst.h"
#include "../../utils/util.h"
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
    if (strcmp(node->nodeType, "ID")) {
        return id_name_and_sizes_new(str_copy(node->text));
    }

    id_name_and_sizes *ins = vardec_semantic(pm, alist_get(node->children, 0));
    id_name_and_sizes_push_size(ins, atoi(alist_get(node->children, 2)->text));
    return ins;
}

func_def *fundef_semantic(program_manager *pm, ASTNode *node) {
    type_def *type = specifier_semantic(pm, alist_get(node->children, 0));
    func_def *func = fundec_semantic(pm, alist_get(node->children, 1), type);
    compst_semantic(pm, alist_get(node->children, 2), func);
    scope_wrapper_free_without_data(program_manager_pop(pm));
    return func;
}

func_def *fundec_semantic(program_manager *pm, ASTNode *node, type_def *type) {
    char *func_name = alist_get(node->children, 0)->text;
    if (program_manager_get_func_local(pm, func_name) != NULL) {
        fprintf(stderr, "Error at line %zu: redeclaration of function %s.\n", node->line, func_name);
        return NULL;
    }

    func_def *func = program_manager_create_func(pm, str_copy(func_name), type);

    if (node->numChildren == 4) {
        varlist *list = varlist_semantic(pm, alist_get(node->children, 2));
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
        if (field_def_cmp(vlist_get(list, i)->name, param->name) == 0) {
            fprintf(stderr, "Error at line %zu: redeclaration of variable %s.\n", node->line, param->name);
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
    for (size_t i = 0; i < ins->size_count; i++) {
        type = type_def_new_array(type, ins->sizes[i]);
    }
    field_def *field = field_def_new(str_copy(ins->id), type);
    id_name_and_sizes_free(ins);
    return field;
}

field_def *dec_semantic(program_manager *pm, ASTNode *node) {
    type_def *type = NULL;
    if (node->numChildren == 3) {
        type = exp_semantic(pm, alist_get(node->children, 2));
        if (type->is_array || type->is_struct) {
            fprintf(stderr, "Error at line %zu: cannot assign array or struct to variable.\n", node->line);
        }
    }
    return field_def_new(vardec_semantic(pm, alist_get(node->children, 0)), type);
}