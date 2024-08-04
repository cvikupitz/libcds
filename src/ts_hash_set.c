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
#include "hash_set.h"
#include "ts_hash_set.h"

/*
 * Struct for the thread-safe hashset.
 */
struct ts_hashset {
    pthread_mutex_t lock;       /* The lock */
    HashSet *instance;          /* Internal instance of HashSet */
};

/* Macro used for locking the set */
#define LOCK(x)    pthread_mutex_lock( &((x)->lock) )
/* Macro used for unlocking the set */
#define UNLOCK(x)  pthread_mutex_unlock( &((x)->lock) )

Status ts_hashset_new(ConcurrentHashSet **set, long (*hash)(void *, long),
        int (*comparator)(void *, void *), long capacity, double loadFactor)
{
    ConcurrentHashSet *temp;
    Status status;
    pthread_mutexattr_t attr;

    /* Allocates memory for the hashset */
    temp = (ConcurrentHashSet *)malloc(sizeof(ConcurrentHashSet));
    if (temp == NULL)
        return ALLOC_FAILURE;

    /* Creates internal instance of hashset */
    status = hashset_new(&(temp->instance), hash, comparator, capacity, loadFactor);
    if (status != OK) {
        free(temp);
        return status;
    }

    /* Creates pthread_mutex for locking */
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&(temp->lock), &attr);
    pthread_mutexattr_destroy(&attr);
    *set = temp;

    return OK;
}

void ts_hashset_lock(ConcurrentHashSet *set) {
    LOCK(set);
}

void ts_hashset_unlock(ConcurrentHashSet *set) {
    UNLOCK(set);
}

Status ts_hashset_add(ConcurrentHashSet *set, void *item) {

    LOCK(set);
    Status status = hashset_add(set->instance, item);
    UNLOCK(set);

    return status;
}

Boolean ts_hashset_contains(ConcurrentHashSet *set, void *item) {

    LOCK(set);
    Boolean contains = hashset_contains(set->instance, item);
    UNLOCK(set);

    return contains;
}

Status ts_hashset_remove(ConcurrentHashSet *set, void *item, void (*destructor)(void *)) {

    LOCK(set);
    Status status = hashset_remove(set->instance, item, destructor);
    UNLOCK(set);

    return status;
}

void ts_hashset_clear(ConcurrentHashSet *set, void (*destructor)(void *)) {

    LOCK(set);
    hashset_clear(set->instance, destructor);
    UNLOCK(set);
}

long ts_hashset_size(ConcurrentHashSet *set) {

    LOCK(set);
    long size = hashset_size(set->instance);
    UNLOCK(set);

    return size;
}

Boolean ts_hashset_isEmpty(ConcurrentHashSet *set) {

    LOCK(set);
    Boolean isEmpty = hashset_isEmpty(set->instance);
    UNLOCK(set);

    return isEmpty;
}

Status ts_hashset_toArray(ConcurrentHashSet *set, Array **array) {

    LOCK(set);
    Status status = hashset_toArray(set->instance, array);
    UNLOCK(set);

    return status;
}

Status ts_hashset_iterator(ConcurrentHashSet *set, ConcurrentIterator **iter) {

    Array *array;
    Status status;

    /* Creates array of items and locks it */
    LOCK(set);
    status = hashset_toArray(set->instance, &array);
    if (status != OK) {
        UNLOCK(set);
        return status;
    }

    /* Creates the iterator */
    status = ts_iterator_new(iter, &(set->lock), array->items, array->len);
    if (status != OK) {
        FREE_ARRAY(array);
        UNLOCK(set);
    } else {
        free(array);
    }

    return status;
}

void ts_hashset_destroy(ConcurrentHashSet *set, void (*destructor)(void *)) {

    LOCK(set);
    hashset_destroy(set->instance, destructor);
    UNLOCK(set);
    pthread_mutex_destroy(&(set->lock));
    free(set);
}
