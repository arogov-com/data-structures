// Copyright (C) 2024 Aleksei Rogov <alekzzzr@gmail.com>. All rights reserved.

#ifndef _LIST_H
#define _LIST_H

#define LIST_PARAM_ERROR -1
#define LIST_ALLOC_ERROR -2
#define LIST_IS_EMPTY -3
#define LIST_OUT_OF_RANGE -4
#define LIST_UNCHANGED -5
#define LIST_SUCCESS 1

struct LIST {
    int field;
    struct LIST *next;
    struct LIST *prev;
};

// Create a new linked list if '*list' is NULL, and append an 'object' to it
// Otherwise, add a new 'object' to the end of the 'list'
// Return MT_LIST_SUCCESS if no errors
int list_add(struct LIST **list, const struct LIST *object);

// Insert 'object' into 'list' at position 'position'
// Return MT_LIST_SUCCESS if no errors
int list_insert(struct LIST **list, const struct LIST *object, int position);

// Copy node number 'n' to 'object'
// Return MT_LIST_SUCCESS if no errors
int list_get_node(struct LIST **list, struct LIST *object, int n);

// Remove object number 'n' from the 'list'
// Return MT_LIST_SUCCESS if no errors
int list_del(struct LIST **list, int n);

// Destroy 'list'
// Return MT_LIST_SUCCESS if no errors
int list_destroy(struct LIST **list);

#endif
