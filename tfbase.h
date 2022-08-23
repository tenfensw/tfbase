//
//  tfbase.h
//  tfbase
//
//  Created by Tim K on 21.08.22.
//

#pragma once

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __clang__
#define __TF_FILE__ __FILE_NAME__
#else
#define __TF_FILE__ __FILE__
#endif

/// indexing type typedef-ed for portability reasons
#ifdef __APPLE__
#define TF_INDEX_PRINTF "%zu"
typedef size_t tf_index_t;
#else
#define TF_INDEX_PRINTF "%u"
typedef uint32_t tf_index_t;
#endif

typedef void (*tf_deallocator_t)(void*);

/// linked list object
typedef struct tf_list_s* tf_list_ref;

/// creates a new empty linked list
tf_list_ref tf_list_init_empty(void);
/// creates a new linked list and populates it with the specified items. Notice that the
/// last argument always must be NULL.
tf_list_ref tf_list_init_with_items(const tf_deallocator_t autorelease, ...);

/// retreives the first item in the linked list
void* tf_list_get_first(const tf_list_ref list);
/// retreives the item located at the specified index in the linked list
void* tf_list_get_at(const tf_list_ref list, const tf_index_t where);
/// retreives the last item in the linked list
void* tf_list_get_last(const tf_list_ref list);

/// returns item count in the specified linked list instance
tf_index_t tf_list_get_count(const tf_list_ref list);

/// appends the specified item to the end of the linked list
void tf_list_push(tf_list_ref list, void* data, const tf_deallocator_t autorelease);

/// enters iteration mode and 'rewinds' the iterator to the first available item in the
/// list
void tf_list_rewind(tf_list_ref list);
/// (for use in a while-loop) iterates to the next available item in the list. Returns
/// 'false' if we are at the end of the list, otherwise returns 'true' and the matching
/// item.
bool tf_list_get_next(tf_list_ref list, void** whereTo);

void tf_list_dump(tf_list_ref list, void* whereTo, const bool strings);

/// releases the linked list and, if necessary, all of its items
void tf_list_release(tf_list_ref list);

/// C string builder object
typedef struct tf_stringbuilder_s* tf_stringbuilder_ref;

tf_stringbuilder_ref tf_stringbuilder_init(void);
tf_stringbuilder_ref tf_stringbuilder_init_with_string(const char* initial);

/// returns the first character in the string buffer
char tf_stringbuilder_get_first(const tf_stringbuilder_ref builder);
/// returns the last character in the string buffer
char tf_stringbuilder_get_last(const tf_stringbuilder_ref builder);

/// returns current string length
tf_index_t tf_stringbuilder_get_length(const tf_stringbuilder_ref builder);
/// returns the resulting C string without detaching it from the string builder
const char* tf_stringbuilder_get_current(const tf_stringbuilder_ref builder);

/// appends the specified character to the end
void tf_stringbuilder_add_character(tf_stringbuilder_ref builder, const char what);
/// removes the last N characters from the string ending
void tf_stringbuilder_remove_last_characters(tf_stringbuilder_ref builder, const tf_index_t count);

/// appends the specified string at the end
void tf_stringbuilder_concat(tf_stringbuilder_ref builder, const char* ending);
/// returns the resulting C string and clears string builder
char* tf_stringbuilder_finish(tf_stringbuilder_ref builder);

bool tf_stringbuilder_ends_with(const tf_stringbuilder_ref builder, const char* ending);

void tf_stringbuilder_release(tf_stringbuilder_ref builder);

/// process object
typedef struct tf_process_s* tf_process_ref;

tf_process_ref tf_process_init_with_argc_argv(const tf_index_t argc, const char** argv);
tf_process_ref tf_process_init_with_arguments(tf_list_ref arguments);

void tf_process_release(tf_process_ref process);

//
// misc C stuff
//

#define TF_SET_IF_NOT_NULL(aa, bb) \
{ \
    if (aa) \
        (*aa) = bb; \
}

/// zero malloc
void* tf_malloc(const tf_index_t size);
/// zero struct malloc (used mostly internally)
#define tf_tmalloc(type) tf_malloc(sizeof(struct type))
