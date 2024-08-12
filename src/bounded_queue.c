/**
 * MIT License
 *
 * Copyright (c) 2020 Cole Vikupitz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdlib.h>
#include "bounded_queue.h"

/**
 * Struct for the bounded queue ADT.
 */
struct bounded_queue {
    void **data;        // Array of the queue's elements
    long front;         // Index of the queue's front element
    long size;          // The queue's current size
    long capacity;      // The queue's capacity
};

// The default capacity to assign when the capacity give is invalid
#define DEFAULT_CAPACITY 16L

Status boundedqueue_new(BoundedQueue **queue, long capacity) {

    // Allocate the struct, check for allocation failures
    BoundedQueue *temp = (BoundedQueue *)malloc(sizeof(BoundedQueue));
    if (temp == NULL) {
        return ALLOC_FAILURE;
    }

    // Sets up capacity, sets up reminaing struct members
    long cap = ( capacity <= 0L ) ? DEFAULT_CAPACITY : capacity;
    size_t bytes = (cap * sizeof(void *));
    void **array = (void **)malloc(bytes);

    // Checks for allocation failures
    if (array == NULL) {
        free(temp);
        return ALLOC_FAILURE;
    }

    // Initialize the remainder of the struct members
    long i;
    for (i = 0L; i < cap; i++) {
        array[i] = NULL;
    }
    temp->data = array;
    temp->front = 0L;
    temp->size = 0L;
    temp->capacity = cap;
    *queue = temp;

    return OK;
}

// Macro to check if the queue `q` is currently empty
#define IS_EMPTY(q) ( ((q)->size == 0L) ? TRUE : FALSE )
// Macro to check if the queue `q` is currently full
#define IS_FULL(q)  ( ((q)->size == (q)->capacity) ? TRUE : FALSE )

Status boundedqueue_add(BoundedQueue *queue, void *item) {

    // Checks if the queue is full
    if (IS_FULL(queue) == TRUE) {
        return STRUCT_FULL;
    }

    // Computes the next index to insert item at
    long index = ( queue->front + queue->size ) % queue->capacity;
    queue->data[index] = item;
    queue->size++;

    return OK;
}

Status boundedqueue_peek(BoundedQueue *queue, void **front) {

    // Checks if the queue is empty
    if (IS_EMPTY(queue) == TRUE) {
        return STRUCT_EMPTY;
    }
    // Extracts the front item, saves into pointer
    *front = queue->data[queue->front];

    return OK;
}

Status boundedqueue_poll(BoundedQueue *queue, void **front) {

    // Checks if the queue is empty
    if (IS_EMPTY(queue) == TRUE) {
        return STRUCT_EMPTY;
    }

    // Extracts the front item, saves into pointer
    *front = queue->data[queue->front];
    // Computes next 'front' index after removal
    queue->data[queue->front] = NULL;
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;

    return OK;
}

/**
 * Clears out the queue `queue` of all its elements, applying the destructor method `destructor` on
 * each element (or if NULL, nothing will be done).
 */
static void _clear_queue(BoundedQueue *queue, void (*destructor)(void *)) {

    long i, j;
    for (i = 0L, j = queue->front; i < queue->size; i++, j = (j + 1) % queue->capacity) {
        if (destructor != NULL) {
            (*destructor)(queue->data[j]);
        }
        queue->data[j] = NULL;
    }
}

void boundedqueue_clear(BoundedQueue *queue, void (*destructor)(void *)) {
    _clear_queue(queue, destructor);
    queue->front = 0L;
    queue->size = 0L;
}

long boundedqueue_size(BoundedQueue *queue) {
    return queue->size;
}

long boundedqueue_capacity(BoundedQueue *queue) {
    return queue->capacity;
}

Boolean boundedqueue_isEmpty(BoundedQueue *queue) {
    return IS_EMPTY(queue);
}

Boolean boundedqueue_isFull(BoundedQueue *queue) {
    return IS_FULL(queue);
}

/**
 * Helper method to generate an array representation of the queue. Returns the allocated array with
 * the populated elements, or NULL if failed (allocation error).
 */
static void **_generate_array(BoundedQueue *queue) {

    long i, j;
    size_t bytes;
    void **items = NULL;

    // Allocates memory for the array
    bytes = ( queue->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL) {
        return NULL;
    }

    // Populates the array with the queue items
    for (i = 0L, j = queue->front; i < queue->size; i++, j = (j + 1) % queue->capacity) {
        items[i] = queue->data[j];
    }

    return items;
}

Status boundedqueue_toArray(BoundedQueue *queue, Array **array) {

    // Does not create array if currently empty
    if (boundedqueue_isEmpty(queue) == TRUE) {
        return STRUCT_EMPTY;
    }

    // Generate the array of queue items
    void **items = _generate_array(queue);
    if (items == NULL) {
        return ALLOC_FAILURE;
    }

    // Allocate memory for the array struct
    Array *temp = (Array *)malloc(sizeof(Array));
    if (temp == NULL) {
        free(items);
        return ALLOC_FAILURE;
    }

    // Initialize the remainder of the struct members
    temp->items = items;
    temp->len = queue->size;
    *array = temp;

    return OK;
}

Status boundedqueue_iterator(BoundedQueue *queue, Iterator **iter) {

    Iterator *temp = NULL;

    // Does not create array if currently empty
    if (boundedqueue_isEmpty(queue) == TRUE) {
        return STRUCT_EMPTY;
    }

    // Generates the array of items for iterator
    void **items = _generate_array(queue);
    if (items == NULL) {
        return ALLOC_FAILURE;
    }

    // Creates a new iterator with the items
    Status status = iterator_new(&temp, items, queue->size);
    if (status != OK) {
        free(items);
        return status;
    }
    *iter = temp;

    return OK;
}

void boundedqueue_destroy(BoundedQueue *queue, void (*destructor)(void *)) {
    _clear_queue(queue, destructor);
    free(queue->data);
    free(queue);
}
