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
#include "boundedqueue.h"
#include "ts_boundedqueue.h"

struct ts_bounded_queue {
    pthread_mutex_t lock;
    BoundedQueue *instance;
};

#define LOCK(x)    pthread_mutex_lock( &((x)->lock) )
#define UNLOCK(x)  pthread_mutex_unlock( &((x)->lock) )

Status ts_boundedqueue_new(ConcurrentBoundedQueue **queue, long capacity) {

    ConcurrentBoundedQueue *temp;
    Status status;
    pthread_mutexattr_t attr;

    temp = (ConcurrentBoundedQueue *)malloc(sizeof(ConcurrentBoundedQueue));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    status = boundedqueue_new(&(temp->instance), capacity);
    if (status != STAT_SUCCESS) {
        free(temp);
        return status;
    }

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&(temp->lock), &attr);
    pthread_mutexattr_destroy(&attr);
    *queue = temp;

    return STAT_SUCCESS;
}

void ts_boundedqueue_lock(ConcurrentBoundedQueue *queue) {
    LOCK(queue);
}

void ts_boundedqueue_unlock(ConcurrentBoundedQueue *queue) {
    UNLOCK(queue);
}

Status ts_boundedqueue_add(ConcurrentBoundedQueue *queue, void *item) {

    LOCK(queue);
    Status status = boundedqueue_add(queue->instance, item);
    UNLOCK(queue);

    return status;
}

Status ts_boundedqueue_peek(ConcurrentBoundedQueue *queue, void **first) {

    LOCK(queue);
    Status status = boundedqueue_peek(queue->instance, first);
    UNLOCK(queue);

    return status;
}

Status ts_boundedqueue_poll(ConcurrentBoundedQueue *queue, void **first) {

    LOCK(queue);
    Status status = boundedqueue_poll(queue->instance, first);
    UNLOCK(queue);

    return status;
}

void ts_boundedqueue_clear(ConcurrentBoundedQueue *queue, void (*destructor)(void *)) {

    LOCK(queue);
    boundedqueue_clear(queue->instance, destructor);
    UNLOCK(queue);
}

long ts_boundedqueue_size(ConcurrentBoundedQueue *queue) {

    LOCK(queue);
    long size = boundedqueue_size(queue->instance);
    UNLOCK(queue);

    return size;
}

long ts_boundedqueue_capacity(ConcurrentBoundedQueue *queue) {

    LOCK(queue);
    long capacity = boundedqueue_capacity(queue->instance);
    UNLOCK(queue);

    return capacity;
}

Boolean ts_boundedqueue_isEmpty(ConcurrentBoundedQueue *queue) {

    LOCK(queue);
    Boolean isEmpty = boundedqueue_isEmpty(queue->instance);
    UNLOCK(queue);

    return isEmpty;
}

Boolean ts_boundedqueue_isFull(ConcurrentBoundedQueue *queue) {

    LOCK(queue);
    Boolean isFull = boundedqueue_isFull(queue->instance);
    UNLOCK(queue);

    return isFull;
}

Status ts_boundedqueue_toArray(ConcurrentBoundedQueue *queue, Array **array) {

    LOCK(queue);
    Status status = boundedqueue_toArray(queue->instance, array);
    UNLOCK(queue);

    return status;
}

Status ts_boundedqueue_iterator(ConcurrentBoundedQueue *queue, ConcurrentIterator **iter) {

    Array *array;
    Status status;

    LOCK(queue);
    status = boundedqueue_toArray(queue->instance, &array);
    if (status != STAT_SUCCESS) {
        UNLOCK(queue);
        return status;
    }

    status = ts_iterator_new(iter, &(queue->lock), array->items, array->len);
    if (status != STAT_SUCCESS) {
        FREE_ARRAY(array);
        UNLOCK(queue);
    } else {
        free(array);
    }

    return status;
}

void ts_boundedqueue_destroy(ConcurrentBoundedQueue *queue, void (*destructor)(void *)) {

    LOCK(queue);
    boundedqueue_destroy(queue->instance, destructor);
    UNLOCK(queue);
    pthread_mutex_destroy(&(queue->lock));
    free(queue);
}
