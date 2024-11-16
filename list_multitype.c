// Copyright (C) 2024 Aleksei Rogov <alekzzzr@gmail.com>. All rights reserved.

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "list_multitype.h"

int mt_list_add(void **list, const void *object, unsigned int object_size) {
    if(object == NULL || object_size == 0 || list == NULL) {
        return MT_LIST_PARAM_ERROR;
    }

    if(*list == NULL) {
        *list = malloc(sizeof(struct LIST_HEADER) + object_size);
        if(*list == NULL) {
            return MT_LIST_ALLOC_ERROR;
        }

        ((struct LIST_HEADER *)*list)->next = NULL;
        ((struct LIST_HEADER *)*list)->prev = *list;
        memcpy(((struct LIST_HEADER *)*list)->data, object, object_size);

        return MT_LIST_SUCCESS;
    }
    else {
        ((struct LIST_HEADER *)*list)->prev->next = malloc(sizeof(struct LIST_HEADER) + object_size);
        if(((struct LIST_HEADER *)*list)->prev->next == NULL) {
            return MT_LIST_ALLOC_ERROR;
        }

        memcpy((((struct LIST_HEADER *)*list)->prev->next)->data, object, object_size);
        ((struct LIST_HEADER *)*list)->prev->next->prev = ((struct LIST_HEADER *)*list)->prev;
        ((struct LIST_HEADER *)*list)->prev->next->next = NULL;
        ((struct LIST_HEADER *)*list)->prev = ((struct LIST_HEADER *)*list)->prev->next;

        return MT_LIST_SUCCESS;
    }
}

int mt_list_insert(void **list, const void *object, unsigned int object_size, int position) {
    if(object == NULL || object_size == 0 || list == NULL) {
        return MT_LIST_PARAM_ERROR;
    }
    if(*list == NULL) {
        return MT_LIST_IS_EMPTY;
    }

    void *list_iter = *list;
    int absposition = abs(position);
    while(absposition--) {
        if(position >= 0) {
            list_iter = ((struct LIST_HEADER *)list_iter)->next;
        }
        else {
            list_iter = ((struct LIST_HEADER *)list_iter)->prev;
        }
        if(list_iter == NULL || list_iter == *list) {
            return MT_LIST_OUT_OF_RANGE;
        }
    }

    void *new = malloc(sizeof(struct LIST_HEADER) + object_size);
    if(new == NULL) {
        return MT_LIST_UNCHANGED;
    }

    memcpy(new + sizeof(struct LIST_HEADER), object, object_size);
    if(list_iter != *list) {
        ((struct LIST_HEADER *)list_iter)->prev->next = new;
    }

    ((struct LIST_HEADER *)new)->prev = ((struct LIST_HEADER *)list_iter)->prev;
    ((struct LIST_HEADER *)new)->next = list_iter;
    ((struct LIST_HEADER *)list_iter)->prev = new;
    *list = list_iter == *list ? new : *list;

    return MT_LIST_SUCCESS;
}

int mt_list_get(void **list, int n, void *object, unsigned int object_size) {
    if(list == NULL || object == NULL || object_size == 0) {
        return MT_LIST_PARAM_ERROR;
    }
    if(*list == NULL) {
        return MT_LIST_IS_EMPTY;
    }

    void *list_iter = *list;
    int absn = abs(n);
    while(absn--) {
        if(n >= 0) {
            list_iter = ((struct LIST_HEADER *)list_iter)->next;
        }
        else {
            list_iter = ((struct LIST_HEADER *)list_iter)->prev;
        }
        if(list_iter == NULL || list_iter == *list) {
            return MT_LIST_OUT_OF_RANGE;
        }
    }

    memcpy(object, ((struct LIST_HEADER *)list_iter)->data, object_size);

    return MT_LIST_SUCCESS;
}

int mt_list_del(void **list, int n) {
    if(list == NULL) {
        return MT_LIST_PARAM_ERROR;
    }
    if(*list == NULL) {
        return MT_LIST_IS_EMPTY;
    }

    void *list_iter = *list;
    int absn = abs(n);
    while(absn--) {
        if(n >= 0) {
            list_iter = ((struct LIST_HEADER *)list_iter)->next;
        }
        else {
            list_iter = ((struct LIST_HEADER *)list_iter)->prev;
        }
        if(list_iter == NULL || list_iter == *list) {
            return MT_LIST_OUT_OF_RANGE;
        }
    }
    if(list_iter == *list && ((struct LIST_HEADER *)list_iter)->next) {
        *list = ((struct LIST_HEADER *)list_iter)->next;
    }
    else {
        ((struct LIST_HEADER *)list_iter)->prev->next = ((struct LIST_HEADER *)list_iter)->next;
    }

    if(((struct LIST_HEADER *)list_iter)->next) {
        ((struct LIST_HEADER *)list_iter)->next->prev = ((struct LIST_HEADER *)list_iter)->prev;
    }
    else {
        ((struct LIST_HEADER *)*list)->prev = ((struct LIST_HEADER *)list_iter)->prev;
    }
    free(list_iter);
    list_iter = list_iter == NULL ? NULL : *list;

    return MT_LIST_SUCCESS;
}

int mt_list_destroy(void **list) {
    if(list == NULL) {
        return MT_LIST_PARAM_ERROR;
    }
    if(*list == NULL) {
        return MT_LIST_IS_EMPTY;
    }

    void *next;
    while(*list) {
        next = ((struct LIST_HEADER *)*list)->next;
        free(*list);
        *list = next;
    }

    return MT_LIST_SUCCESS;
}
