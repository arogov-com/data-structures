// Copyright (C) 2024 Aleksei Rogov <alekzzzr@gmail.com>. All rights reserved.

#ifndef _LIST_MULTITYPE_H
#define _LIST_MULTITYPE_H

#define MT_LIST_PARAM_ERROR -1
#define MT_LIST_ALLOC_ERROR -2
#define MT_LIST_IS_EMPTY -3
#define MT_LIST_OUT_OF_RANGE -4
#define MT_LIST_UNCHANGED -5
#define MT_LIST_SUCCESS 1

struct LIST_HEADER {
    struct LIST_HEADER *prev;
    struct LIST_HEADER *next;
    char data[];
};

// Create a new linked list if '*list' is NULL, and append an 'object' of size 'object_size' to it.
// Otherwise, add a new object of size "object_size" to the end of the list.
// Return MT_LIST_SUCCESS if no errors
int mt_list_add(void **list, const void *object, unsigned int object_size);

// Insert 'object' into 'list' at position 'position'
// Return MT_LIST_SUCCESS if no errors
int mt_list_insert(void **list, const void *object, unsigned int object_size, int position);

// Copy node number 'n' to 'object'
// Return MT_LIST_SUCCESS if no errors
int mt_list_get(void **list, int n, void *object, unsigned int object_size);

// Remove object number 'n' from the 'list'
// Return MT_LIST_SUCCESS if no errors
int mt_list_del(void **list, int n);

// Destroy 'list'
// Return MT_LIST_SUCCESS if no errors
int mt_list_destroy(void **list);

#endif
