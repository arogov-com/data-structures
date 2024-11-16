// Copyright (C) 2024 Aleksei Rogov <alekzzzr@gmail.com>. All rights reserved.

#ifndef _STACK_H
#define _STACK_H

#define STACK_ERROR        -1
#define STACK_IS_BROKEN    -2
#define STACK_IS_FULL      -3
#define STACK_IS_EMPTY     -4
#define STACK_UNCHANGED    -5
#define STACK_PARAM_ERROR  -6
#define STACK_ALLOC_ERROR  -7
#define STACK_SIZE_ERROR   -8
#define STACK_SUCCESS       1

struct STACK {
    void *ptr;                 // Pointer to stack data
    unsigned int length;       // Number of stack records
    unsigned int object_size;  // Size of stack record
    unsigned int current;      // Current record
};

struct MT_STACK {
    int stack_size;        // Stack size in bytes
    int stack_length;      // Stack length
    int object_size;       // Object size
    int prev_object_size;  // Previous object's size
};


// Initialize a new stack
// Return stack pointer
struct STACK *stack_init(unsigned int length, unsigned int object_size);

// Change 'stack' 'length'. Return the new stack size in bytes
// If the current stack position is outside the new stack's memory, set it to the last position in the new stack
// Return STACK_SUCCESS if no errors
int stack_reinit(struct STACK *stack, unsigned int length);

// Delete 'stack' structure and data
// Return STACK_SUCCESS if no errors
int stack_del(struct STACK *stack);

// Push 'object' onto the 'stack'
// Return remaining capacity or error code in case of error
int stack_push(struct STACK *stack, const void *object);

// Pop an 'object' from the 'stack'
// Return remaining capacity or error code in case of error
int stack_pop(struct STACK *stack, void *object);


// Push an object of size 'size' onto the 'stack'
// Return STACK_SUCCESS if no errors
int mt_stack_push(void **stack, const void *object, unsigned int size);

// Pop an object from the 'stack' and copy it into the 'object'
// Return STACK_SUCCESS if no errors
int mt_stack_pop(void **stack, void *object, unsigned int size);

// Return the size of the 'stack' in bytes, or a value less than 0 on error.
int mt_stack_size(const void **stack);

// Return length of the 'stack' in entries, or value less than 0 if error
int mt_stack_length(void **stack);

// Destroy 'stack'
// Return NULL
int mt_stack_destroy(void **stack);

#endif