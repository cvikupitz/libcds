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
#include "circular_list.h"

/*
 * Struct for a node inside the circular list.
 */
typedef struct node {
    struct node *next;  /* Pointer to the next node */
    struct node *prev;  /* Pointer to the previous node */
    void *data;         /* Pointer to hold the element */
} Node;

/*
 * Struct for the circular list ADT.
 */
struct circular_list {
    Node *head;         /* Points to the list's head node */
    long size;          /* The list's current size */
};

Status circularlist_new(CircularList **list) {

    /* Allocates the struct, checks for allocation failure */
    CircularList *temp = (CircularList *)malloc(sizeof(CircularList));
    if (temp == NULL)
        return ALLOC_FAILURE;

    /* Initializes the remaining struct members */
    temp->head = NULL;
    temp->size = 0L;
    *list = temp;

    return OK;
}

/* Macro used to validate the given index i */
#define INDEX_VALID(i,N) ( ( 0L <= (i) && (i) < (N) ) ? TRUE : FALSE )
/* Macro to check if the list is currently empty */
#define IS_EMPTY(x)  ( ((x)->size == 0L) ? TRUE : FALSE )
/* Macro to return the list's tail node */
#define TAIL(x) ( ((x)->head != NULL) ? (x)->head->prev : NULL )

/*
 * Fetches the node from list at the specified index.
 */
static Node *fetchNode(CircularList *list, long index) {

    Node *temp;
    long i;
    long mid = ( list->size / 2 );  /* Middle index in list */

    /* If the index is closer to the head, traverse from the head */
    /* Otherwise, traverse from tail for faster access */
    if (index <= mid) {
        /* Traverses forward from the head to the node */
        temp = list->head;
        for (i = 0L; i < index; i++)
            temp = temp->next;
    } else {
        /* Traverses backwards from the tail to node */
        temp = TAIL(list);
        for (i = list->size - 1; i > index; i--)
            temp = temp->prev;
    }

    return temp;
}

/*
 * Links up the specified node 'node' with 'prev' and 'next' such that
 * the node linkage will become:
 *     prev <------> node <-----> next
 * If either 'prev' or 'next' are NULL, the node will link those pointers
 * to itself.
 */
static void linkNodes(Node *node, Node *prev, Node *next) {
    node->prev = (prev != NULL) ? prev : node;
    node->next = (next != NULL) ? next : node;
    if (next != NULL)
        next->prev = node;
    if (prev != NULL)
        prev->next = node;
}

/*
 * Unlinks the node 'node' from the list. The previous and next nodes
 * will be linked up removing the node.
 */
static void unlinkNode(Node *node) {

    /* Node is linked to itself, so no need to unlink anything */
    if (node == node->next)
        return;
    Node *next = node->next;
    Node *prev = node->prev;
    next->prev = prev;
    prev->next = next;
}

Status circularlist_addFirst(CircularList *list, void *item) {

    /* Allocates the node for insertion */
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
        return ALLOC_FAILURE;

    /* Inserts the node into the front of the list */
    node->data = item;
    linkNodes(node, TAIL(list), list->head);
    list->head = node;
    list->size++;

    return OK;
}

Status circularlist_addLast(CircularList *list, void *item) {

    /* Allocates the node for insertion */
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
        return ALLOC_FAILURE;

    /* Inserts the node into the back of the list */
    node->data = item;
    linkNodes(node, TAIL(list), list->head);
    list->head = node->next;
    list->size++;

    return OK;
}

Status circularlist_insert(CircularList *list, long i, void *item) {

    /* Check if the index is valid */
    if (INDEX_VALID(i, list->size) == FALSE)
        return INVALID_INDEX;

    if (i == 0)         /* Same operation as addFirst() */
        return circularlist_addFirst(list, item);
    if (i == list->size)        /* Same operation as addLast() */
        return circularlist_addLast(list, item);

    /* Allocates the node for insertion */
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
        return ALLOC_FAILURE;

    /* Fetch the node at the current index, relink for insertion */
    node->data = item;
    Node *temp = fetchNode(list, i);
    linkNodes(node, temp->prev, temp);
    list->size++;

    return OK;
}

Status circularlist_first(CircularList *list, void **first) {

    /* Checks if the list is empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;
    /* Fetches the first item, stores into pointer */
    *first = list->head->data;

    return OK;
}

Status circularlist_last(CircularList *list, void **last) {

    /* Checks if the list is empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;
    /* Fetches the last item, stores into pointer */
    *last = TAIL(list)->data;

    return OK;
}

Status circularlist_get(CircularList *list, long i, void **item) {

    /* Checks if the list is empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;
    /* Checks if the index is valid */
    if (INDEX_VALID(i, list->size) == FALSE)
        return INVALID_INDEX;

    /* Fetches the item at the index, stores into pointer */
    Node *temp = fetchNode(list, i);
    *item = temp->data;

    return OK;
}

Status circularlist_set(CircularList *list, long i, void *item, void **previous) {

    /* Checks if the list is empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;
    /* Checks if the index is valid */
    if (INDEX_VALID(i, list->size) == FALSE)
        return INVALID_INDEX;

    /* Retrieve the node, swaps with new element */
    Node *temp = fetchNode(list, i);
    *previous = temp->data;
    temp->data = item;

    return OK;
}

Status circularlist_removeFirst(CircularList *list, void **first) {

    /* Check if the list is empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;

    /* Fetches the first node, unlinks from list */
    list->size--;
    Node *temp = list->head;
    list->head = (IS_EMPTY(list) == FALSE) ? temp->next : NULL;
    *first = temp->data;
    unlinkNode(temp);
    free(temp);

    return OK;
}

Status circularlist_removeLast(CircularList *list, void **last) {

    /* Check if the list is empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;

    /* Fetches the first node, unlinks from list */
    list->size--;
    Node *temp = TAIL(list);
    *last = temp->data;
    unlinkNode(temp);
    free(temp);

    return OK;
}

Status circularlist_remove(CircularList *list, long i, void **item) {

    /* Checks if the list is empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;
    /* Checks if the index is valid */
    if (INDEX_VALID(i, list->size) == FALSE)
        return INVALID_INDEX;

    if (i == 0L)        /* Same operation as removeFirst() */
        return circularlist_removeFirst(list, item);
    if (i == list->size - 1)    /* Same operation as removeLast() */
        return circularlist_removeLast(list, item);

    /* Fetch the node to be removed, unlink from list */
    Node *temp = fetchNode(list, i);
    *item = temp->data;
    unlinkNode(temp);
    free(temp);
    list->size--;

    return OK;
}

void circularlist_rotateForward(CircularList *list) {
    if (IS_EMPTY(list) == FALSE)
        list->head = list->head->next;
}

void circularlist_rotateBackward(CircularList *list) {
    if (IS_EMPTY(list) == FALSE)
        list->head = list->head->prev;
}

/*
 * Clears out the list of all its elements. Frees up all reserved memory
 * back to the heap.
 */
static void clearList(CircularList *list, void (*destructor)(void *)) {

    Node *curr = list->head, *next = NULL;
    long i;

    for (i = 0L; i < list->size; i++) {
        next = curr->next;
        if (destructor != NULL)
            (*destructor)(curr->data);
        free(curr);
        curr = next;
    }
}

void circularlist_clear(CircularList *list, void (*destructor)(void *)) {
    clearList(list, destructor);
    list->head = NULL;
    list->size = 0L;
}

long circularlist_size(CircularList *list) {
    return list->size;
}

Boolean circularlist_isEmpty(CircularList *list) {
    return IS_EMPTY(list);
}

/*
 * Generates and returns an array representation of the circular list.
 */
static void **generateArray(CircularList *list) {

    Node *temp = list->head;
    long i;
    size_t bytes;
    void **items = NULL;

    /* Allocates memory for the array */
    bytes = ( list->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return NULL;

    /* Populates the array with circular list items */
    for (i = 0L; i < list->size; i++) {
        items[i] = temp->data;
        temp = temp->next;
    }

    return items;
}

Status circularlist_toArray(CircularList *list, Array **array) {

    /* Does not create the array if empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;

    /* Generate the array of circular list items */
    void **items = generateArray(list);
    if (items == NULL)
        return ALLOC_FAILURE;

    /* Allocate memory for the array struct */
    Array *temp = (Array *)malloc(sizeof(Array));
    if (temp == NULL) {
        free(items);
        return ALLOC_FAILURE;
    }

    /* Initialize remaining struct members */
    temp->items = items;
    temp->len = list->size;
    *array = temp;

    return OK;
}

Status circularlist_iterator(CircularList *list, Iterator **iter) {

    Iterator *temp = NULL;

    /* Does not create the array if empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;

    /* Generates the array of items for iterator */
    void **items = generateArray(list);
    if (items == NULL)
        return ALLOC_FAILURE;

    /* Creates a new iterator with the items */
    Status status = iterator_new(&temp, items, list->size);
    if (status != OK) {
        free(items);
        return status;
    }
    *iter = temp;

    return OK;
}

void circularlist_destroy(CircularList *list, void (*destructor)(void *)) {
    clearList(list, destructor);
    free(list);
}
