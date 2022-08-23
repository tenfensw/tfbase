//
//  list.c
//  tfbase
//
//  Created by Tim K on 21.08.22.
//

#include "list.h"

//
// private
//

tf_item_ref tf_item_init(tf_item_ref previous, void* mem, const tf_deallocator_t autorelease) {
    if (!mem)
        return NULL;
    
    tf_item_ref item = tf_tmalloc(tf_item_s);
    
    if (previous && previous->next) {
        // in this case we are inserting ourselves
        item->next = previous->next;
        previous->next = item;
    }
    
    item->mem = mem;
    item->autorelease = autorelease;
    
    return item;
}

tf_item_ref tf_item_get_last_referenced(tf_item_ref start, tf_index_t* countP, const bool search,
                                        const tf_index_t index) {
    TF_SET_IF_NOT_NULL(countP, 0);
    
    if (!start)
        return NULL;
    
    tf_index_t count = 1;
    tf_item_ref current = start;
    
    while (current->next) {
        if ((count - 1) == index && search)
            break; // this is the right item
        
        current = current->next;
        count++;
    }
    
    if (search && (index + 1) < count)
        current = NULL;
    
    TF_SET_IF_NOT_NULL(countP, count);
    return current;
}

void tf_item_release(tf_item_ref item) {
    tf_item_ref current = item;
    
    while (current) {
        tf_item_ref next = current->next;
        
        if (current->autorelease)
            current->autorelease(current->mem);
        
        free(current);
        current = next;
    }
}

//
// public
//

tf_list_ref tf_list_init_empty() {
    tf_list_ref list = tf_tmalloc(tf_list_s);
    return list;
}

tf_list_ref tf_list_init_with_items(const tf_deallocator_t autorelease, ...) {
    tf_list_ref list = tf_tmalloc(tf_list_s);
    
    // add all of the specified arguments into the list
    va_list vl;
    va_start(vl, autorelease);
    
    void* item = NULL;
    while (true) {
        item = va_arg(vl, void*);
        
        if (!item)
            break;
        
        tf_list_push(list, item, autorelease);
    }
    
    return list;
}


void tf_list_rewind(tf_list_ref list) {
    if (!list)
        return;
    
    list->current = list->first;
}

bool tf_list_get_next(tf_list_ref list, void** whereTo) {
    if (!list)
        return false;
    
    if (list->current) {
        TF_SET_IF_NOT_NULL(whereTo, list->current->mem);
        list->current = list->current->next;
        
        return true;
    }
    
    return false;
}

void tf_list_dump(tf_list_ref list, void* whereTo, const bool strings) {
    FILE* desc = (FILE*)whereTo;
    if (!desc)
        desc = stderr;
    
    if (!list)
        fprintf(desc, "(null)\n");
    else {
        fprintf(desc, "=======================\n");
        fprintf(desc, "<%p>, items:\n", list);
        tf_list_rewind(list);
        
        tf_index_t count = 0;
        void* mem = NULL;
        
        while (tf_list_get_next(list, &mem)) {
            fprintf(desc, "[" TF_INDEX_PRINTF "] ", count);
            if (strings)
                fprintf(desc, "%s\n", (char*)mem);
            else
                fprintf(desc, "mem = <%p>\n", mem);
            
            count++;
        }
        
        fprintf(desc, TF_INDEX_PRINTF " items total\n", count);
        fprintf(desc, "=======================\n");
    }
}

void* tf_list_get_first(const tf_list_ref list) {
    if (!list)
        return NULL;
    
    return (list->first ? list->first->mem : NULL);
}

void* tf_list_get_at(const tf_list_ref list, const tf_index_t where) {
    if (!list)
        return NULL;
    
    tf_item_ref item = tf_item_get_last_referenced(list->first, NULL, true, where);
    return (item ? item->mem : NULL);
}

void* tf_list_get_last(const tf_list_ref list) {
    if (!list)
        return NULL;
    
    tf_item_ref item = tf_item_get_last_referenced(list->first, NULL, false, 0);
    return (item ? item->mem : NULL);
}

void tf_list_push(tf_list_ref list, void* data, const tf_deallocator_t autorelease) {
    if (!list || !data)
        return;
    
    if (!list->first)
        list->first = tf_item_init(NULL, data, autorelease);
    else {
        tf_item_ref last = tf_item_get_last_referenced(list->first, NULL, false, 0);
        last->next = tf_item_init(last, data, autorelease);
    }
}

tf_index_t tf_list_get_count(const tf_list_ref list) {
    if (!list)
        return 0;
    
    tf_index_t count = 0;
    tf_item_get_last_referenced(list->first, &count, false, 0);
    
    return count;
}

void tf_list_release(tf_list_ref list) {
    if (!list)
        return;
    
    // release all items first
    tf_item_release(list->first);
    free(list);
}
