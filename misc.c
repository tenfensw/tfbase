//
//  misc.c
//  tfbase
//
//  Created by Tim K on 21.08.22.
//

#include <stdlib.h>
#include <string.h>
#include "tfbase.h"

void* tf_malloc(const tf_index_t size) {
    void* mem = malloc(size);
    bzero(mem, size);
    
    return mem;
}
