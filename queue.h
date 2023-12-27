#include <stdint.h>

#ifndef _QUEUE_H
#define _QUEUE_H

#define QUEUE_SUCCESS       0
#define QUEUE_PARAM_ERROR  -1
#define QUEUE_ALLOC_ERROR  -2
#define QUEUE_SIZE_ERROR   -3
#define QUEUE_IS_EMPTY     -4


struct MT_QUEUE {
    struct MT_QUEUE *next;  // Pointer to next queue entry
    struct MT_QUEUE *last;  // Pointer to last queue entry
    uint32_t object_size;   // Entry size in bytes
    uint32_t queue_length;  // Queue length
};


// Create a 'queue' and add an 'object' to it if '*queue' is NULL, or add an 'object' to an existing queue otherwise
// Return QUEUE_SUCCESS if no errors
int mt_queue_push(void **queue, void *object, unsigned int object_size);

// Get an object from the beginning of the 'queue' and remove it
// Return QUEUE_SUCCESS if no errors
int mt_queue_pop(void **queue, void *object, unsigned int object_size);

// Destroy queue
// Return QUEUE_SUCCESS if no errors
int mt_qeue_destroy(void **queue);

// Access first element
// Return QUEUE_SUCCESS if no errors
int mt_queue_front(void **queue, void *object, unsigned int object_size);

// Access last element
// Return QUEUE_SUCCESS if no errors
int mt_queue_back(void **queue, void *object, unsigned int object_size);

// Get queue length
// Return QUEUE_SUCCESS if no errors
int mt_queue_length(void **queue);

#endif
