#include "../defs/struct.h"
#include <cmc/utl/foreach.h>

void main() {
    structmap *map = smap_new(100, 0.6, &smap_fkeys, &smap_fvals);
    CMC_FOREACH(smap, structmap, iter, map) {
        printf("Key: %s\n", smap_iter_key(&iter));
        printf("Value: %s\n", smap_iter_value(&iter));
    }
}