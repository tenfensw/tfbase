//
//  stringbuilder.h
//  tfbase
//
//  Created by Tim K on 22.08.22.
//

#pragma once

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tfbase.h"

struct tf_stringbuilder_s {
    // the resulting string buffer
    char* raw;
    
    // how much bytes were allocated for the string buffer
    tf_index_t allocated;
    // how much characters are used in the string buffer
    tf_index_t count;
};

#define TF_STRINGBUILDER_STEP 25

/// automatically realloc-s the string buffer if necessary (necessity is determined whether the
/// specified amount of characters can be fit into the buffer by default or not)
void tf_stringbuilder_extend_memory(tf_stringbuilder_ref builder, const tf_index_t amount);
