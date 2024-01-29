#ifndef _MAP_H
#define _MAP_H

#define MAP_INITIAL_SIZE   16
#define MAP_PARAM_ERROR    -1
#define MAP_UNINITIALIZED  -2
#define MAP_MALLOC_ERROR   -3
#define MAP_KEY_ERROR      -4
#define MAP_OK              0


struct MAP_OBJECT {
    void *key;
    void *value;
    unsigned int key_size;
    unsigned int value_size;
    struct MAP_OBJECT *ptr;
};

struct MAP {
    struct MAP_OBJECT *objects;
    unsigned int length;
    unsigned int count;
};

// Add object into 'map'. If map is not initialized, initialize it with default size
int map_add(struct MAP *map, const void *key, unsigned int key_size, const void *value, unsigned int value_size);

// Get object from 'map' by 'key'
int map_get(struct MAP *map, const void *key, unsigned int key_size, void *value);

// Init 'map' with initial length of 'length'
int map_init(struct MAP *map, unsigned int length);

// Destroy 'map'
int map_destroy(struct MAP *map);

#endif