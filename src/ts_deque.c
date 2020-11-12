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
#include "deque.h"
#include "ts_deque.h"

struct ts_deque {
    pthread_mutex_t lock;
    Deque *instance;
};

#define LOCK(x)    pthread_mutex_lock( &((x)->lock) )
#define UNLOCK(x)  pthread_mutex_unlock( &((x)->lock) )

Status ts_deque_new(ConcurrentDeque **deque) {

    ConcurrentDeque *temp;
    Status status;
    pthread_mutexattr_t attr;

    temp = (ConcurrentDeque *)malloc(sizeof(ConcurrentDeque));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    status = deque_new(&(temp->instance));
    if (status != STAT_SUCCESS) {
        free(temp);
        return status;
    }

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&(temp->lock), &attr);
    pthread_mutexattr_destroy(&attr);
    *deque = temp;

    return STAT_SUCCESS;
}

void ts_deque_lock(ConcurrentDeque *deque) {
    LOCK(deque);
}

void ts_deque_unlock(ConcurrentDeque *deque) {
    UNLOCK(deque);
}

Status ts_deque_addFirst(ConcurrentDeque *deque, void *item) {

    LOCK(deque);
    Status status = deque_addFirst(deque->instance, item);
    UNLOCK(deque);

    return status;
}

Status ts_deque_addLast(ConcurrentDeque *deque, void *item) {

    LOCK(deque);
    Status status = deque_addLast(deque->instance, item);
    UNLOCK(deque);

    return status;
}

Status ts_deque_first(ConcurrentDeque *deque, void **first) {

    LOCK(deque);
    Status status = deque_first(deque->instance, first);
    UNLOCK(deque);

    return status;
}

Status ts_deque_last(ConcurrentDeque *deque, void **last) {

    LOCK(deque);
    Status status = deque_last(deque->instance, last);
    UNLOCK(deque);

    return status;
}

Status ts_deque_removeFirst(ConcurrentDeque *deque, void **first) {

    LOCK(deque);
    Status status = deque_removeFirst(deque->instance, first);
    UNLOCK(deque);

    return status;
}

Status ts_deque_removeLast(ConcurrentDeque *deque, void **last) {

    LOCK(deque);
    Status status = deque_removeLast(deque->instance, last);
    UNLOCK(deque);

    return status;
}

void ts_deque_clear(ConcurrentDeque *deque, void (*destructor)(void *)) {

    LOCK(deque);
    deque_clear(deque->instance, destructor);
    UNLOCK(deque);
}

long ts_deque_size(ConcurrentDeque *deque) {

    LOCK(deque);
    long size = deque_size(deque->instance);
    UNLOCK(deque);

    return size;
}

Boolean ts_deque_isEmpty(ConcurrentDeque *deque) {

    LOCK(deque);
    Boolean isEmpty = deque_isEmpty(deque->instance);
    UNLOCK(deque);

    return isEmpty;
}

Status ts_deque_toArray(ConcurrentDeque *deque, Array **array) {

    LOCK(deque);
    Status status = deque_toArray(deque->instance, array);
    UNLOCK(deque);

    return status;
}

Status ts_deque_iterator(ConcurrentDeque *deque, ConcurrentIterator **iter) {

    Array *array;
    Status status;

    LOCK(deque);
    status = deque_toArray(deque->instance, &array);
    if (status != STAT_SUCCESS) {
        UNLOCK(deque);
        return status;
    }

    status = ts_iterator_new(iter, &(deque->lock), array->items, array->len);
    if (status != STAT_SUCCESS) {
        FREE_ARRAY(array);
        UNLOCK(deque);
    } else {
        free(array);
    }

    return status;
}

void ts_deque_destroy(ConcurrentDeque *deque, void (*destructor)(void *)) {

    LOCK(deque);
    deque_destroy(deque->instance, destructor);
    UNLOCK(deque);
    pthread_mutex_destroy(&(deque->lock));
    free(deque);
}
