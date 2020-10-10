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
#include "queue.h"

/*
 * The struct for a node inside the queue.
 */
typedef struct node {
    struct node *next;  /* Pointer to the next node */
    void *data;         /* Pointer to hold the element */
} Node;

/*
 * The struct for the queue ADT.
 */
struct queue {
    Node *head;         /* Pointer to head queue node */
    Node *tail;         /* Pointer to tail queue node */
    long size;          /* The queue's size */
};

Status queue_new(Queue **queue) {

    /* Allocate the struct, check for allocation failures */
    Queue *temp = (Queue *)malloc(sizeof(Queue));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Initializes the remaining struct members */
    temp->head = NULL;
    temp->tail = NULL;
    temp->size = 0L;
    *queue = temp;

    return STAT_SUCCESS;
}

Status queue_enqueue(Queue *queue, void *item) {

    /* Allocate the node for item insertion */
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
        return STAT_ALLOC_FAILURE;

    node->next = NULL;
    node->data = item;
    /* Edge case: queue is empty, link the front and rear nodes */
    if (queue->size == 0L)
        queue->head = node;
    /* Otherwise, links to the rear node */
    else
        queue->tail->next = node;
    queue->tail = node;
    queue->size++;

    return STAT_SUCCESS;
}

Status queue_peek(Queue *queue, void **first) {

    /* Checks of the queue is empty */
    if (queue_isEmpty(queue) == TRUE)
        return STAT_STRUCT_EMPTY;
    *first = queue->head->data;

    return STAT_SUCCESS;
}

Status queue_dequeue(Queue *queue, void **first) {

    /* Checks if the queue is empty */
    if (queue_isEmpty(queue) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *temp = queue->head;
    queue->head = temp->next;
    /* Edge case: one item, nullify both head and tail pointers */
    if (queue->size == 1L)
        queue->tail = NULL;
    /* Free the allocated node's struct */
    *first = temp->data;
    free(temp);
    queue->size--;

    return STAT_SUCCESS;
}

/*
 * Local method to clear out the queue of its elements. If destructor != NULL, it is
 * invoked on each element after removal.
 */
static void clearQueue(Queue *queue, void (*destructor)(void *)) {

    void *temp;

    while (queue_dequeue(queue, &temp) == STAT_SUCCESS)
        if (destructor != NULL)
            (*destructor)(temp);
}

void queue_clear(Queue *queue, void (*destructor)(void *)) {
    clearQueue(queue, destructor);
}

long queue_size(Queue *queue) {
    return queue->size;
}

Boolean queue_isEmpty(Queue *queue) {
    return ( queue->size == 0L ) ? TRUE : FALSE;
}

/*
 * Local method to allocate and create an array representation of the queue.
 */
static Status generateArray(Queue *queue, void ***array, long *len) {

    Node *temp = NULL;
    long i = 0L;
    size_t bytes;
    void **items = NULL;

    /* Does not create the array if currently empty */
    if (queue_isEmpty(queue) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Allocates memory for the array */
    bytes = ( queue->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    /* Populates the array with the queue items */
    for (temp = queue->head; temp != NULL; temp = temp->next)
        items[i++] = temp->data;
    *array = items;
    *len = queue->size;

    return STAT_SUCCESS;
}

Status queue_toArray(Queue *queue, void ***array, long *len) {
    return generateArray(queue, array, len);
}

Status queue_iterator(Queue *queue, Iterator **iter) {

    Iterator *temp = NULL;
    void **items = NULL;
    long len;

    /* Generates the array of queue items for the iterator */
    Status status = generateArray(queue, &items, &len);
    if (status != STAT_SUCCESS)
        return status;

    /* Creates a new iterator with the queue items */
    status = iterator_new(&temp, items, len);
    if (status != STAT_SUCCESS) {
        free(items);
        return status;
    }
    *iter = temp;

    return STAT_SUCCESS;
}

void queue_destroy(Queue *queue, void (*destructor)(void *)) {
    clearQueue(queue, destructor);
    free(queue);
}
