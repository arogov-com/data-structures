// Copyright (C) 2024 Aleksei Rogov <alekzzzr@gmail.com>. All rights reserved.

#ifndef _ARRAY_H
#define _ARRAY_H

#define ARRAY_INVALID_LEN    -1
#define ARRAY_INVALID_OBJSZ  -2
#define ARRAY_MALLOC_ERROR   -3
#define ARRAY_INVALID_PTR    -4
#define ARRAY_INVALID_INDEX  -5


struct ARRAY_ENTRY {
    void *data;
    size_t size;
};

struct ARRAY {
    struct ARRAY_ENTRY *data;
    size_t current;
    size_t index;
    size_t len;
    size_t size;
};

// Init array 'array'
// Return 0 if success or error code
int array_init(struct ARRAY *array);

// Destroy array 'array'
// Return 0 if success or error code
int array_destroy(struct ARRAY *array);

// Add object 'src' of size 'object_size' to 'array'
// Return 0 if success or error code
int array_append(struct ARRAY *array, const void *src, size_t object_size);

// Put object 'src' of size 'object_size' to 'array' at index 'index'
// Return 0 if success or error code
int array_put(struct ARRAY *array, const void *src, size_t object_size, size_t index);

// Copy array[index] to 'src'. If 'object_size' is not equal array[index] return error
// Return 0 if success or error code
int array_get(const struct ARRAY *array, size_t index, void *dst, size_t object_size);

// Insert 'object' of size 'object_size' to array[index] with shift to right
// Return 0 if success or error code
int array_insert(struct ARRAY *array, size_t index, const void *object, size_t object_size);

// Delete objects in 'array' from 'start' to 'end'
// Return 0 if success or error code
int array_del(struct ARRAY *array, size_t start, size_t end);

// Start iterator at 'index'
// Return 0 if success or error code
int array_get_objects_start(struct ARRAY *array, size_t index);

// Get next object from 'array'
// Return 0 if success or error code
struct ARRAY_ENTRY* array_get_objects_next(struct ARRAY *array);

// Get number of object in the array
// Return number of object in the array
size_t array_len(const struct ARRAY *array);

// Get array size in bytes
// Return array size in bytes
size_t array_size(const struct ARRAY *array);

#endif
