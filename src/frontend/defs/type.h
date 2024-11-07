#ifndef __TYPE_H__
#define __TYPE_H__

#include <stddef.h>

typedef size_t type_id;

// variable type is design as follows and convert them into type_id
// 0: int
// 1: float
// 2: char
#define TYPE_INT 0
#define TYPE_FLOAT 1
#define TYPE_CHAR 2

#endif