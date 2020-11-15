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
#include "circularlist.h"
#include "ts_circularlist.h"

/*
 * Struct for the thread-safe circular list.
 */
struct ts_circular_list {
    pthread_mutex_t lock;       /* The lock */
    CircularList *instance;     /* Internal isntance of CircularList */
};

/* Macro used for locking the list */
#define LOCK(x)    pthread_mutex_lock( &((x)->lock) )
/* Macro used for unlocking the list */
#define UNLOCK(x)  pthread_mutex_unlock( &((x)->lock) )

Status ts_circularlist_new(ConcurrentCircularList **list) {

    ConcurrentCircularList *temp;
    Status status;
    pthread_mutexattr_t attr;

    /* Allocates memory for the list instance */
    temp = (ConcurrentCircularList *)malloc(sizeof(ConcurrentCircularList));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Creates the internal list instance */
    status = circularlist_new(&(temp->instance));
    if (status != STAT_SUCCESS) {
        free(temp);
        return status;
    }

    /* Creates the pthread_mutex for locking */
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&(temp->lock), &attr);
    pthread_mutexattr_destroy(&attr);
    *list = temp;

    return STAT_SUCCESS;
}

void ts_circularlist_lock(ConcurrentCircularList *list) {
    LOCK(list);
}

void ts_circularlist_unlock(ConcurrentCircularList *list) {
    UNLOCK(list);
}

Status ts_circularlist_addFirst(ConcurrentCircularList *list, void *item) {

    LOCK(list);
    Status status = circularlist_addFirst(list->instance, item);
    UNLOCK(list);

    return status;
}

Status ts_circularlist_addLast(ConcurrentCircularList *list, void *item) {

    LOCK(list);
    Status status = circularlist_addLast(list->instance, item);
    UNLOCK(list);

    return status;
}

Status ts_circularlist_first(ConcurrentCircularList *list, void **first) {

    LOCK(list);
    Status status = circularlist_first(list->instance, first);
    UNLOCK(list);

    return status;
}

Status ts_circularlist_last(ConcurrentCircularList *list, void **last) {

    LOCK(list);
    Status status = circularlist_last(list->instance, last);
    UNLOCK(list);

    return status;
}

Status ts_circularlist_poll(ConcurrentCircularList *list, void **first) {

    LOCK(list);
    Status status = circularlist_poll(list->instance, first);
    UNLOCK(list);

    return status;
}

Status ts_circularlist_rotate(ConcurrentCircularList *list) {

    LOCK(list);
    Status status = circularlist_rotate(list->instance);
    UNLOCK(list);

    return status;
}

void ts_circularlist_clear(ConcurrentCircularList *list, void (*destructor)(void *)) {

    LOCK(list);
    circularlist_clear(list->instance, destructor);
    UNLOCK(list);
}

long ts_circularlist_size(ConcurrentCircularList *list) {

    LOCK(list);
    long size = circularlist_size(list->instance);
    UNLOCK(list);

    return size;
}

Boolean ts_circularlist_isEmpty(ConcurrentCircularList *list) {

    LOCK(list);
    Boolean isEmpty = circularlist_isEmpty(list->instance);
    UNLOCK(list);

    return isEmpty;
}

Status ts_circularlist_toArray(ConcurrentCircularList *list, Array **array) {

    LOCK(list);
    Status status = circularlist_toArray(list->instance, array);
    UNLOCK(list);

    return status;
}

Status ts_circularlist_iterator(ConcurrentCircularList *list, ConcurrentIterator **iter) {

    Array *array;
    Status status;

    /* Creates the array of items and locks it */
    LOCK(list);
    status = circularlist_toArray(list->instance, &array);
    if (status != STAT_SUCCESS) {
        UNLOCK(list);
        return status;
    }

    /* Creates the iterator */
    status = ts_iterator_new(iter, &(list->lock), array->items, array->len);
    if (status != STAT_SUCCESS) {
        FREE_ARRAY(array);
        UNLOCK(list);
    } else {
        free(array);
    }

    return status;
}

void ts_circularlist_destroy(ConcurrentCircularList *list, void (*destructor)(void *)) {

    LOCK(list);
    circularlist_destroy(list->instance, destructor);
    UNLOCK(list);
    pthread_mutex_destroy(&(list->lock));
    free(list);
}
