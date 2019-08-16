#ifndef UTILC_DYNARRAY_H
#define UTILC_DYNARRAY_H

#include <stdlib.h>

/**
 * Creates an dynamic array of type (type) with the typename name.
 * Construct it as empty struct ( = {};, or calloc).
 * Kill it with <name>_kill;
 * <name>_set_capacity sets the internal capacity. If realloc fails, nothing changes.
 * <name>_resize sets the array size and may set capacity with size*2.
 * <name>_push copies an item to the end of the array (copies to the last element).
 * <name>_append creates an item at the end and returns its pointer (returns the last element).
 * <name>_pop returns a copy of the last item and removes it from the array.
 */
#define DynArray(type, name) \
typedef struct { \
    type *array; \
    int capacity; \
    int size; \
} name; \
void name ## _kill(name *self) { \
    free(self->array); \
    self->array = NULL; \
    self->capacity = 0; \
    self->size = 0; \
} \
void name ## _set_capacity(name *self, int capacity) { \
    type *new_array = (type *) realloc(self->array, capacity * sizeof(type)); \
    if (new_array) { \
        self->array = new_array; \
        self->capacity = capacity; \
        if (self->size > capacity) \
            self->size = capacity; \
    } \
} \
void name ## _resize(name *self, int size) { \
    if (size > self->capacity) { \
        name ## _set_capacity(self, size * 2); \
        if(size>self->capacity) \
            /* alloc error */ \
            return;\
    } \
    self->size = size; \
} \
void name ## _push(name *self, type item) { \
    name ## _resize(self, self->size+1); \
    self->array[self->size-1] = item; \
} \
type *name ## _append(name *self) { \
    name ## _resize(self, self->size+1); \
    return &self->array[self->size-1]; \
} \
type name ## _pop(name *self) { \
    if(self->size > 0) \
        return self->array[--self->size]; \
    type t; \
    return t; \
}


/**
 * Creates an dynamic array of type (type) with the typename name.
 * Only the function prototypes will be generated. Use DynArray(type, name) in an source file.
 *
 * Construct it as empty struct ( = {};, or calloc).
 * Kill it with <name>_kill;
 * <name>_set_capacity sets the internal capacity. If realloc fails, nothing changes.
 * <name>_resize sets the array size and may set capacity with size*2.
 * <name>_push copies an item to the end of the array (copies to the last element).
 * <name>_append creates an item at the end and returns its pointer (returns the last element).
 * <name>_pop returns a copy of the last item and removes it from the array.
 */
#define DynArrayHeader(type, name) \
typedef struct { \
    type *array; \
    int capacity; \
    int size; \
} name; \
void name ## _kill(name *self); \
void name ## _set_capacity(name *self, int capacity); \
void name ## _resize(name *self, int size); \
void name ## _push(name *self, type item); \
type *name ## _append(name *self); \
type name ## _pop(name *self);




/**
 * Creates an dynamic array of type (type) with the typename name.
 * This version dont uses copies (push and pop). For use with array types like int[2].
 *
 * Construct it as empty struct ( = {};, or calloc).
 * Kill it with <name>_kill;
 * <name>_set_capacity sets the internal capacity. If realloc fails, nothing changes.
 * <name>_resize sets the array size and may set capacity with size*2.
 * <name>_append creates an item at the end and returns its pointer (returns the last element).
 */
#define DynArrayWithoutCopy(type, name) \
typedef struct { \
    type *array; \
    int capacity; \
    int size; \
} name; \
void name ## _kill(name *self) { \
    free(self->array); \
    self->array = NULL; \
    self->capacity = 0; \
    self->size = 0; \
} \
void name ## _set_capacity(name *self, int capacity) { \
    type *new_array = (type *) realloc(self->array, capacity * sizeof(type)); \
    if (new_array) { \
        self->array = new_array; \
        self->capacity = capacity; \
        if (self->size > capacity) \
            self->size = capacity; \
    } \
} \
void name ## _resize(name *self, int size) { \
    if (size > self->capacity) { \
        name ## _set_capacity(self, size * 2); \
        if(size>self->capacity) \
            /* alloc error */ \
            return;\
    } \
    self->size = size; \
} \
type *name ## _append(name *self) { \
    name ## _resize(self, self->size+1); \
    return &self->array[self->size-1]; \
}


/**
 * Creates an dynamic array of type (type) with the typename name.
 * This version dont uses copies (push and pop). For use with array types like int[2].
 * Only the function prototypes will be generated. Use DynArray(type, name) in an source file.
 *
 * Construct it as empty struct ( = {};, or calloc).
 * Kill it with <name>_kill;
 * <name>_set_capacity sets the internal capacity. If realloc fails, nothing changes.
 * <name>_resize sets the array size and may set capacity with size*2.
 * <name>_append creates an item at the end and returns its pointer (returns the last element).
 */
#define DynArrayWithoutCopyHeader(type, name) \
typedef struct { \
    type *array; \
    int capacity; \
    int size; \
} name; \
void name ## _kill(name *self); \
void name ## _set_capacity(name *self, int capacity); \
void name ## _resize(name *self, int size); \
type *name ## _append(name *self);


#endif //UTILC_DYNARRAY_H
