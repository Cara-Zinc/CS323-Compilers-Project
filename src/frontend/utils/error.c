#include "error.h"

#include <stdarg.h>

bool compiler_error = false;

void eprintf(const char *fmt, ...) {
    compiler_error = true;
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

void efprintf(FILE *file, const char *fmt, ...) {
    compiler_error = true;
    va_list args;
    va_start(args, fmt);
    vfprintf(file, fmt, args);
    va_end(args);
}