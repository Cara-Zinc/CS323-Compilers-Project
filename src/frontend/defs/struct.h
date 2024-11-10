#ifndef __STRUCT_H__
#define __STRUCT_H__

// Let's call it Structra! LGTM!

#include "util.h"
#include "type.h"
#include "scope.h"

// definition of a struct
typedef struct {
    type_id id; // id of the struct
    char *name; // name of the struct
    scope *scope; // scope of the struct
} struct_def;

// create a new struct definition
struct_def *struct_def_new(type_id id, char *name) {
    struct_def *s = new(struct_def);
    s->id = id;
    s->name = name;
    s->scope = scope_new();
}

// free a struct definition
void struct_def_free(struct_def *s) {
    free(s->name);
    scope_free(s->scope);
    free(s);
}

int struct_def_cmp(struct_def *s1, struct_def *s2) {
    return cmc_size_cmp(s1->id, s2->id);
}

struct_def *struct_def_cpy(struct_def *s) {
    struct_def *res = new(struct_def);
    res->id = s->id;
    res->name = str_copy(s->name);
    res->scope = scope_cpy(s->scope);
    return res;
}

bool struct_def_str(FILE *file, struct_def *s) {
    fprintf(file, "struct #%d %s", s->id, s->name);
    return true;
}

size_t struct_def_hash(struct_def *s) {
    return cmc_size_hash(s->id);
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

struct structmap_fval smap_fvals = {
    .cmp = struct_def_cmp,
    .cpy = struct_def_cpy,
    .str = struct_def_str,
    .free = struct_def_free,
    .hash = struct_def_hash,
    .pri = struct_def_cmp,
};

#define SNAME typeid_structmap
#define PFX tsmap
#define K type_id
#define V struct_def*
#include <cmc/hashmap.h>

typedef struct typeid_structmap typeid_structmap;

struct typeid_structmap_fkey tsmap_fkeys = {
    .cmp = cmc_size_cmp,
    .cpy = NULL,
    .str = cmc_size_str,
    .free = NULL,
    .hash = cmc_size_hash,
    .pri = cmc_size_cmp,
};

struct typeid_structmap_fval tsmap_fvals = {
    .cmp = struct_def_cmp,
    .cpy = struct_def_cpy,
    .str = struct_def_str,
    .free = struct_def_free,
    .hash = struct_def_hash,
    .pri = struct_def_cmp,
};

#endif