//
//  list.h
//  tfbase
//
//  Created by Tim K on 21.08.22.
//

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tfbase.h"

/// linked list item object
typedef struct tf_item_s* tf_item_ref;
struct tf_item_s {
    tf_item_ref next;
    
    void* mem;
    // if not NULL, then is used for memory management for this
    // specific item
    tf_deallocator_t autorelease;
};

tf_item_ref tf_item_init(tf_item_ref previous, void* mem, const tf_deallocator_t autorelease);
/// internally-used function for either obtaining the last item object in the linked list or for
/// index-based search
tf_item_ref tf_item_get_last_referenced(tf_item_ref start, tf_index_t* countP, const bool search,
                                        const tf_index_t index);
/// release this instance of tf_item_ref as well as all other instances referenced after this item
void tf_item_release(tf_item_ref item);

struct tf_list_s {
    tf_item_ref first;
    
    // as you might probably guess from the implementation, using tf_list_get_at is horrible for ite-
    // ration, so instead there is an iteration mode built-in right into the list itself
    tf_item_ref current;
};
