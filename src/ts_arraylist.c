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
#include "arraylist.h"
#include "ts_arraylist.h"

/*
 * Struct for the thread-safe arraylist.
 */
struct ts_arraylist {
    pthread_mutex_t lock;       /* The lock */
    ArrayList *instance;        /* Internal instance of ArrayList */
};

/* Macro used for locking the arraylist */
#define LOCK(x)    pthread_mutex_lock( &((x)->lock) )
/* Macro used for unlocking the arraylist */
#define UNLOCK(x)  pthread_mutex_unlock( &((x)->lock) )

Status ts_arraylist_new(ConcurrentArrayList **list, long capacity) {

    ConcurrentArrayList *temp;
    Status status;
    pthread_mutexattr_t attr;

    /* Allocates memory for the arraylist */
    temp = (ConcurrentArrayList *)malloc(sizeof(ConcurrentArrayList));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Creates new internal arraylist instance */
    status = arraylist_new(&(temp->instance), capacity);
    if (status != STAT_SUCCESS) {
        free(temp);
        return status;
    }

    /* Create the pthread_mutex for locking */
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&(temp->lock), &attr);
    pthread_mutexattr_destroy(&attr);
    *list = temp;

    return STAT_SUCCESS;
}

void ts_arraylist_lock(ConcurrentArrayList *list) {
    LOCK(list);
}

void ts_arraylist_unlock(ConcurrentArrayList *list) {
    UNLOCK(list);
}

Status ts_arraylist_add(ConcurrentArrayList *list, void *item) {

    LOCK(list);
    Status status = arraylist_add(list->instance, item);
    UNLOCK(list);

    return status;
}

Status ts_arraylist_insert(ConcurrentArrayList *list, long i, void *item) {

    LOCK(list);
    Status status = arraylist_insert(list->instance, i, item);
    UNLOCK(list);

    return status;
}

Status ts_arraylist_get(ConcurrentArrayList *list, long i, void **item) {

    LOCK(list);
    Status status = arraylist_get(list->instance, i, item);
    UNLOCK(list);

    return status;
}

Status ts_arraylist_set(ConcurrentArrayList *list, long i, void *item, void **previous) {

    LOCK(list);
    Status status = arraylist_set(list->instance, i, item, previous);
    UNLOCK(list);

    return status;
}

Status ts_arraylist_remove(ConcurrentArrayList *list, long i, void **item) {

    LOCK(list);
    Status status = arraylist_remove(list->instance, i, item);
    UNLOCK(list);

    return status;
}

Status ts_arraylist_ensureCapacity(ConcurrentArrayList *list, long capacity) {

    LOCK(list);
    Status status = arraylist_ensureCapacity(list->instance, capacity);
    UNLOCK(list);

    return status;
}

Status ts_arraylist_trimToSize(ConcurrentArrayList *list) {

    LOCK(list);
    Status status = arraylist_trimToSize(list->instance);
    UNLOCK(list);

    return status;
}

void ts_arraylist_clear(ConcurrentArrayList *list, void (*destructor)(void *)) {

    LOCK(list);
    arraylist_clear(list->instance, destructor);
    UNLOCK(list);
}

long ts_arraylist_size(ConcurrentArrayList *list) {

    LOCK(list);
    long size = arraylist_size(list->instance);
    UNLOCK(list);

    return size;
}

long ts_arraylist_capacity(ConcurrentArrayList *list) {

    LOCK(list);
    long capacity = arraylist_capacity(list->instance);
    UNLOCK(list);

    return capacity;
}

Boolean ts_arraylist_isEmpty(ConcurrentArrayList *list) {

    LOCK(list);
    Boolean isEmpty = arraylist_isEmpty(list->instance);
    UNLOCK(list);

    return isEmpty;
}

Status ts_arraylist_toArray(ConcurrentArrayList *list, Array **array) {

    LOCK(list);
    Status status = arraylist_toArray(list->instance, array);
    UNLOCK(list);

    return status;
}

Status ts_arraylist_iterator(ConcurrentArrayList *list, ConcurrentIterator **iter) {

    Array *array;
    Status status;

    /* Creates the array of items and locks the instance */
    LOCK(list);
    status = arraylist_toArray(list->instance, &array);
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

void ts_arraylist_destroy(ConcurrentArrayList *list, void (*destructor)(void *)) {

    LOCK(list);
    arraylist_destroy(list->instance, destructor);
    UNLOCK(list);
    pthread_mutex_destroy(&(list->lock));
    free(list);
}
