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
#include "heap.h"
#include "ts_heap.h"

/*
 * Struct for the thread-safe heap.
 */
struct ts_heap {
    pthread_mutex_t lock;       /* The lock */
    Heap *instance;             /* Internal instance of Heap */
};

/* Macro used for locking the heap */
#define LOCK(x)    pthread_mutex_lock( &((x)->lock) )
/* Macro used for unlocking the heap */
#define UNLOCK(x)  pthread_mutex_unlock( &((x)->lock) )

Status ts_heap_new(ConcurrentHeap **heap, long capacity, int (*comparator)(void *, void *)) {

    ConcurrentHeap *temp;
    Status status;
    pthread_mutexattr_t attr;

    /* Allocates memory for the heap */
    temp = (ConcurrentHeap *)malloc(sizeof(ConcurrentHeap));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Creates the internal heap instance */
    status = heap_new(&(temp->instance), capacity, comparator);
    if (status != STAT_SUCCESS) {
        free(temp);
        return status;
    }

    /* Creates the pthread_mutex for locking */
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&(temp->lock), &attr);
    pthread_mutexattr_destroy(&attr);
    *heap = temp;

    return STAT_SUCCESS;
}

void ts_heap_lock(ConcurrentHeap *heap) {
    LOCK(heap);
}

void ts_heap_unlock(ConcurrentHeap *heap) {
    UNLOCK(heap);
}

Status ts_heap_insert(ConcurrentHeap *heap, void *item) {

    LOCK(heap);
    Status status = heap_insert(heap->instance, item);
    UNLOCK(heap);

    return status;
}

Status ts_heap_peek(ConcurrentHeap *heap, void **min) {

    LOCK(heap);
    Status status = heap_peek(heap->instance, min);
    UNLOCK(heap);

    return status;
}

Status ts_heap_poll(ConcurrentHeap *heap, void **min) {

    LOCK(heap);
    Status status = heap_poll(heap->instance, min);
    UNLOCK(heap);

    return status;
}

void ts_heap_clear(ConcurrentHeap *heap, void (*destructor)(void *)) {

    LOCK(heap);
    heap_clear(heap->instance, destructor);
    UNLOCK(heap);
}

long ts_heap_size(ConcurrentHeap *heap) {

    LOCK(heap);
    long size = heap_size(heap->instance);
    UNLOCK(heap);

    return size;
}

Boolean ts_heap_isEmpty(ConcurrentHeap *heap) {

    LOCK(heap);
    Boolean isEmpty = heap_isEmpty(heap->instance);
    UNLOCK(heap);

    return isEmpty;
}

Status ts_heap_toArray(ConcurrentHeap *heap, Array **array) {

    LOCK(heap);
    Status status = heap_toArray(heap->instance, array);
    UNLOCK(heap);

    return status;
}

Status ts_heap_iterator(ConcurrentHeap *heap, ConcurrentIterator **iter) {

    Array *array;
    Status status;

    /* Creates the array of items and locks it */
    LOCK(heap);
    status = heap_toArray(heap->instance, &array);
    if (status != STAT_SUCCESS) {
        UNLOCK(heap);
        return status;
    }

    /* Creates the iterator */
    status = ts_iterator_new(iter, &(heap->lock), array->items, array->len);
    if (status != STAT_SUCCESS) {
        FREE_ARRAY(array);
        UNLOCK(heap);
    } else {
        free(array);
    }

    return status;
}

void ts_heap_destroy(ConcurrentHeap *heap, void (*destructor)(void *)) {

    LOCK(heap);
    heap_destroy(heap->instance, destructor);
    UNLOCK(heap);
    pthread_mutex_destroy(&(heap->lock));
    free(heap);
}
