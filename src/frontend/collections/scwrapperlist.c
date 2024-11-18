#include "../defs/scope_wrapper.h"

#define SNAME scwrapperlist
#define PFX scwlist
#define V scope_wrapper*
#include "list.h"

struct scwrapperlist_fval scwlist_fvals = {
    .cmp = NULL,
    .cpy = scope_wrapper_cpy,
    .str = scope_wrapper_str,
    .free = scope_wrapper_free,
    .hash = NULL,
    .pri = NULL,
};

struct scwrapperlist_fval scwlist_without_data_fvals = {
    .cmp = NULL,
    .cpy = scope_wrapper_cpy_without_data,
    .str = scope_wrapper_str,
    .free = scope_wrapper_free_without_data,
    .hash = NULL,
    .pri = NULL,
};