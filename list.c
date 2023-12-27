#include <stdlib.h>
#include "list.h"

int list_add(struct LIST **list, const struct LIST *object) {
    if(list == NULL|| object == NULL) {
        return LIST_PARAM_ERROR;
    }

    if(*list == NULL) {
        *list = (struct LIST *)malloc(sizeof(struct LIST));
        if(*list == NULL) {
            return LIST_ALLOC_ERROR;
        }

        **list = *object;
        (*list)->next = NULL;
        (*list)->prev = *list;

        return LIST_SUCCESS;
    }
    else {
        (*list)->prev->next = (struct LIST *)malloc(sizeof(struct LIST));
        if((*list)->prev->next == NULL) {
            return LIST_ALLOC_ERROR;
        }

        *(*list)->prev->next = *object;
        (*list)->prev->next->prev = (*list)->prev;
        (*list)->prev->next->next = NULL;
        (*list)->prev = (*list)->prev->next;

        return LIST_SUCCESS;
    }
}

int list_insert(struct LIST **list, const struct LIST *object, int position) {
    if(object == NULL || list == NULL) {
        return LIST_PARAM_ERROR;
    }
    if(*list == NULL) {
        return LIST_IS_EMPTY;
    }

    struct LIST *list_iter = *list;
    int absposition = abs(position);
    while(absposition--) {
        if(position >= 0) {
            list_iter = list_iter->next;
        }
        else {
            list_iter = list_iter->prev;
        }
        if(list_iter == NULL || list_iter == *list) {
            return LIST_OUT_OF_RANGE;
        }
    }
    struct LIST *new = (struct LIST *)malloc(sizeof(struct LIST));
    if(new == NULL) {
        return LIST_ALLOC_ERROR;
    }

    *new = *object;
    if(list_iter != *list) {
        list_iter->prev->next = new;
    }

    new->prev = list_iter->prev;
    new->next = list_iter;
    list_iter->prev = new;
    *list = list_iter == *list ? new : *list;

    return LIST_SUCCESS;
}

int list_get_node(struct LIST **list, struct LIST *object, int n) {
    if(list == NULL || object == NULL) {
        return LIST_PARAM_ERROR;
    }
    if(*list == NULL) {
        return LIST_IS_EMPTY;
    }

    struct LIST *list_iter = *list;
    int absn = abs(n);
    while(absn--) {
        if(n >= 0) {
            list_iter = list_iter->next;
        }
        else {
            list_iter = list_iter->prev;
        }
        if(list_iter == NULL || list_iter == *list) {
            return LIST_OUT_OF_RANGE;
        }
    }

    *object = *list_iter;

    return LIST_SUCCESS;
}

int list_del(struct LIST **list, int n) {
    if(list == NULL) {
        return LIST_PARAM_ERROR;
    }
    if(*list == NULL) {
        return LIST_IS_EMPTY;
    }

    struct LIST *list_iter = *list;
    int absn = abs(n);
    while(absn--) {
        if(n >= 0) {
            list_iter = list_iter->next;
        }
        else {
            list_iter = list_iter->prev;
        }
        if(list_iter == NULL || list_iter == *list) {
            return LIST_OUT_OF_RANGE;
        }
    }
    if(list_iter == *list && list_iter->next) {
        *list = list_iter->next;
    }
    else {
        list_iter->prev->next = list_iter->next;
    }

    if(list_iter->next) {
        list_iter->next->prev = list_iter->prev;
    }
    else {
        (*list)->prev = list_iter->prev;
    }
    free(list_iter);
    *list = list_iter == *list ? NULL : *list;

    return LIST_SUCCESS;
}


int list_destroy(struct LIST **list) {
    if(list == NULL) {
        return LIST_PARAM_ERROR;
    }
    if(*list == NULL) {
        return LIST_IS_EMPTY;
    }

    struct LIST *next;
    while(*list) {
        next = (*list)->next;
        free(*list);
        *list = next;
    }

    return LIST_SUCCESS;
}
