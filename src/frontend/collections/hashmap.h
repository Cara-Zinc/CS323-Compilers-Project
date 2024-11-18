// modified based on cmc

#include "cmc/cor/core.h"
#include "cmc/cor/hashtable.h"

#ifdef CMC_DEV
#include "cmc/utl/log.h"
#endif

/**
 * Used values
 * K - hashmap key data type
 * V - hashmap value data type
 * SNAME - struct name and prefix of other related structs
 * PFX - functions prefix
 */

/* Function implementation */
#include "cmc/hashmap/code.h"

/**
 * Extensions
 *
 * INIT - Initializes the struct on the stack
 * ITER - hashmap iterator
 * STR - Print helper functions
 */
#define CMC_EXT_HASHMAP_PARTS INIT, ITER, STR
/**/
#include "cmc/hashmap/ext/code.h"

#include "cmc/cor/undef.h"