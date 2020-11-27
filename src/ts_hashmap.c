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
#include "hashmap.h"
#include "ts_hashmap.h"

/*
 * Struct for the thread-safe hashmap.
 */
struct ts_hashmap {
    pthread_mutex_t lock;       /* The lock */
    HashMap *instance;          /* Internal instance of HashMap */
};

/* Macro used for locking the map */
#define LOCK(x)    pthread_mutex_lock( &((x)->lock) )
/* Macro used for unlocking the map */
#define UNLOCK(x)  pthread_mutex_unlock( &((x)->lock) )

Status ts_hashmap_new(ConcurrentHashMap **map, long capacity, double loadFactor) {

    ConcurrentHashMap *temp;
    Status status;
    pthread_mutexattr_t attr;

    /* Allocates memory for the new hashmap */
    temp = (ConcurrentHashMap *)malloc(sizeof(ConcurrentHashMap));
    if (temp == NULL)
        return ALLOC_FAILURE;

    /* Creates the internal hashmap */
    status = hashmap_new(&(temp->instance), capacity, loadFactor);
    if (status != OK) {
        free(temp);
        return status;
    }

    /* Creates pthread_mutex for locking */
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&(temp->lock), &attr);
    pthread_mutexattr_destroy(&attr);
    *map = temp;

    return OK;
}

void ts_hashmap_lock(ConcurrentHashMap *map) {
    LOCK(map);
}

void ts_hashmap_unlock(ConcurrentHashMap *map) {
    UNLOCK(map);
}

Status ts_hashmap_put(ConcurrentHashMap *map, char *key, void *value, void **previous) {

    LOCK(map);
    Status status = hashmap_put(map->instance, key, value, previous);
    UNLOCK(map);

    return status;
}

Boolean ts_hashmap_containsKey(ConcurrentHashMap *map, char *key) {

    LOCK(map);
    Boolean containsKey = hashmap_containsKey(map->instance, key);
    UNLOCK(map);

    return containsKey;
}

Status ts_hashmap_get(ConcurrentHashMap *map, char *key, void **value) {

    LOCK(map);
    Status status = hashmap_get(map->instance, key, value);
    UNLOCK(map);

    return status;
}

Status ts_hashmap_remove(ConcurrentHashMap *map, char *key, void **value) {

    LOCK(map);
    Status status = hashmap_remove(map->instance, key, value);
    UNLOCK(map);

    return status;
}

void ts_hashmap_clear(ConcurrentHashMap *map, void (*destructor)(void *)) {

    LOCK(map);
    hashmap_clear(map->instance, destructor);
    UNLOCK(map);
}

long ts_hashmap_size(ConcurrentHashMap *map) {

    LOCK(map);
    long size = hashmap_size(map->instance);
    UNLOCK(map);

    return size;
}

Boolean ts_hashmap_isEmpty(ConcurrentHashMap *map) {

    LOCK(map);
    Boolean isEmpty = hashmap_isEmpty(map->instance);
    UNLOCK(map);

    return isEmpty;
}

Status ts_hashmap_keyArray(ConcurrentHashMap *map, Array **keys) {

    LOCK(map);
    Status status = hashmap_keyArray(map->instance, keys);
    UNLOCK(map);

    return status;
}

Status ts_hashmap_entryArray(ConcurrentHashMap *map, Array **entries) {

    LOCK(map);
    Status status = hashmap_entryArray(map->instance, entries);
    UNLOCK(map);

    return status;
}

Status ts_hashmap_iterator(ConcurrentHashMap *map, ConcurrentIterator **iter) {

    Array *array;
    Status status;

    /* Creates the array of items and locks it */
    LOCK(map);
    status = hashmap_entryArray(map->instance, &array);
    if (status != OK) {
        UNLOCK(map);
        return status;
    }

    /* Creates the iterator */
    status = ts_iterator_new(iter, &(map->lock), array->items, array->len);
    if (status != OK) {
        FREE_ARRAY(array);
        UNLOCK(map);
    } else {
        free(array);
    }

    return status;
}

void ts_hashmap_destroy(ConcurrentHashMap *map, void (*destructor)(void *)) {

    LOCK(map);
    hashmap_destroy(map->instance, destructor);
    UNLOCK(map);
    pthread_mutex_destroy(&(map->lock));
    free(map);
}
