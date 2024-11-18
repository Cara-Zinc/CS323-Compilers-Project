#ifndef __UTIL_H__
#define __UTIL_H__

// allocate memory for a new object of type
#define new(type) ((type *) malloc(sizeof(type)))

void str_free(char *str);
#endif