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
#include "circularlist.h"

/*
 * The struct for a node inside the circular list
 */
typedef struct node {
    struct node *next;  /* Pointer to the next node */
    void *data;         /* Pointer to hold the element */
} Node;

/*
 * The struct for the circular list ADT.
 */
struct circular_list {
    Node *tail;         /* Pointer to the tail node */
    long size;          /* The circular list's size */
};

Status circularlist_new(CircularList **list) {

    /* Allocates the struct, check for allocation failures */
    CircularList *temp = (CircularList *)malloc(sizeof(CircularList));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Initialize the remaining struct members */
    temp->tail = NULL;
    temp->size = 0L;
    *list = temp;

    return STAT_SUCCESS;
}

Status circularlist_addFirst(CircularList *list, void *item) {

    /* Allocates the node for insertion */
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
        return STAT_ALLOC_FAILURE;

    node->data = item;
    if (list->size == 0L) {
        /* Edge case: when empty, link the tail node to itself */
        node->next = node;
        list->tail = node;
    } else {
        /* Links new node to tail's next pointer */
        node->next = list->tail->next;
        list->tail->next = node;
    }
    list->size++;

    return STAT_SUCCESS;
}

Status circularlist_addLast(CircularList *list, void *item) {

    Status status = circularlist_addFirst(list, item);
    /* Same as addFirst(), but needs to rotate afterwards */
    if (status == STAT_SUCCESS)
        list->tail = list->tail->next;

    return status;
}

Status circularlist_first(CircularList *list, void **first) {

    /* Checks if the list is empty */
    if (circularlist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;
    *first = list->tail->next->data;

    return STAT_SUCCESS;
}

Status circularlist_last(CircularList *list, void **last) {

    /* Checks if the list is empty */
    if (circularlist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;
    *last = list->tail->data;

    return STAT_SUCCESS;
}

Status circularlist_poll(CircularList *list, void **first) {

    /* Checks if the list is empty */
    if (circularlist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *temp = list->tail->next;
    /* Edge case: if only one item, nullify the tail pointer */
    if (list->size == 1L)
        list->tail = NULL;
    /* Links the next node to the tail's next pointer */
    else
        list->tail->next = temp->next;

    *first = temp->data;
    free(temp);
    list->size--;

    return STAT_SUCCESS;
}

Status circularlist_rotate(CircularList *list) {

    /* Checks if the list is empty */
    if (circularlist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;
    list->tail = list->tail->next;

    return STAT_SUCCESS;
}

/*
 * Local method to clear out the circular list of its elements. If destructor != NULL,
 * it is invoked on each element after removal.
 */
static void clearList(CircularList *list, void (*destructor)(void *)) {

    void *temp;

    while (circularlist_poll(list, &temp) == STAT_SUCCESS)
        if (destructor != NULL)
            (*destructor)(temp);
}

void circularlist_clear(CircularList *list, void (*destructor)(void *)) {
    clearList(list, destructor);
}

long circularlist_size(CircularList *list) {
    return list->size;
}

Boolean circularlist_isEmpty(CircularList *list) {
    return ( list->size == 0L ) ? TRUE : FALSE;
}

/*
 * Local method to allocate and create an array representation of the circular list.
 */
static void **generateArray(CircularList *list) {

    Node *temp = NULL;
    long i = 0L;
    size_t bytes;
    void **items = NULL;

    /* Allocates memory for the array */
    bytes = ( list->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return NULL;

    /* Populates the array with the list items */
    temp = list->tail;
    for (i = 0L; i < list->size; i++) {
        temp = temp->next;
        items[i] = temp->data;
    }

    return items;
}

Status circularlist_toArray(CircularList *list, Array **array) {

    /* Does not create the array if currently empty */
    if (circularlist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;

    void **items = generateArray(list);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    Array *temp = (Array *)malloc(sizeof(Array));
    if (temp == NULL) {
        free(items);
        return STAT_ALLOC_FAILURE;
    }

    temp->items = items;
    temp->len = list->size;
    *array = temp;

    return STAT_SUCCESS;
}

Status circularlist_iterator(CircularList *list, Iterator **iter) {

    Iterator *temp = NULL;
    void **items = NULL;

    /* Does not create the array if currently empty */
    if (circularlist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Generates the array of stack items for iterator */
    items = generateArray(list);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    /* Creates a new iterator with the stack items */
    Status status = iterator_new(&temp, items, list->size);
    if (status != STAT_SUCCESS) {
        free(items);
        return status;
    }
    *iter = temp;

    return STAT_SUCCESS;
}

void circularlist_destroy(CircularList *list, void (*destructor)(void *)) {
    clearList(list, destructor);
    free(list);
}
