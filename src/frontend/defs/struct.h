#ifndef __STRUCT_H__
#define __STRUCT_H__

// Let's call it Structra! LGTM!

#include <stdio.h>

#include "../utils/util.h"
#include "type.h"
#include "scope.h"

typedef struct scope scope;
typedef struct field_def field_def;
typedef struct func_def func_def;

// definition of a struct
typedef struct struct_def {
    type_id id; // id of the struct
    char *name; // name of the struct
    scope *scope; // scope of the struct
} struct_def;

#define INVALID_STRUCT_NAME "<invalid_struct>"

// create a new struct definition
struct_def *struct_def_new(type_id id, char *name);

// free a struct definition
void struct_def_free(struct_def *s);

void struct_def_add_field(struct_def *s, field_def *f);

field_def *struct_def_get_field(struct_def *s, char *name);

void struct_def_add_func(struct_def *s, func_def *func);

func_def *struct_def_get_func(struct_def *s, char *name);

void struct_def_add_struct(struct_def *s, struct_def *struct_def);

struct_def *struct_def_get_struct(struct_def *s, char *name);

void struct_def_add_subscope(struct_def *s, scope *scope);

scope *struct_def_get_subscope(struct_def *s, size_t index);

size_t struct_def_get_subscope_count(struct_def *s);

int struct_def_cmp(struct_def *s1, struct_def *s2);

struct_def *struct_def_cpy(struct_def *s);

bool struct_def_str(FILE *file, struct_def *s);

size_t struct_def_hash(struct_def *s);

#include <cmc/utl/futils.h>

#define SNAME structmap
#define PFX smap
#define K char*
#define V struct_def*
#include "../collections/hashmap_h.h"

typedef struct structmap structmap;

extern struct structmap_fkey smap_fkeys;

extern struct structmap_fval smap_fvals;

#define SNAME typeid_structmap
#define PFX tsmap
#define K type_id
#define V struct_def*
#include "../collections/hashmap_h.h"

typedef struct typeid_structmap typeid_structmap;

extern struct typeid_structmap_fkey tsmap_fkeys;

extern struct typeid_structmap_fval tsmap_fvals;

#endif