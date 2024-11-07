#ifndef __FIELD_H__
#define __FIELD_H__

#include "list.h"
#include "type.h"
#include <stdbool.h>

typedef struct {
    char *name;
    type_id type;
    bool is_struct;
    bool is_array;
    size_t array_size;
} field_def;

typedef struct {
    char *name;
    type_id type;
    bool is_struct;
    bool is_array;
    size_t array_size;
    void *value;
} readonly_field_def;

#endif