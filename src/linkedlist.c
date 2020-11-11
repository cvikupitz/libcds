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
    void *payload;      /* Pointer to hold the element */
} Node;

/*
 * The struct for the linked list ADT.
 */
struct linkedlist {
    Node *head;       /* Pointer to the list's head */
    Node *tail;       /* Pointer to the list's tail */
    long size;        /* The linked list's size */
};

Status linkedlist_new(LinkedList **list) {

    /* Allocates the struct, checks for allocation failure */
    LinkedList *temp = (LinkedList *)malloc(sizeof(LinkedList));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Initializes the remaining struct members */
    temp->head = NULL;
    temp->tail = NULL;
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
        temp = list->head;
        for (i = 0L; i < index; i++)
            temp = temp->next;
    } else {
        /* Traverses backwards from the tail to node */
        temp = list->tail;
        for (i = list->size - 1; i > index; i--)
            temp = temp->prev;
    }

    return temp;
}

/*
 * Local method to allocate node for the linked list. Links up the new node with the
 * specified 'next' and 'prev' nodes.
 */
static Node *mallocNode(void *item, Node *next, Node *prev) {

    Node *node = (Node *)malloc(sizeof(Node));
    if (node != NULL) {
        node->payload = item;
        node->next = next;
        node->prev = prev;
    }

    return node;
}

Status linkedlist_addFirst(LinkedList *list, void *item) {

    /* Allocates the node for insertion */
    Node *node = mallocNode(item, list->head, NULL);
    if (node == NULL)
        return STAT_ALLOC_FAILURE;

    /* Edge case: list is empty, link node as head and tail */
    if (list->size == 0L)
        list->tail = node;
    /* Otherwise, link node to head */
    else
        list->head->prev = node;
    list->head = node;
    list->size++;

    return STAT_SUCCESS;
}

Status linkedlist_addLast(LinkedList *list, void *item) {

    /* Allocates the node for insertion */
    Node *node = mallocNode(item, NULL, list->tail);
    if (node == NULL)
        return STAT_ALLOC_FAILURE;

    /* Edge case: list is empty, link node as head and tail */
    if (list->size == 0L)
        list->head = node;
    /* Otherwise, link node to tail */
    else
        list->tail->next = node;
    list->tail = node;
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

    /* Allocate the node for insertion */
    Node *temp = fetchNode(list, i);
    Node *node = mallocNode(item, temp, temp->prev);
    if (node == NULL)
        return STAT_ALLOC_FAILURE;

    /* Links the new node to middle of list */
    temp->prev->next = node;
    temp->prev = node;
    list->size++;

    return STAT_SUCCESS;
}

Status linkedlist_first(LinkedList *list, void **first) {

    /* Checks if the list is empty */
    if (linkedlist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;
    *first = list->head->payload;

    return STAT_SUCCESS;
}

Status linkedlist_last(LinkedList *list, void **last) {

    /* Checks if the list is empty */
    if (linkedlist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;
    *last = list->tail->payload;

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
    *item = temp->payload;

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
    *previous = temp->payload;
    temp->payload = item;

    return STAT_SUCCESS;
}

Status linkedlist_removeFirst(LinkedList *list, void **first) {

    /* Check if the list is empty */
    if (linkedlist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *temp = list->head;
    *first = temp->payload;
    list->head = temp->next;
    /* Edge case: size is 1, nullify both end nodes */
    if (list->size == 1L)
        list->tail = NULL;
    /* Otherwise, remove link to head node */
    else
        list->head->prev = NULL;
    free(temp);
    list->size--;

    return STAT_SUCCESS;
}

Status linkedlist_removeLast(LinkedList *list, void **last) {

    /* Check if the list is empty */
    if (linkedlist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *temp = list->tail;
    *last = temp->payload;
    list->tail = temp->prev;
    /* Edge case: size if 1, nullify both end nodes */
    if (list->size == 1L)
        list->head = NULL;
    /* Otherwise, remove link to tail node */
    else
        list->tail->next = NULL;
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
    *item = temp->payload;
    /* Unlinks the node from linked list */
    temp->next->prev = temp->prev;
    temp->prev->next = temp->next;
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
    for (temp = list->head; temp != NULL; temp = temp->next)
        items[i++] = temp->payload;

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
