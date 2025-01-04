#define CMC_EXT_ITER

//modified based on cmc

#include "cmc/cor/core.h"

#ifdef CMC_DEV
#include "cmc/utl/log.h"
#endif

#if defined(CMC_SAC) && !defined(SIZE)
#error "When using CMC_SAC, please also define SIZE"
#endif

/**
 * Used values
 * V - list data type
 * SNAME - struct name and prefix of other related structs
 * PFX - functions prefix
 */

/* Function implementation */
#include "cmc/list/code.h"

/**
 * Extensions
 *
 * INIT - Initializes the struct on the stack
 * ITER - List iterator
 * SEQ - Push and pop sequence of items
 * STR - Print helper functions
 */
#define CMC_EXT_LIST_PARTS INIT, ITER, SEQ, STR
/**/
#include "cmc/list/ext/struct.h"
/**/
#include "cmc/list/ext/header.h"
/**/
#include "cmc/list/ext/code.h"

#include "cmc/cor/undef.h"