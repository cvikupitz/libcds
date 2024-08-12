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
#include "linked_list.h"
#include "ts_linked_list.h"

/**
 * Struct for the thread-safe linked list.
 */
struct ts_linkedlist {
    pthread_mutex_t lock;       // The lock
    LinkedList *instance;       // Internal instance of LinkedList
};

// Macro used for locking the list `li`
#define LOCK(li)    pthread_mutex_lock( &((li)->lock) )
// Macro used for unlocking the list `li`
#define UNLOCK(li)  pthread_mutex_unlock( &((li)->lock) )

Status ts_linkedlist_new(ConcurrentLinkedList **list) {

    ConcurrentLinkedList *temp;
    Status status;
    pthread_mutexattr_t attr;

    // Allocates memory for the linkedlist
    temp = (ConcurrentLinkedList *)malloc(sizeof(ConcurrentLinkedList));
    if (temp == NULL) {
        return ALLOC_FAILURE;
    }

    // Creates internal instance of linked list
    status = linkedlist_new(&(temp->instance));
    if (status != OK) {
        free(temp);
        return status;
    }

    // Creates the pthread_mutex for locking
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&(temp->lock), &attr);
    pthread_mutexattr_destroy(&attr);
    *list = temp;

    return OK;
}

void ts_linkedlist_lock(ConcurrentLinkedList *list) {
    LOCK(list);
}

void ts_linkedlist_unlock(ConcurrentLinkedList *list) {
    UNLOCK(list);
}

Status ts_linkedlist_addFirst(ConcurrentLinkedList *list, void *item) {

    LOCK(list);
    Status status = linkedlist_addFirst(list->instance, item);
    UNLOCK(list);

    return status;
}

Status ts_linkedlist_addLast(ConcurrentLinkedList *list, void *item) {

    LOCK(list);
    Status status = linkedlist_addLast(list->instance, item);
    UNLOCK(list);

    return status;
}

Status ts_linkedlist_insert(ConcurrentLinkedList *list, long i, void *item) {

    LOCK(list);
    Status status = linkedlist_insert(list->instance, i, item);
    UNLOCK(list);

    return status;
}

Status ts_linkedlist_first(ConcurrentLinkedList *list, void **first) {

    LOCK(list);
    Status status = linkedlist_first(list->instance, first);
    UNLOCK(list);

    return status;
}

Status ts_linkedlist_last(ConcurrentLinkedList *list, void **last) {

    LOCK(list);
    Status status = linkedlist_last(list->instance, last);
    UNLOCK(list);

    return status;
}

Status ts_linkedlist_get(ConcurrentLinkedList *list, long i, void **item) {

    LOCK(list);
    Status status = linkedlist_get(list->instance, i, item);
    UNLOCK(list);

    return status;
}

Status ts_linkedlist_set(ConcurrentLinkedList *list, long i, void *item, void **previous) {

    LOCK(list);
    Status status = linkedlist_set(list->instance, i, item, previous);
    UNLOCK(list);

    return status;
}

Status ts_linkedlist_removeFirst(ConcurrentLinkedList *list, void **first) {

    LOCK(list);
    Status status = linkedlist_removeFirst(list->instance, first);
    UNLOCK(list);

    return status;
}

Status ts_linkedlist_removeLast(ConcurrentLinkedList *list, void **last) {

    LOCK(list);
    Status status = linkedlist_removeLast(list->instance, last);
    UNLOCK(list);

    return status;
}

Status ts_linkedlist_remove(ConcurrentLinkedList *list, long i, void **item) {

    LOCK(list);
    Status status = linkedlist_remove(list->instance, i, item);
    UNLOCK(list);

    return status;
}

void ts_linkedlist_clear(ConcurrentLinkedList *list, void (*destructor)(void *)) {

    LOCK(list);
    linkedlist_clear(list->instance, destructor);
    UNLOCK(list);
}

long ts_linkedlist_size(ConcurrentLinkedList *list) {

    LOCK(list);
    long size = linkedlist_size(list->instance);
    UNLOCK(list);

    return size;
}

Boolean ts_linkedlist_isEmpty(ConcurrentLinkedList *list) {

    LOCK(list);
    Boolean isEmpty = linkedlist_isEmpty(list->instance);
    UNLOCK(list);

    return isEmpty;
}

Status ts_linkedlist_toArray(ConcurrentLinkedList *list, Array **array) {

    LOCK(list);
    Status status = linkedlist_toArray(list->instance, array);
    UNLOCK(list);

    return status;
}

Status ts_linkedlist_iterator(ConcurrentLinkedList *list, ConcurrentIterator **iter) {

    Array *array;
    Status status;

    // Creates the array of items and locks it
    LOCK(list);
    status = linkedlist_toArray(list->instance, &array);
    if (status != OK) {
        UNLOCK(list);
        return status;
    }

    // Creates the iterator
    status = ts_iterator_new(iter, &(list->lock), array->items, array->len);
    if (status != OK) {
        FREE_ARRAY(array);
        UNLOCK(list);
    } else {
        free(array);
    }

    return status;
}

void ts_linkedlist_destroy(ConcurrentLinkedList *list, void (*destructor)(void *)) {

    LOCK(list);
    linkedlist_destroy(list->instance, destructor);
    UNLOCK(list);
    pthread_mutex_destroy(&(list->lock));
    free(list);
}
