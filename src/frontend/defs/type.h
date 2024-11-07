#ifndef __TYPE_H__
#define __TYPE_H__

#include <stddef.h>

#define TYPE_STRUCT_OFFSET 10000

typedef size_t type_id;
// variable type is design as follows:
// 0: int
// 1: float
// 2: char
// 10000+: structs

#endif