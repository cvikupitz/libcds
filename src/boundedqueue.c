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

#include <stdlib.h>
#include "boundedqueue.h"

/*
 * Struct for the bounded queue ADT.
 */
struct bounded_queue {
    void **data;        /* Array of the queue's elements */
    long front;         /* Index of the queue's front element */
    long size;          /* The queue's current size */
    long capacity;      /* The queue's capacity */
};

/* The default capacity to assign when the capacity give is invalid */
#define DEFAULT_CAPACITY 16L

Status boundedqueue_new(BoundedQueue **queue, long capacity) {

    /* Allocate the struct, check for allocation failures */
    BoundedQueue *temp = (BoundedQueue *)malloc(sizeof(BoundedQueue));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Sets up capacity, sets up reminaing struct members */
    long cap = ( capacity <= 0L ) ? DEFAULT_CAPACITY : capacity;
    size_t bytes = (cap * sizeof(void *));
    void **array = (void **)malloc(bytes);

    /* Checks for allocation failures */
    if (array == NULL) {
        free(temp);
        return STAT_ALLOC_FAILURE;
    }

    /* Initialize the remainder of the struct members */
    temp->data = array;
    temp->front = 0L;
    temp->size = 0L;
    temp->capacity = cap;
    *queue = temp;

    return STAT_SUCCESS;
}

/* Macro to check if the queue is currently empty */
#define IS_EMPTY(x) ( ((x)->size == 0L) ? TRUE : FALSE )
/* Macro to check if the queue is currently full */
#define IS_FULL(x)  ( ((x)->size == (x)->capacity) ? TRUE : FALSE )

Status boundedqueue_add(BoundedQueue *queue, void *item) {

    /* Checks if the queue is full */
    if (IS_FULL(queue) == TRUE)
        return STAT_STRUCT_FULL;

    /* Computes the next index to insert item at */
    long index = ( queue->front + queue->size ) % queue->capacity;
    queue->data[index] = item;
    queue->size++;

    return STAT_SUCCESS;
}

Status boundedqueue_peek(BoundedQueue *queue, void **front) {

    /* Checks if the queue is empty */
    if (IS_EMPTY(queue) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Extracts the front item, saves into pointer */
    *front = queue->data[queue->front];

    return STAT_SUCCESS;
}

Status boundedqueue_poll(BoundedQueue *queue, void **front) {

    /* Checks if the queue is empty */
    if (IS_EMPTY(queue) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Extracts the front item, saves into pointer */
    *front = queue->data[queue->front];
    /* Computes next 'front' index after removal */
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;

    return STAT_SUCCESS;
}

/*
 * Clears out the queue of all its elements.
 */
static void clearQueue(BoundedQueue *queue, void (*destructor)(void *)) {

    long i, j;
    for (i = 0L, j = queue->front; i < queue->size; i++, j = (j + 1) % queue->capacity) {
        if (destructor != NULL)
            (*destructor)(queue->data[j]);
    }
}

void boundedqueue_clear(BoundedQueue *queue, void (*destructor)(void *)) {
    clearQueue(queue, destructor);
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

/*
 * Generates and returns an array representation of the queue.
 */
static void **generateArray(BoundedQueue *queue) {

    long i, j;
    size_t bytes;
    void **items = NULL;

    /* Allocates memory for the array */
    bytes = ( queue->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return NULL;

    /* Populates the array with the queue items */
    for (i = 0L, j = queue->front; i < queue->size; i++, j = (j + 1) % queue->capacity)
        items[i] = queue->data[j];

    return items;
}

Status boundedqueue_toArray(BoundedQueue *queue, Array **array) {

    /* Does not create array if currently empty */
    if (boundedqueue_isEmpty(queue) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Generate the array of queue items */
    void **items = generateArray(queue);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    /* Allocate memory for the array struct */
    Array *temp = (Array *)malloc(sizeof(Array));
    if (temp == NULL) {
        free(items);
        return STAT_ALLOC_FAILURE;
    }

    /* Initialize the remainder of the struct members */
    temp->items = items;
    temp->len = queue->size;
    *array = temp;

    return STAT_SUCCESS;
}

Status boundedqueue_iterator(BoundedQueue *queue, Iterator **iter) {

    Iterator *temp = NULL;

    /* Does not create array if currently empty */
    if (boundedqueue_isEmpty(queue) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Generates the array of items for iterator */
    void **items = generateArray(queue);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    /* Creates a new iterator with the items */
    Status status = iterator_new(&temp, items, queue->size);
    if (status != STAT_SUCCESS) {
        free(items);
        return status;
    }
    *iter = temp;

    return STAT_SUCCESS;
}

void boundedqueue_destroy(BoundedQueue *queue, void (*destructor)(void *)) {
    clearQueue(queue, destructor);
    free(queue->data);
    free(queue);
}
