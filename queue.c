// Copyright (C) 2024 Aleksei Rogov <alekzzzr@gmail.com>. All rights reserved.

#include <string.h>
#include <stdlib.h>
#include "queue.h"

int mt_queue_push(void **queue, void *object, size_t object_size) {
    if(object == NULL || object_size == 0 || queue == NULL) {
        return QUEUE_PARAM_ERROR;
    }

    if(*queue == NULL) {
        struct MT_QUEUE *mt_queue = malloc(sizeof(struct MT_QUEUE) + object_size);
        if(mt_queue == NULL) {
            return QUEUE_ALLOC_ERROR;
        }

        mt_queue->last = mt_queue;
        mt_queue->next = NULL;
        mt_queue->object_size = object_size;
        mt_queue->queue_length = 1;
        memcpy((void *)mt_queue + sizeof(struct MT_QUEUE), object, object_size);
        *queue = mt_queue;

        return QUEUE_SUCCESS;
    }
    else {
        struct MT_QUEUE *mt_queue = *queue;
        mt_queue->last->next = malloc(sizeof(struct MT_QUEUE) + object_size);
        if(mt_queue->last->next == NULL) {
            return QUEUE_ALLOC_ERROR;
        }

        uint32_t length = mt_queue->last->queue_length + 1;
        mt_queue->last->next->next = NULL;
        mt_queue->last->next->object_size = object_size;
        mt_queue->last->next->last = NULL;
        mt_queue->last = mt_queue->last->next;
        mt_queue->last->queue_length = length;
        memcpy((void *)mt_queue->last + sizeof(struct MT_QUEUE), object, object_size);

        return QUEUE_SUCCESS;
    }
}

int mt_queue_pop(void **queue, void *object, size_t object_size) {
    if(queue == NULL || object == NULL || object_size == 0) {
        return QUEUE_PARAM_ERROR;
    }
    if(*queue == NULL) {
        return QUEUE_IS_EMPTY;
    }

    struct MT_QUEUE *mt_queue = *queue;
    if(mt_queue->object_size != object_size) {
        return QUEUE_SIZE_ERROR;
    }

    uint32_t length = mt_queue->last->queue_length - 1;
    memcpy(object, (*queue) + sizeof(struct MT_QUEUE), object_size);

    struct MT_QUEUE *next = mt_queue->next;
    struct MT_QUEUE *last = mt_queue->last;
    free(*queue);
    if(next == NULL) {
        *queue = NULL;
        return QUEUE_SUCCESS;
    }

    next->last = last;
    *queue = next;
    last->queue_length = length;

    return QUEUE_SUCCESS;
}

int mt_queue_front(void **queue, void *object, size_t object_size) {
    if(queue == NULL) {
        return QUEUE_PARAM_ERROR;
    }
    if(*queue == NULL) {
        return QUEUE_IS_EMPTY;
    }

    struct MT_QUEUE *mt_qeue = *queue;
    if(mt_qeue->object_size != object_size) {
        return QUEUE_SIZE_ERROR;
    }

    memcpy(object, (*queue) + sizeof(struct MT_QUEUE), object_size);

    return QUEUE_SUCCESS;
}

int mt_queue_back(void **queue, void *object, size_t object_size) {
    if(queue == NULL) {
        return QUEUE_PARAM_ERROR;
    }
    if(*queue == NULL) {
        return QUEUE_IS_EMPTY;
    }

    struct MT_QUEUE *mt_qeue = *queue;
    if(mt_qeue->object_size != object_size) {
        return QUEUE_SIZE_ERROR;
    }

    memcpy(object, ((void *)mt_qeue->last) + sizeof(struct MT_QUEUE), object_size);

    return QUEUE_SUCCESS;
}

int mt_queue_length(void **queue) {
    if(queue == NULL) {
        return QUEUE_PARAM_ERROR;
    }
    if(*queue == NULL) {
        return 0;
    }

    struct MT_QUEUE *mt_qeue = *queue;
    return mt_qeue->last->queue_length;
}

int mt_qeue_destroy(void **queue) {
    if(queue == NULL) {
        return QUEUE_PARAM_ERROR;
    }
    if(*queue == NULL) {
        return QUEUE_IS_EMPTY;
    }

    struct MT_QUEUE *mt_queue = *queue;
    while(mt_queue) {
        void *ptr = mt_queue->next;
        free(mt_queue);
        mt_queue = ptr;
    }

    *queue = NULL;

    return QUEUE_SUCCESS;
}
