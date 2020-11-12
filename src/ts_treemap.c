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
#include "treemap.h"
#include "ts_treemap.h"

struct ts_treemap {
    pthread_mutex_t lock;
    TreeMap *instance;
};

#define LOCK(x)    pthread_mutex_lock( &((x)->lock) )
#define UNLOCK(x)  pthread_mutex_unlock( &((x)->lock) )

Status ts_treemap_new(ConcurrentTreeMap **tree, int (*keyComparator)(void *, void *),
        void (*keyDestructor)(void *))
{
    ConcurrentTreeMap *temp;
    Status status;
    pthread_mutexattr_t attr;

    temp = (ConcurrentTreeMap *)malloc(sizeof(ConcurrentTreeMap));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    status = treemap_new(&(temp->instance), keyComparator, keyDestructor);
    if (status != STAT_SUCCESS) {
        free(temp);
        return status;
    }

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&(temp->lock), &attr);
    pthread_mutexattr_destroy(&attr);
    *tree = temp;

    return STAT_SUCCESS;
}

void ts_treemap_lock(ConcurrentTreeMap *tree) {
    LOCK(tree);
}

void ts_treemap_unlock(ConcurrentTreeMap *tree) {
    UNLOCK(tree);
}

Status ts_treemap_put(ConcurrentTreeMap *tree, void *key, void *value, void **previous) {

    LOCK(tree);
    Status status = treemap_put(tree->instance, key, value, previous);
    UNLOCK(tree);

    return status;
}

Status ts_treemap_firstKey(ConcurrentTreeMap *tree, void **firstKey) {

    LOCK(tree);
    Status status = treemap_firstKey(tree->instance, firstKey);
    UNLOCK(tree);

    return status;
}

Status ts_treemap_first(ConcurrentTreeMap *tree, TmEntry **first) {

    LOCK(tree);
    Status status = treemap_first(tree->instance, first);
    UNLOCK(tree);

    return status;
}

Status ts_treemap_lastKey(ConcurrentTreeMap *tree, void **lastKey) {

    LOCK(tree);
    Status status = treemap_lastKey(tree->instance, lastKey);
    UNLOCK(tree);

    return status;
}

Status ts_treemap_last(ConcurrentTreeMap *tree, TmEntry **last) {

    LOCK(tree);
    Status status = treemap_last(tree->instance, last);
    UNLOCK(tree);

    return status;
}

Status ts_treemap_floorKey(ConcurrentTreeMap *tree, void *key, void **floorKey) {

    LOCK(tree);
    Status status = treemap_floorKey(tree->instance, key, floorKey);
    UNLOCK(tree);

    return status;
}

Status ts_treemap_floor(ConcurrentTreeMap *tree, void *key, TmEntry **floor) {

    LOCK(tree);
    Status status = treemap_floor(tree->instance, key, floor);
    UNLOCK(tree);

    return status;
}

Status ts_treemap_ceilingKey(ConcurrentTreeMap *tree, void *key, void **ceilingKey) {

    LOCK(tree);
    Status status = treemap_ceilingKey(tree->instance, key, ceilingKey);
    UNLOCK(tree);

    return status;
}

Status ts_treemap_ceiling(ConcurrentTreeMap *tree, void *key, TmEntry **ceiling) {

    LOCK(tree);
    Status status = treemap_ceiling(tree->instance, key, ceiling);
    UNLOCK(tree);

    return status;
}

Status ts_treemap_lowerKey(ConcurrentTreeMap *tree, void *key, void **lowerKey) {

    LOCK(tree);
    Status status = treemap_lowerKey(tree->instance, key, lowerKey);
    UNLOCK(tree);

    return status;
}

Status ts_treemap_lower(ConcurrentTreeMap *tree, void *key, TmEntry **lower) {

    LOCK(tree);
    Status status = treemap_lower(tree->instance, key, lower);
    UNLOCK(tree);

    return status;
}

Status ts_treemap_higherKey(ConcurrentTreeMap *tree, void *key, void **higherKey) {

    LOCK(tree);
    Status status = treemap_higherKey(tree->instance, key, higherKey);
    UNLOCK(tree);

    return status;
}

Status ts_treemap_higher(ConcurrentTreeMap *tree, void *key, TmEntry **higher) {

    LOCK(tree);
    Status status = treemap_higher(tree->instance, key, higher);
    UNLOCK(tree);

    return status;
}

Boolean ts_treemap_containsKey(ConcurrentTreeMap *tree, void *key) {

    LOCK(tree);
    Boolean containsKey = treemap_containsKey(tree->instance, key);
    UNLOCK(tree);

    return containsKey;
}

Status ts_treemap_get(ConcurrentTreeMap *tree, void *key, void **value) {

    LOCK(tree);
    Status status = treemap_get(tree->instance, key, value);
    UNLOCK(tree);

    return status;
}

Status ts_treemap_pollFirst(ConcurrentTreeMap *tree, void **firstKey, void **firstValue) {

    LOCK(tree);
    Status status = treemap_pollFirst(tree->instance, firstKey, firstValue);
    UNLOCK(tree);

    return status;
}

Status ts_treemap_pollLast(ConcurrentTreeMap *tree, void **lastKey, void **lastValue) {

    LOCK(tree);
    Status status = treemap_pollLast(tree->instance, lastKey, lastValue);
    UNLOCK(tree);

    return status;
}

Status ts_treemap_remove(ConcurrentTreeMap *tree, void *key, void **value) {

    LOCK(tree);
    Status status = treemap_remove(tree->instance, key, value);
    UNLOCK(tree);

    return status;
}

void ts_treemap_clear(ConcurrentTreeMap *tree, void (*valueDestructor)(void *)) {

    LOCK(tree);
    treemap_clear(tree->instance, valueDestructor);
    UNLOCK(tree);
}

long ts_treemap_size(ConcurrentTreeMap *tree) {

    LOCK(tree);
    long size = treemap_size(tree->instance);
    UNLOCK(tree);

    return size;
}

Boolean ts_treemap_isEmpty(ConcurrentTreeMap *tree) {

    LOCK(tree);
    Boolean isEmpty = treemap_isEmpty(tree->instance);
    UNLOCK(tree);

    return isEmpty;
}

Status ts_treemap_keyArray(ConcurrentTreeMap *tree, Array **keys) {

    LOCK(tree);
    Status status = treemap_keyArray(tree->instance, keys);
    UNLOCK(tree);

    return status;
}

Status ts_treemap_entryArray(ConcurrentTreeMap *tree, Array **entries) {

    LOCK(tree);
    Status status = treemap_entryArray(tree->instance, entries);
    UNLOCK(tree);

    return status;
}

Status ts_treemap_iterator(ConcurrentTreeMap *tree, ConcurrentIterator **iter) {

    Array *array;
    Status status;

    LOCK(tree);
    status = treemap_entryArray(tree->instance, &array);
    if (status != STAT_SUCCESS) {
        UNLOCK(tree);
        return status;
    }

    status = ts_iterator_new(iter, &(tree->lock), array->items, array->len);
    if (status != STAT_SUCCESS) {
        FREE_ARRAY(array);
        UNLOCK(tree);
    } else {
        free(array);
    }

    return status;
}

void ts_treemap_destroy(ConcurrentTreeMap *tree, void (*valueDestructor)(void *)) {

    LOCK(tree);
    treemap_destroy(tree->instance, valueDestructor);
    UNLOCK(tree);
    pthread_mutex_destroy(&(tree->lock));
    free(tree);
}
