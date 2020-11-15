/**
 * MIT License
 *
 * Copyright (c) 2020 Cole Vikupitz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <pthread.h>
#include <stdlib.h>
#include "boundedstack.h"
#include "ts_boundedstack.h"

/*
 * Struct for the thread-safe bounded stack.
 */
struct ts_bounded_stack {
    pthread_mutex_t lock;       /* The lock */
    BoundedStack *instance;     /* Internal instance of BoundedStack */
};

/* Macro used for locking the stack */
#define LOCK(x)    pthread_mutex_lock( &((x)->lock) )
/* Macro used for unlocking the stack */
#define UNLOCK(x)  pthread_mutex_unlock( &((x)->lock) )

Status ts_boundedstack_new(ConcurrentBoundedStack **stack, long capacity) {

    ConcurrentBoundedStack *temp;
    Status status;
    pthread_mutexattr_t attr;

    /* Allocates memory for the new stack */
    temp = (ConcurrentBoundedStack *)malloc(sizeof(ConcurrentBoundedStack));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Creates the internal stack instance */
    status = boundedstack_new(&(temp->instance), capacity);
    if (status != STAT_SUCCESS) {
        free(temp);
        return status;
    }

    /* Creates the pthread_mutex for locking */
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&(temp->lock), &attr);
    pthread_mutexattr_destroy(&attr);
    *stack = temp;

    return STAT_SUCCESS;
}

void ts_boundedstack_lock(ConcurrentBoundedStack *stack) {
    LOCK(stack);
}

void ts_boundedstack_unlock(ConcurrentBoundedStack *stack) {
    UNLOCK(stack);
}

Status ts_boundedstack_push(ConcurrentBoundedStack *stack, void *item) {

    LOCK(stack);
    Status status = boundedstack_push(stack->instance, item);
    UNLOCK(stack);

    return status;
}

Status ts_boundedstack_peek(ConcurrentBoundedStack *stack, void **top) {

    LOCK(stack);
    Status status = boundedstack_peek(stack->instance, top);
    UNLOCK(stack);

    return status;
}

Status ts_boundedstack_pop(ConcurrentBoundedStack *stack, void **top) {

    LOCK(stack);
    Status status = boundedstack_pop(stack->instance, top);
    UNLOCK(stack);

    return status;
}

void ts_boundedstack_clear(ConcurrentBoundedStack *stack, void (*destructor)(void *)) {

    LOCK(stack);
    boundedstack_clear(stack->instance, destructor);
    UNLOCK(stack);
}

long ts_boundedstack_size(ConcurrentBoundedStack *stack) {

    LOCK(stack);
    long size = boundedstack_size(stack->instance);
    UNLOCK(stack);

    return size;
}

long ts_boundedstack_capacity(ConcurrentBoundedStack *stack) {

    LOCK(stack);
    long capacity = boundedstack_capacity(stack->instance);
    UNLOCK(stack);

    return capacity;
}

Boolean ts_boundedstack_isEmpty(ConcurrentBoundedStack *stack) {

    LOCK(stack);
    Boolean isEmpty = boundedstack_isEmpty(stack->instance);
    UNLOCK(stack);

    return isEmpty;
}

Boolean ts_boundedstack_isFull(ConcurrentBoundedStack *stack) {

    LOCK(stack);
    Boolean isFull = boundedstack_isFull(stack->instance);
    UNLOCK(stack);

    return isFull;
}

Status ts_boundedstack_toArray(ConcurrentBoundedStack *stack, Array **array) {

    LOCK(stack);
    Status status = boundedstack_toArray(stack->instance, array);
    UNLOCK(stack);

    return status;
}

Status ts_boundedstack_iterator(ConcurrentBoundedStack *stack, ConcurrentIterator **iter) {

    Array *array;
    Status status;

    /* Creates the array of items and locks it */
    LOCK(stack);
    status = boundedstack_toArray(stack->instance, &array);
    if (status != STAT_SUCCESS) {
        UNLOCK(stack);
        return status;
    }

    /* Creates the iterator */
    status = ts_iterator_new(iter, &(stack->lock), array->items, array->len);
    if (status != STAT_SUCCESS) {
        FREE_ARRAY(array);
        UNLOCK(stack);
    } else {
        free(array);
    }

    return status;
}

void ts_boundedstack_destroy(ConcurrentBoundedStack *stack, void (*destructor)(void *)) {

    LOCK(stack);
    boundedstack_destroy(stack->instance, destructor);
    UNLOCK(stack);
    pthread_mutex_destroy(&(stack->lock));
    free(stack);
}
