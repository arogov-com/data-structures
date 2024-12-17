// Copyright (C) 2024 Aleksei Rogov <alekzzzr@gmail.com>. All rights reserved.

#include <stdlib.h>
#include <string.h>
#include "array.h"

int array_init(struct ARRAY *array) {
    if(array == NULL) {
        return ARRAY_INVALID_PTR;
    }

    memset(array, 0, sizeof(struct ARRAY));

    return 0;
}

static int array_resize(struct ARRAY *array, size_t size) {
    void *ptr = realloc(array->data, size * sizeof(struct ARRAY_ENTRY));
    if(ptr == NULL) {
        return ARRAY_MALLOC_ERROR;
    }

    array->data = ptr;
    array->len = size;

    return 0;
}

int array_destroy(struct ARRAY *array) {
    if(array == NULL) {
        return ARRAY_INVALID_PTR;
    }

    if(array->data) {
        for(size_t i = 0; i < array->current; ++i) {
            free(array->data[i].data);
        }
        free(array->data);
    }
    memset(array, 0, sizeof(struct ARRAY));

    return 0;
}

int array_append(struct ARRAY *array, const void *src, size_t object_size) {
    if(array == NULL || src == NULL) {
        return ARRAY_INVALID_PTR;
    }
    if(object_size == 0) {
        return ARRAY_INVALID_OBJSZ;
    }

    if(array->current >= array->len) {
        if(array->data) {
            int err = array_resize(array, array->len << 1);
            if(err != 0) {
                return err;
            }
        }
        else {
            array->data = malloc(sizeof(struct ARRAY_ENTRY));
            if(array->data == NULL) {
                return ARRAY_MALLOC_ERROR;
            }
            array->len = 1;
        }
    }

    array->data[array->current].data = malloc(object_size);
    if(array->data[array->current].data == NULL) {
        return ARRAY_MALLOC_ERROR;
    }
    memcpy(array->data[array->current].data, src, object_size);
    array->data[array->current].size = object_size;
    ++array->current;
    array->size += object_size;

    return 0;
}

int array_put(struct ARRAY *array, const void *src, size_t object_size, size_t index) {
    if(array == NULL || src == NULL) {
        return ARRAY_INVALID_PTR;
    }
    if(object_size == 0) {
        return ARRAY_INVALID_OBJSZ;
    }
    if(index > array->current - 1) {
        return ARRAY_INVALID_INDEX;
    }

    if(array->data[index].size != object_size) {
        void *ptr = malloc(object_size);
        if(ptr == NULL) {
            return ARRAY_MALLOC_ERROR;
        }
        array->size -= array->data[index].size;
        array->size += object_size;
        free(array->data[index].data);
        array->data[index].data = ptr;
    }
    array->data[index].size = object_size;
    memcpy(array->data[index].data, src, object_size);

    return 0;
}

int array_insert(struct ARRAY *array, size_t index, const void *object, size_t object_size) {
    if(array == NULL || array->data == NULL || object == NULL) {
        return ARRAY_INVALID_PTR;
    }
    if(object_size == 0) {
        return ARRAY_INVALID_OBJSZ;
    }

    void *ptr = malloc(object_size);
    if(ptr == NULL) {
        return ARRAY_MALLOC_ERROR;
    }

    if(array->current + 1 >= array->len) {
        array_resize(array, array->len << 1);
    }

    index = index >= array->current ? array->current : index;

    for(size_t i = array->current; i > index; --i) {
        array->data[i] = array->data[i - 1];
    }
    memcpy(ptr, object, object_size);
    array->data[index].data = ptr;
    array->data[index].size = object_size;
    ++array->current;
    array->size += object_size;

    return 0;
}

int array_get(const struct ARRAY *array, size_t index, void *dst, size_t object_size) {
    if(array == NULL || array->data == NULL || dst == NULL) {
        return ARRAY_INVALID_PTR;
    }
    if(index > array->current - 1) {
        return ARRAY_INVALID_INDEX;
    }
    if(array->data[index].size != object_size) {
        return ARRAY_INVALID_OBJSZ;
    }

    memcpy(dst, array->data[index].data, array->data[index].size);

    return 0;
}

int array_del(struct ARRAY *array, size_t start, size_t end) {
    if(array == NULL) {
        return ARRAY_INVALID_PTR;
    }
    if(start > array->current - 1) {
        return ARRAY_INVALID_INDEX;
    }
    if(start > end) {
        return ARRAY_INVALID_INDEX;
    }

    for(size_t i = start; i <= end && i < array->current; ++i) {
        array->size -= array->data[i].size;
        free(array->data[i].data);
        array->data[i].data = NULL;
    }

    for(size_t i = start, j = 0; i < array->current && end + 1 + j < array->current; ++i, ++j) {
        array->data[i] = array->data[end + 1 + j];
    }

    end = end > array->current - 1 ? array->current - 1 : end;
    array->current -= (end - start + 1);
    size_t new_len = 1;
    if(array->current != 0) {
        while(new_len < array->current) {
            new_len = new_len << 1;
        }
    }
    else {
        new_len = 0;
    }

    array_resize(array, new_len);

    if(array->current == 0) {
        array->len = 0;
        array->data = NULL;
    }

    return 0;
}

int array_get_objects_start(struct ARRAY *array, size_t index) {
    if(array == NULL) {
        return ARRAY_INVALID_PTR;
    }
    if(index > array->len) {
        return ARRAY_INVALID_INDEX;
    }

    array->index = index;

    return 0;
}

struct ARRAY_ENTRY* array_get_objects_next(struct ARRAY *array) {
    if(array == NULL) {
        return NULL;
    }
    if(array->index >= array->current) {
        return NULL;
    }
    if(array->data[array->index].data == NULL) {
        return NULL;
    }

    return &array->data[array->index++];
}

size_t array_len(const struct ARRAY *array) {
    if(array == NULL) {
        return ARRAY_INVALID_PTR;
    }

    return array->current;
}

size_t array_size(const struct ARRAY *array) {
    if(array == NULL) {
        return 0;
    }

    return sizeof(struct ARRAY) + array->len * sizeof(struct ARRAY_ENTRY) + array->size;
}
