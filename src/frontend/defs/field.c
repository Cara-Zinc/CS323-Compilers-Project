#include "field.h"

// create a new field definition
field_def *field_def_new(char *name, type_def *type_spec) {
    field_def *res = new(field_def);
    res->name = name;
    res->type_spec = type_spec;
    res->assign_exp = NULL;
    return res;
}

field_def *field_def_new_with_exp(char *name, type_def *type_spec, exp *assign_exp) {
    field_def *res = new(field_def);
    res->name = name;
    res->type_spec = type_spec;
    res->assign_exp = assign_exp;
    return res;
}

// free a field definition
void field_def_free(field_def *f) {
    free(f->name);
    type_def_free(f->type_spec);
    if (f->assign_exp != NULL) {
        exp_free(f->assign_exp);
    }
    free(f);
}

int field_def_cmp(field_def *f1, field_def *f2) {
    return cmc_str_cmp(f1->name, f2->name);
}

field_def *field_def_cpy(field_def *f) {
    char *name = str_copy(f->name);
    exp *assign_exp = NULL;
    if (f->assign_exp != NULL) {
        assign_exp = exp_cpy(f->assign_exp);
    }
    return field_def_new_with_exp(name, type_def_cpy(f->type_spec), assign_exp);
}

bool field_def_str(FILE *file, field_def *f) {
    fprintf(file, "variable %s", f->name);
    return true;
}

size_t field_def_hash(field_def *f) {
    return cmc_str_hash_java(f->name);
}
