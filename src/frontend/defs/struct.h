#ifndef __STRUCT_H__
#define __STRUCT_H__

// Let's call it Structra! LGTM!

#include "util.h"
#include "scope.h"

// definition of a struct
typedef struct {
    char *name; // name of the struct
    scope *scope; // scope of the struct
} struct_def;

// create a new struct definition
struct_def *struct_def_new(char *name) {
    struct_def *s = new(struct_def);
    s->name = name;
    s->scope = scope_new();
}

// free a struct definition
void struct_def_free(struct_def *s) {
    free(s->name);
    scope_free(s->scope);
    free(s);
}

#define SNAME structmap
#define PFX smap
#define K char*
#define V struct_def*
#include <cmc/hashmap.h>

typedef struct structmap structmap;

struct structmap_fkey smap_fkeys = {
    .cmp = cmc_str_cmp,
    .cpy = cmc_str_cpy,
    .str = cmc_str_str,
    .free = free,
    .hash = cmc_str_hash_java,
    .pri = cmc_str_cmp,
};

int struct_def_cmp(struct_def *s1, struct_def *s2) {
    return cmc_str_cmp(s1->name, s2->name);
}

struct_def *struct_def_cpy(struct_def *s) {
    struct_def *res = new(struct_def);
    res->name = str_copy(s->name);
    res->scope = scope_cpy(s->scope);
    return res;
}

bool struct_def_str(FILE *file, struct_def *s) {
    fprintf(file, "struct %s", s->name);
    return true;
}

size_t struct_def_hash(struct_def *s) {
    return cmc_str_hash_java(s->name);
}

struct structmap_fval smap_fvals = {
    .cmp = struct_def_cmp,
    .cpy = struct_def_cpy,
    .str = struct_def_str,
    .free = struct_def_free,
    .hash = struct_def_hash,
    .pri = struct_def_cmp,
};

#endif