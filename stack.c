// Copyright (C) 2024 Aleksei Rogov <alekzzzr@gmail.com>. All rights reserved.

#include <string.h>
#include <stdlib.h>
#include "stack.h"

struct STACK *stack_init(unsigned int length, unsigned int object_size) {
    if(length == 0 || object_size == 0) {
        return NULL;
    }

    struct STACK *stack = malloc(sizeof(struct STACK));
    if(stack == NULL) {
        return NULL;
    }

    stack->ptr = malloc(length * object_size);
    if(stack->ptr == NULL) {
        free(stack);
        return NULL;
    }

    stack->length = length;
    stack->object_size = object_size;
    stack->current = 0;

    return stack;
}

int stack_reinit(struct STACK *stack, unsigned int length) {
    if(stack == NULL) {
        return STACK_ERROR;
    }
    if(length == 0) {
        return STACK_PARAM_ERROR;
    }
    if(stack->ptr == NULL) {
        return STACK_IS_BROKEN;
    }

    void *tmp = realloc(stack->ptr, length * stack->object_size);
    if(tmp == NULL) {
        return STACK_UNCHANGED;
    }

    if(stack->current > length) {
        stack->current = length;
    }

    stack->ptr = tmp;
    stack->length = length;

    return length * stack->object_size;
}

int stack_del(struct STACK *stack) {
    if(stack == NULL) {
        return STACK_ERROR;
    }

    if(stack->ptr == NULL) {
        free(stack);
        return STACK_IS_BROKEN;
    }

    free(stack->ptr);
    free(stack);

    return STACK_SUCCESS;
}

int stack_push(struct STACK *stack, const void *object) {
    if(stack == NULL) {
        return STACK_ERROR;
    }
    if(stack->ptr == NULL) {
        return STACK_IS_BROKEN;
    }
    if(stack->length == stack->current) {
        return STACK_IS_FULL;
    }
    if(object == NULL) {
        return STACK_PARAM_ERROR;
    }

    memcpy(stack->ptr + stack->current * stack->object_size, object, stack->object_size);
    ++stack->current;

    return stack->length - stack->current;
}

int stack_pop(struct STACK *stack, void *object) {
    if(stack == NULL) {
        return STACK_ERROR;
    }
    if(stack->ptr == NULL) {
        return STACK_IS_BROKEN;
    }
    if(stack->current == 0) {
        return STACK_IS_EMPTY;
    }
    if(object == NULL) {
        return STACK_PARAM_ERROR;
    }

    memcpy(object, stack->ptr + stack->object_size * (stack->current - 1), stack->object_size);
    --stack->current;

    return stack->length - stack->current;
}

int mt_stack_push(void **stack, const void *object, unsigned int size) {
    if(object == NULL || size == 0) {
        return STACK_PARAM_ERROR;
    }

    struct MT_STACK *hdr = *stack;
    if(*stack == NULL) {
        *stack = malloc(sizeof(struct MT_STACK) + size);
        if(*stack == NULL) {
            return STACK_ALLOC_ERROR;
        }
        hdr = *stack;
        hdr->object_size = size;
        hdr->stack_length = 1;
        hdr->stack_size = size + sizeof(struct MT_STACK);
        hdr->prev_object_size = 0;
        memcpy((*stack) + sizeof(struct MT_STACK), object, size);

        return STACK_SUCCESS;
    }
    else {
        int stack_size = hdr->stack_size;
        int stack_length = hdr->stack_length;
        int prev_object_size = hdr->object_size;

        void *stack_save = *stack;
        *stack = realloc((*stack) - (hdr->stack_size - hdr->object_size - sizeof(struct MT_STACK)), hdr->stack_size + size + sizeof(struct MT_STACK));
        if(*stack == NULL) {
            *stack = stack_save;

            return STACK_UNCHANGED;
        }

        (*stack) += stack_size;
        memcpy((*stack) + sizeof(struct MT_STACK), object, size);
        hdr = *stack;
        hdr->object_size = size;
        hdr->stack_length = stack_length + 1;
        hdr->stack_size = stack_size + size + sizeof(struct MT_STACK);
        hdr->prev_object_size = prev_object_size;

        return STACK_SUCCESS;
    }
}

int mt_stack_pop(void **stack, void *object, unsigned int size) {
    if(stack == NULL || object == NULL || size == 0) {
        return STACK_PARAM_ERROR;
    }
    if(*stack == NULL) {
        return STACK_IS_EMPTY;
    }

    struct MT_STACK *hdr = *stack;
    if(hdr->object_size != size) {
        return STACK_SIZE_ERROR;
    }

    memcpy(object, (*stack) + sizeof(struct MT_STACK), size);

    struct MT_STACK tmp = *hdr;
    *stack = realloc((*stack) - hdr->stack_size + hdr->object_size + sizeof(struct MT_STACK), hdr->stack_size - hdr->object_size - sizeof(struct MT_STACK));

    if(*stack != NULL) {
        *stack = (*stack) + tmp.stack_size - tmp.object_size - sizeof(struct MT_STACK) - tmp.prev_object_size - sizeof(struct MT_STACK);

        return STACK_SUCCESS;
    }

    return STACK_SUCCESS;
}

int mt_stack_size(const void **stack) {
    if(stack == NULL) {
        return STACK_PARAM_ERROR;
    }
    if(*stack == NULL) {
        return 0;
    }

    const struct MT_STACK *hdr = *stack;

    return hdr->stack_size;
}

int mt_stack_length(void **stack) {
    if(stack == NULL) {
        return STACK_PARAM_ERROR;
    }
    if(*stack == NULL) {
        return 0;
    }

    const struct MT_STACK *hdr = *stack;

    return hdr->stack_length;
}

int mt_stack_destroy(void **stack) {
    if(stack == NULL) {
        return STACK_PARAM_ERROR;
    }
    if(*stack == NULL) {
        return STACK_IS_EMPTY;
    }

    struct MT_STACK *hdr = *stack;
    free((*stack) - hdr->stack_size + hdr->object_size);

    return 1;
}
