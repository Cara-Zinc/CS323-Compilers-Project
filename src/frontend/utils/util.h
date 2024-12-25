#ifndef __UTIL_H__
#define __UTIL_H__

// allocate memory for a new object of type
#define new(type) ((type *) malloc(sizeof(type)))

#define new_n(type, n) ((type *) malloc(sizeof(type) * n))

void str_free(char *str);
#endif