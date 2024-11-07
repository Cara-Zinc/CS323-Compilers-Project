#ifndef __TYPE_H__
#define __TYPE_H__

#include <stddef.h>

#define TYPE_ARRAY_FLAG (1 << 24)
#define TYPE_STRUCT_FLAG (1 << 25)

typedef size_t type_id;

// variable type is design as follows and convert them into type_id, bit 24 and bit 25 are ARRAY_FLAG and STRUCT_FLAG:
// 0: int
// 1: float
// 2: char
// 1<<24: array flag, maximum number of arrays is 2^24
// 1<<25: struct flag, maximum number of structs is 2^24
#define TYPE_INT 0
#define TYPE_FLOAT 1
#define TYPE_CHAR 2

#endif