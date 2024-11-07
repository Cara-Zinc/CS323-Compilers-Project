#ifndef __FIELD_H__
#define __FIELD_H__

#include "list.h"
#include "type.h"

typedef struct {
    char *name;
    type_id type;
} field_def;

typedef struct {
    char *name;
    type_id type;
    void *value;
} readonly_field_def;

#endif