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
#include "linkedlist.h"

/*
 * The struct for a node inside the linked list.
 */
typedef struct node {
    struct node *next;  /* Pointer to the next node */
    struct node *prev;  /* Pointer to the previous node */
    void *data;         /* Pointer to hold the element */
} Node;

/*
 * The struct for the linked list ADT.
 */
struct linkedlist {
    Node head;          /* Pointer to the list's head */
    Node tail;          /* Pointer to the list's tail */
    long size;          /* The linked list's size */
};

#define HEADER(x)  (&((x)->head))
#define TRAILER(x) (&((x)->tail))

Status linkedlist_new(LinkedList **list) {

    /* Allocates the struct, checks for allocation failure */
    LinkedList *temp = (LinkedList *)malloc(sizeof(LinkedList));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Initializes the remaining struct members */
    HEADER(temp)->data = NULL;
    HEADER(temp)->next = TRAILER(temp);
    HEADER(temp)->prev = NULL;
    TRAILER(temp)->data = NULL;
    TRAILER(temp)->next = NULL;
    TRAILER(temp)->prev = HEADER(temp);
    temp->size = 0L;
    *list = temp;

    return STAT_SUCCESS;
}

/* Macro used to validate the given index i */
#define INDEX_VALID(i,N) ( ( 0L <= (i) && (i) < (N) ) ? TRUE : FALSE )

/*
 * Fetches the node from list at the specified index.
 */
static Node *fetchNode(LinkedList *list, long index) {

    Node *temp;
    long i;
    long mid = ( list->size / 2 );  /* Middle index in list */

    if (index <= mid) {
        /* Traverses forward from the head to the node */
        temp = HEADER(list)->next;
        for (i = 0L; i < index; i++)
            temp = temp->next;
    } else {
        /* Traverses backwards from the tail to node */
        temp = TRAILER(list)->prev;
        for (i = list->size - 1; i > index; i--)
            temp = temp->prev;
    }

    return temp;
}

static void linkNodes(Node *node, Node *prev, Node *next) {
    node->prev = prev;
    node->next = next;
    next->prev = node;
    prev->next = node;
}

static void unlinkNode(Node *node) {
    Node *next = node->next;
    Node *prev = node->prev;
    next->prev = prev;
    prev->next = next;
}

Status linkedlist_addFirst(LinkedList *list, void *item) {

    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
        return STAT_ALLOC_FAILURE;

    node->data = item;
    linkNodes(node, HEADER(list), HEADER(list)->next);
    list->size++;

    return STAT_SUCCESS;
}

Status linkedlist_addLast(LinkedList *list, void *item) {

    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
        return STAT_ALLOC_FAILURE;

    node->data = item;
    linkNodes(node, TRAILER(list)->prev, TRAILER(list));
    list->size++;

    return STAT_SUCCESS;
}

Status linkedlist_add(LinkedList *list, void *item) {
    return linkedlist_addLast(list, item);
}

Status linkedlist_insert(LinkedList *list, long i, void *item) {

    /* Check if the index is valid */
    if (INDEX_VALID(i, list->size) == FALSE)
        return STAT_INVALID_INDEX;

    if (i == 0)         /* Same operation as addFirst() */
        return linkedlist_addFirst(list, item);
    if (i == list->size)        /* Same operation as addLast() */
        return linkedlist_addLast(list, item);

    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
        return STAT_ALLOC_FAILURE;

    node->data = item;
    Node *temp = fetchNode(list, i);
    linkNodes(node, temp->prev, temp);
    list->size++;

    return STAT_SUCCESS;
}

Status linkedlist_first(LinkedList *list, void **first) {

    /* Checks if the list is empty */
    if (linkedlist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;
    *first = HEADER(list)->next->data;

    return STAT_SUCCESS;
}

Status linkedlist_last(LinkedList *list, void **last) {

    /* Checks if the list is empty */
    if (linkedlist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;
    *last = TRAILER(list)->prev->data;

    return STAT_SUCCESS;
}

Status linkedlist_get(LinkedList *list, long i, void **item) {

    /* Checks if the list is empty */
    if (linkedlist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Checks if the index is valid */
    if (INDEX_VALID(i, list->size) == FALSE)
        return STAT_INVALID_INDEX;

    Node *temp = fetchNode(list, i);
    *item = temp->data;

    return STAT_SUCCESS;
}

Status linkedlist_set(LinkedList *list, long i, void *item, void **previous) {

    /* Checks if the list is empty */
    if (linkedlist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Checks if the index is valid */
    if (INDEX_VALID(i, list->size) == FALSE)
        return STAT_INVALID_INDEX;

    /* Retrieve the node, swap with new element */
    Node *temp = fetchNode(list, i);
    *previous = temp->data;
    temp->data = item;

    return STAT_SUCCESS;
}

Status linkedlist_removeFirst(LinkedList *list, void **first) {

    /* Check if the list is empty */
    if (linkedlist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *temp = HEADER(list)->next;
    *first = temp->data;
    unlinkNode(temp);
    free(temp);
    list->size--;

    return STAT_SUCCESS;
}

Status linkedlist_removeLast(LinkedList *list, void **last) {

    /* Check if the list is empty */
    if (linkedlist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *temp = TRAILER(list)->prev;
    *last = temp->data;
    unlinkNode(temp);
    free(temp);
    list->size--;

    return STAT_SUCCESS;
}

Status linkedlist_remove(LinkedList *list, long i, void **item) {

    /* Checks if the list is empty */
    if (linkedlist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Checks if the index is valid */
    if (INDEX_VALID(i, list->size) == FALSE)
        return STAT_INVALID_INDEX;

    if (i == 0L)        /* Same operation as removeFirst() */
        return linkedlist_removeFirst(list, item);
    if (i == list->size - 1)    /* Same operation as removeLast() */
        return linkedlist_removeLast(list, item);

    /* Fetch the node to be removed */
    Node *temp = fetchNode(list, i);
    *item = temp->data;
    unlinkNode(temp);
    free(temp);
    list->size--;

    return STAT_SUCCESS;
}

/*
 * Local method to clear out the linked list of its elements. If destructor != NULL, it is
 * invoked on each element after removal.
 */
static void clearList(LinkedList *list, void (*destructor)(void *)) {

    void *temp;

    while (linkedlist_removeFirst(list, &temp) == STAT_SUCCESS)
        if (destructor != NULL)
            (*destructor)(temp);
}

void linkedlist_clear(LinkedList *list, void (*destructor)(void *)) {
    clearList(list, destructor);
}

long linkedlist_size(LinkedList *list) {
    return list->size;
}

Boolean linkedlist_isEmpty(LinkedList *list) {
    return ( list->size == 0L ) ? TRUE : FALSE;
}

/*
 * Local method to allocate and create an array representation of the linked list.
 */
static void **generateArray(LinkedList *list) {

    Node *temp = NULL;
    long i = 0L;
    size_t bytes;
    void **items = NULL;

    /* Allocates memory for the array */
    bytes = ( list->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return NULL;

    /* Populates the array with linked list items */
    for (temp = HEADER(list)->next; temp != TRAILER(list); temp = temp->next)
        items[i++] = temp->data;

    return items;
}

Status linkedlist_toArray(LinkedList *list, Array **array) {

    /* Does not create the array if empty */
    if (linkedlist_isEmpty(list) == TRUE)
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

Status linkedlist_iterator(LinkedList *list, Iterator **iter) {

    Iterator *temp = NULL;
    void **items = NULL;

    /* Does not create the array if empty */
    if (linkedlist_isEmpty(list) == TRUE)
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

void linkedlist_destroy(LinkedList *list, void (*destructor)(void *)) {
    clearList(list, destructor);
    free(list);
}
