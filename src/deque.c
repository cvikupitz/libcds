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
#include "deque.h"

/*
 * The struct for a node inside the deque.
 */
typedef struct node {
    struct node *next;  /* Pointer to the next node */
    struct node *prev;  /* Pointer to the previous node */
    void *data;         /* Pointer to hold the element */
} Node;

/*
 * The struct for the deque ADT.
 */
struct deque {
    Node *head;         /* Points to the deque's front */
    Node *tail;         /* Points to the deque's rear */
    long size;          /* The deque's size */
};

Status deque_new(Deque **deque) {

    /* Allocate the struct, check for allocation failures */
    Deque *temp = (Deque *)malloc(sizeof(Deque));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Initialize the reminaing struct members */
    temp->head = NULL;
    temp->tail = NULL;
    temp->size = 0L;
    *deque = temp;

    return STAT_SUCCESS;
}

/*
 * Local method to allocate node for the deque. Links up the new node with the specified
 * 'next' and 'prev' nodes.
 */
static Node *mallocNode(void *item, Node *next, Node *prev) {

    Node *node = (Node *)malloc(sizeof(Node));
    if (node != NULL) {
        node->next = next;
        node->prev = prev;
        node->data = item;
    }

    return node;
}

Status deque_addFirst(Deque *deque, void *item) {

    /* Allocate the node for insertion */
    Node *node = mallocNode(item, deque->head, NULL);
    if (node == NULL)
        return STAT_ALLOC_FAILURE;

    /* Edge case: empty deque, links node as front and rear */
    if (deque_isEmpty(deque) == TRUE)
        deque->tail = node;
    /* Otherwise, links node to front pointer */
    else
        deque->head->prev = node;
    deque->head = node;
    deque->size++;

    return STAT_SUCCESS;
}

Status deque_addLast(Deque *deque, void *item) {

    /* Allocate the node for insertion */
    Node *node = mallocNode(item, NULL, deque->tail);
    if (node == NULL)
        return STAT_ALLOC_FAILURE;

    /* Edge case: empty deque, links node as front and rear */
    if (deque_isEmpty(deque) == TRUE)
        deque->head = node;
    /* Otherwise, links node to rear pointer */
    else
        deque->tail->next = node;
    deque->tail = node;
    deque->size++;

    return STAT_SUCCESS;
}

Status deque_first(Deque *deque, void **first) {

    /* Checks if the deque is empty */
    if (deque_isEmpty(deque) == TRUE)
        return STAT_STRUCT_EMPTY;
    *first = deque->head->data;

    return STAT_SUCCESS;
}

Status deque_last(Deque *deque, void **last) {

    /* Checks if the deque is empty */
    if (deque_isEmpty(deque) == TRUE)
        return STAT_STRUCT_EMPTY;
    *last = deque->tail->data;

    return STAT_SUCCESS;
}

Status deque_removeFirst(Deque *deque, void **first) {

    /* Checks if the deque is empty */
    if (deque_isEmpty(deque) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *temp = deque->head;
    *first = temp->data;
    deque->head = temp->next;

    /* Edge case: one item, so nullify both end pointers */
    if (deque->head == NULL)
        deque->tail = NULL;
    /* Removes the link to the front node */
    else
        deque->head->prev = NULL;
    free(temp);
    deque->size--;

    return STAT_SUCCESS;
}

Status deque_removeLast(Deque *deque, void **last) {

    /* Checks if the deque is empty */
    if (deque_isEmpty(deque) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *temp = deque->tail;
    *last = temp->data;
    deque->tail = temp->prev;

    /* Edge case: one item, so nullify both end pointers */
    if (deque->tail == NULL)
        deque->head = NULL;
    /* Removes the link to the rear node */
    else
        deque->tail->next = NULL;
    free(temp);
    deque->size--;

    return STAT_SUCCESS;
}

/*
 * Local method to clear out the deque of its elements. If destructor != NULL, it is
 * invoked on each element after removal.
 */
static void clearDeque(Deque *deque, void (*destructor)(void *)) {

    void *temp;

    while (deque_removeFirst(deque, &temp) == STAT_SUCCESS)
        if (destructor != NULL)
            (*destructor)(temp);
}

void deque_clear(Deque *deque, void (*destructor)(void *)) {
    clearDeque(deque, destructor);
}

long deque_size(Deque *deque) {
    return deque->size;
}

Boolean deque_isEmpty(Deque *deque) {
    return ( deque->size == 0L ) ? TRUE : FALSE;
}

/*
 * Local method to allocate and create an array representation of the deque.
 */
static void **generateArray(Deque *deque) {

    Node *temp = NULL;
    long i = 0L;
    size_t bytes;
    void **items = NULL;

    /* Allocates the memory for the array */
    bytes = ( deque->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return NULL;

    /* Populates the array with deque items */
    for (temp = deque->head; temp != NULL; temp = temp->next)
        items[i++] = temp->data;

    return items;
}

Status deque_toArray(Deque *deque, Array **array) {

    /* Does not create the deque if currently empty */
    if (deque_isEmpty(deque) == TRUE)
        return STAT_STRUCT_EMPTY;

    void **items = generateArray(deque);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    Array *temp = (Array *)malloc(sizeof(Array));
    if (temp == NULL) {
        free(items);
        return STAT_ALLOC_FAILURE;
    }

    temp->items = items;
    temp->len = deque->size;
    *array = temp;

    return STAT_SUCCESS;
}

Status deque_iterator(Deque *deque, Iterator **iter) {

    Iterator *temp = NULL;
    void **items = NULL;

    if (deque_isEmpty(deque) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Generates the array of stack items for iterator */
    items = generateArray(deque);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    /* Creates a new iterator with the stack items */
    Status status = iterator_new(&temp, items, deque->size);
    if (status != STAT_SUCCESS) {
        free(items);
        return status;
    }
    *iter = temp;

    return STAT_SUCCESS;
}

void deque_destroy(Deque *deque, void (*destructor)(void *)) {
    clearDeque(deque, destructor);
    free(deque);
}
