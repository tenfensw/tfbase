//
//  stringbuilder.c
//  tfbase
//
//  Created by Tim K on 22.08.22.
//

#include "stringbuilder.h"

//
// private
//

void tf_stringbuilder_extend_memory(tf_stringbuilder_ref builder, const tf_index_t amount) {
    if (!builder)
        return;
    
    // I would single-line this, but clang keeps complaining about the brackets :P
    tf_index_t compAmount = builder->count + amount + 1;
    
    if (!builder->raw) {
        // first time allocation
        builder->allocated = amount;
        builder->raw = calloc(builder->allocated, sizeof(char));
    } else if (compAmount < builder->allocated) {
        tf_index_t oldAllocated = builder->allocated;
        
        // reallocate the existing buffer
        builder->allocated += amount + 2;
        builder->raw = realloc(builder->raw, sizeof(char) * builder->allocated);
        
        // zero it out for safety
        memset(builder->raw + oldAllocated, 0, amount + 2);
    }
}

//
// public
//

tf_stringbuilder_ref tf_stringbuilder_init() {
    tf_stringbuilder_ref builder = tf_tmalloc(tf_stringbuilder_s);
    return builder;
}

tf_stringbuilder_ref tf_stringbuilder_init_with_string(const char* initial) {
    if (!initial)
        return tf_stringbuilder_init();
    
    tf_stringbuilder_ref builder = tf_tmalloc(tf_stringbuilder_s);
    tf_stringbuilder_concat(builder, initial);
    
    return builder;
}

tf_index_t tf_stringbuilder_get_length(const tf_stringbuilder_ref builder) {
    return (builder ? builder->count : 0);
}

const char* tf_stringbuilder_get_current(const tf_stringbuilder_ref builder) {
    return (builder ? builder->raw : NULL);
}

char tf_stringbuilder_get_first(const tf_stringbuilder_ref builder) {
    if (builder && builder->raw && builder->count >= 1)
        return builder->raw[0];
    
    return '\0';
}

char tf_stringbuilder_get_last(const tf_stringbuilder_ref builder) {
    if (builder && builder->raw && builder->count >= 1)
        return builder->raw[builder->count - 1];
    
    return '\0';
}

void tf_stringbuilder_add_character(tf_stringbuilder_ref builder, const char what) {
    if (what == '\0' || !builder)
        return;
    
    tf_stringbuilder_extend_memory(builder, 5);
    builder->raw[builder->count++] = what;
}

void tf_stringbuilder_remove_last_characters(tf_stringbuilder_ref builder, const tf_index_t count) {
    if (!builder || count < 1)
        return;
    
    for (tf_index_t times = 0; times < count; times++) {
        if (builder->count < 1)
            break;
        
        builder->raw[--builder->count] = '\0';
    }
}

void tf_stringbuilder_concat(tf_stringbuilder_ref builder, const char* ending) {
    if (!ending || !builder)
        return;
    
    tf_index_t length = strlen(ending);
    
    tf_stringbuilder_extend_memory(builder, length);
    strcat(builder->raw, ending);
    
    builder->count += length;
}

char* tf_stringbuilder_finish(tf_stringbuilder_ref builder) {
    if (!builder)
        return NULL;
    
    char* result = builder->raw;
    
    // clean all things up
    bzero(builder, sizeof(struct tf_stringbuilder_s));
    
    return result;
}

bool tf_stringbuilder_ends_with(const tf_stringbuilder_ref builder, const char* ending) {
    if (!builder || !builder->raw || !ending)
        return false;
    
    tf_index_t eLength = strlen(ending);
    if (eLength > builder->count)
        return false;
    
    tf_index_t substringBegin = builder->count - eLength;
    
    for (tf_index_t index = 0; index < eLength; index++) {
        if (builder->raw[substringBegin + index] != ending[index])
            return false;
    }
    
    return true;
}

void tf_stringbuilder_release(tf_stringbuilder_ref builder) {
    if (!builder)
        return;
    
    if (builder->raw)
        free(tf_stringbuilder_finish(builder));
    
    free(builder);
}
