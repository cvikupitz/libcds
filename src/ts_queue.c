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
#include "queue.h"
#include "ts_queue.h"

/*
 * Struct for the thread-safe queue.
 */
struct ts_queue {
    pthread_mutex_t lock;       /* The lock */
    Queue *instance;            /* Internal instance of Queue */
};

/* Macro used for locking the queue */
#define LOCK(x)    pthread_mutex_lock( &((x)->lock) )
/* Macro used for unlocking the queue */
#define UNLOCK(x)  pthread_mutex_unlock( &((x)->lock) )

Status ts_queue_new(ConcurrentQueue **queue) {

    ConcurrentQueue *temp;
    Status status;
    pthread_mutexattr_t attr;

    /* Allocates memory for the new queue */
    temp = (ConcurrentQueue *)malloc(sizeof(ConcurrentQueue));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Creates the internal queue instance */
    status = queue_new(&(temp->instance));
    if (status != STAT_SUCCESS) {
        free(temp);
        return status;
    }

    /* Creates pthread_mutex for locking */
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&(temp->lock), &attr);
    pthread_mutexattr_destroy(&attr);
    *queue = temp;

    return STAT_SUCCESS;
}

void ts_queue_lock(ConcurrentQueue *queue) {
    LOCK(queue);
}

void ts_queue_unlock(ConcurrentQueue *queue) {
    UNLOCK(queue);
}

Status ts_queue_add(ConcurrentQueue *queue, void *item) {

    LOCK(queue);
    Status status = queue_add(queue->instance, item);
    UNLOCK(queue);

    return status;
}

Status ts_queue_peek(ConcurrentQueue *queue, void **first) {

    LOCK(queue);
    Status status = queue_peek(queue->instance, first);
    UNLOCK(queue);

    return status;
}

Status ts_queue_poll(ConcurrentQueue *queue, void **first) {

    LOCK(queue);
    Status status = queue_poll(queue->instance, first);
    UNLOCK(queue);

    return status;
}

void ts_queue_clear(ConcurrentQueue *queue, void (*destructor)(void *)) {

    LOCK(queue);
    queue_clear(queue->instance, destructor);
    UNLOCK(queue);
}

long ts_queue_size(ConcurrentQueue *queue) {

    LOCK(queue);
    long size = queue_size(queue->instance);
    UNLOCK(queue);

    return size;
}

Boolean ts_queue_isEmpty(ConcurrentQueue *queue) {

    LOCK(queue);
    Boolean isEmpty = queue_isEmpty(queue->instance);
    UNLOCK(queue);

    return isEmpty;
}

Status ts_queue_toArray(ConcurrentQueue *queue, Array **array) {

    LOCK(queue);
    Status status = queue_toArray(queue->instance, array);
    UNLOCK(queue);

    return status;
}

Status ts_queue_iterator(ConcurrentQueue *queue, ConcurrentIterator **iter) {

    Array *array;
    Status status;

    /* Creates array of items and locks it */
    LOCK(queue);
    status = queue_toArray(queue->instance, &array);
    if (status != STAT_SUCCESS) {
        UNLOCK(queue);
        return status;
    }

    /* Creates the iterator */
    status = ts_iterator_new(iter, &(queue->lock), array->items, array->len);
    if (status != STAT_SUCCESS) {
        FREE_ARRAY(array);
        UNLOCK(queue);
    } else {
        free(array);
    }

    return status;
}

void ts_queue_destroy(ConcurrentQueue *queue, void (*destructor)(void *)) {

    LOCK(queue);
    queue_destroy(queue->instance, destructor);
    UNLOCK(queue);
    pthread_mutex_destroy(&(queue->lock));
    free(queue);
}
