#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdbool.h>
#include <stdio.h>

extern bool compiler_error;

void eprintf(const char *fmt, ...);

void efprintf(FILE *file, const char *fmt, ...);

#endif