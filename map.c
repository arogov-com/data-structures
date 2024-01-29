#include <stdlib.h>
#include <string.h>
#include "map.h"

// Calculate DJB2 hash
static unsigned djb2_hash(const void *data, unsigned int len) {
    unsigned hash = 5381;
    while(len--) {
        hash = ((hash << 5) + hash) ^ *((char *)data);
        ++data;
    }
    return hash;
}

// Recalculate map
static int map_recalc(struct MAP *map) {
    if(map == NULL) {
        return MAP_PARAM_ERROR;
    }
    if(map->objects == NULL || map->length == 0) {
        return MAP_UNINITIALIZED;
    }

    unsigned int new_length = map->length << 1;

    struct MAP_OBJECT *new_obj = malloc(sizeof(struct MAP_OBJECT) * new_length);
    if(new_obj == NULL) {
        return MAP_MALLOC_ERROR;
    }

    for(int i = 0; i != map->length; ++i) {
        // Check if old array has object at position 'i'
        if(map->objects[i].key) {
            struct MAP_OBJECT *obj = &map->objects[i];
            // Iterate objects in nested linked list
            do {
                unsigned int hash = djb2_hash(obj->key, obj->key_size) % new_length;
                // If new objects array has key at position 'hash'
                if(new_obj[hash].key) {
                    struct MAP_OBJECT *inner_obj = &new_obj[hash];
                    // Go to the last nested object in list
                    while(inner_obj->ptr) {
                        inner_obj = inner_obj->ptr;
                    }

                    inner_obj->ptr = malloc(sizeof(struct MAP_OBJECT));
                    if(inner_obj->ptr == NULL) {
                        // Need to delete all created objects
                        free(new_obj);
                        return MAP_MALLOC_ERROR;
                    }
                    *inner_obj->ptr = *obj;
                    inner_obj->ptr->ptr = NULL;
                }
                else {
                    new_obj[hash] = *obj;
                    new_obj[hash].ptr = NULL;
                }

                // Free nested object in old array
                if(obj != &map->objects[i]) {
                    free(obj);
                }
                obj = obj->ptr;
            }while(obj);
        }
    }

    free(map->objects);
    map->objects = new_obj;
    map->length = new_length;
    return MAP_OK;
}

int map_init(struct MAP *map, unsigned int length) {
    if(map->objects == NULL && length > MAP_INITIAL_SIZE) {
        map->objects = malloc(sizeof(struct MAP_OBJECT) * length);
        if(map->objects == NULL) {
            return MAP_MALLOC_ERROR;
        }

        memset(map->objects, 0, sizeof(struct MAP_OBJECT) * length);
        map->count = 0;
        map->length = length;
        return MAP_OK;
    }
    return MAP_PARAM_ERROR;
}

int map_add(struct MAP *map, const void *key, unsigned int key_size, const void *value, unsigned int value_size) {
    if(map == NULL || key == NULL || value == NULL || key_size == 0 || value_size == 0) {
        return MAP_PARAM_ERROR;
    }

    // Map is uninitialized. Initialize map
    if(map->objects == NULL) {
        map->objects = malloc(sizeof(struct MAP_OBJECT) * MAP_INITIAL_SIZE);
        if(map->objects == NULL) {
            return MAP_MALLOC_ERROR;
        }

        memset(map->objects, 0, sizeof(struct MAP_OBJECT) * MAP_INITIAL_SIZE);
        map->length = MAP_INITIAL_SIZE;
        map->count = 0;
    }

    // If map filled more than half, double map size
    if(map->count << 1 > map->length) {
        int ret = map_recalc(map);
        if(ret != MAP_OK) {
            return ret;
        }
    }

    unsigned int hash = djb2_hash(key, key_size) % map->length;
    struct MAP_OBJECT *object = &map->objects[hash];
    struct MAP_OBJECT *next;

    if(object->key) {
        do {
            next = object->ptr;
            // Update value
            if(object->key_size == key_size && memcmp(object->key, key, key_size) == 0) {
                // Resize value length
                if(value_size > object->value_size) {
                    void *resized_value = realloc(object->value, value_size);
                    if(resized_value == NULL) {
                        return MAP_MALLOC_ERROR;
                    }
                    object->value = resized_value;
                    object->value_size = value_size;
                }
                memcpy(object->value, value, value_size);

                return MAP_OK;
            }
            object = next ? next : object;
        }while(next);

        object->ptr = malloc(sizeof(struct MAP_OBJECT));
        if(object->ptr == NULL) {
            return MAP_MALLOC_ERROR;
        }
        object = object->ptr;
    }

    object->key = malloc(key_size);
    if(object->key == NULL) {
        return MAP_MALLOC_ERROR;
    }

    object->value = malloc(value_size);
    if(object->value == NULL) {
        free(object->key);
        object->key = NULL;
        return MAP_MALLOC_ERROR;
    }

    object->key_size = key_size;
    object->value_size = value_size;
    object->ptr = NULL;
    memcpy(object->key, key, key_size);
    memcpy(object->value, value, value_size);
    ++map->count;

    return MAP_OK;
}

int map_get(struct MAP *map, const void *key, unsigned int key_size, void *value) {
    if(map == NULL) {
        return MAP_PARAM_ERROR;
    }
    if(map->objects == NULL) {
        return MAP_UNINITIALIZED;
    }

    unsigned int hash = djb2_hash(key, key_size) % map->length;
    // Is nested object
    if(map->objects[hash].ptr) {
        struct MAP_OBJECT *object = &map->objects[hash];
        while(object) {
            if(object->key_size == key_size && memcmp(object->key, key, key_size) == 0) {
                memcpy(value, object->value, object->value_size);
                return MAP_OK;
            }
            object = object->ptr;
        }
        return MAP_KEY_ERROR;
    }
    else {
        if(map->objects[hash].key_size == key_size && memcmp(map->objects[hash].key, key, key_size) == 0) {
            memcpy(value, map->objects[hash].value, map->objects[hash].value_size);
        }
        else {
            return MAP_KEY_ERROR;
        }
    }

    return MAP_OK;
}

int map_destroy(struct MAP *map) {
    if(map == NULL) {
        return MAP_PARAM_ERROR;
    }
    if(map->objects == NULL) {
        return MAP_UNINITIALIZED;
    }

    while(map->length--) {
        if(map->objects[map->length].ptr) {
            struct MAP_OBJECT *obj = map->objects[map->length].ptr;
            struct MAP_OBJECT *next;
            do {
                next = obj->ptr;
                free(obj->key);
                free(obj->value);
                free(obj);
                obj = next;
            }while(next);
        }
        if(map->objects[map->length].key) {
            free(map->objects[map->length].key);
            free(map->objects[map->length].value);
            --map->count;
        }
    }

    free(map->objects);
    map->objects = NULL;
    map->count = 0;
    map->length = 0;
    return MAP_OK;
}
