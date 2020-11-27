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
#include "arraylist.h"

/*
 * Struct for the arraylist ADT.
 */
struct arraylist {
    void **data;        /* The list of elements */
    long size;          /* The arraylist's current size */
    long capacity;      /* The arraylist's current capacity */
};

/* The default capacity to assign when the capacity give is invalid */
#define DEFAULT_CAPACITY 10L

Status arraylist_new(ArrayList **list, long capacity) {

    /* Allocate the struct, check for allocation failures */
    ArrayList *temp = (ArrayList *)malloc(sizeof(ArrayList));
    if (temp == NULL)
        return ALLOC_FAILURE;

    /* Set up capacity, initialize the reminaing struct members */
    long cap = ( capacity <= 0L ) ? DEFAULT_CAPACITY : capacity;
    size_t bytes = (cap * sizeof(void *));
    void **array = (void **)malloc(bytes);

    /* Checks for allocation failures */
    if (array == NULL) {
        free(temp);
        return ALLOC_FAILURE;
    }

    /* Initializes the remainder of struct members */
    temp->data = array;
    temp->size = 0L;
    temp->capacity = cap;
    *list = temp;

    return OK;
}

/* Macro used to validate the given index i */
#define INDEX_VALID(i,N) ( ( 0L <= (i) && (i) < (N) ) ? TRUE : FALSE )
/* Macro to check if the list is currently empty */
#define IS_EMPTY(x)  ( ((x)->size == 0L) ? TRUE : FALSE )

/*
 * Extends the arraylist's capacity to the new specified capacity. Returns TRUE if
 * extension was successful, or FALSE if not.
 */
static Boolean ensureCapacity(ArrayList *list, long newCapacity) {

    Boolean status = FALSE;
    size_t bytes = ( newCapacity * sizeof(void *) );
    void **temp = (void **)realloc(list->data, bytes);

    if (temp != NULL) {
        /* Update attributes after extension */
        list->data = temp;
        list->capacity = newCapacity;
        status = TRUE;
    }

    return status;
}

Status arraylist_add(ArrayList *list, void *item) {

    /* Check the capacity, extends if needed */
    if (list->size == list->capacity)
        if (ensureCapacity(list, list->capacity * 2) == FALSE)
            return ALLOC_FAILURE;
    /* Append the new item to the arraylist */
    list->data[list->size++] = item;

    return OK;
}

Status arraylist_insert(ArrayList *list, long i, void *item) {

    /* Checks if the index is valid */
    if (INDEX_VALID(i, list->size + 1) == FALSE)
        return INVALID_INDEX;

    /* Check the capacity, extend if needed */
    if (list->size == list->capacity)
        if (ensureCapacity(list, list->capacity * 2) == FALSE)
            return ALLOC_FAILURE;

    /* Shift items to make room for insertion */
    long j;
    for (j = list->size; j > i; j--)
        list->data[j] = list->data[j - 1];
    /* Insert data into new index */
    list->data[i] = item;
    list->size++;

    return OK;
}

Status arraylist_get(ArrayList *list, long i, void **item) {

    /* Checks if the list is currently empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;

    /* Checks if the index is valid */
    if (INDEX_VALID(i, list->size) == FALSE)
        return INVALID_INDEX;
    /* Retrieves the item, saves into pointer */
    *item = list->data[i];

    return OK;
}

Status arraylist_set(ArrayList *list, long i, void *item, void **previous) {

    /* Checks if the list is currently empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;
    /* Checks if the index is valid */
    if (INDEX_VALID(i, list->size) == FALSE)
        return INVALID_INDEX;

    /* Replaces the old item with the new one */
    *previous = list->data[i];
    list->data[i] = item;

    return OK;
}

Status arraylist_remove(ArrayList *list, long i, void **item) {

    /* Checks if the list is currently empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;
    /* Checks if the index is valid */
    if (INDEX_VALID(i, list->size) == FALSE)
        return INVALID_INDEX;

    /* Retrieve removed item, saves to pointer */
    *item = list->data[i];
    /* Shift items to fill gap after removal */
    long j;
    for (j = i; j < list->size - 1; j++)
        list->data[j] = list->data[j + 1];
    list->size--;

    return OK;
}

Status arraylist_ensureCapacity(ArrayList *list, long capacity) {

    /* Only extend if capacity < newCapacity */
    if (list->capacity < capacity)
        if (ensureCapacity(list, capacity) == FALSE)
            return ALLOC_FAILURE;

    return OK;
}

Status arraylist_trimToSize(ArrayList *list) {

    /* Checks if the list is currently empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;

    /* Only trim if size < capacity */
    if (list->size != list->capacity)
        if (ensureCapacity(list, list->size) == FALSE)
            return ALLOC_FAILURE;

    return OK;
}

/*
 * Clears out the list of all its elements. Frees up all reserved memory
 * back to the heap.
 */
static void clearList(ArrayList *list, void (*destructor)(void *)) {

    long i;
    for (i = 0L; i < list->size; i++) {
        if (destructor != NULL)
            (*destructor)(list->data[i]);
    }
}

void arraylist_clear(ArrayList *list, void (*destructor)(void *)) {
    clearList(list, destructor);
    list->size = 0L;
}

long arraylist_size(ArrayList *list) {
    return list->size;
}

long arraylist_capacity(ArrayList *list) {
    return list->capacity;
}

Boolean arraylist_isEmpty(ArrayList *list) {
    return IS_EMPTY(list);
}

/*
 * Generates and returns an array representation of the array list.
 */
static void **generateArray(ArrayList *list) {

    long i;
    size_t bytes;
    void **items = NULL;

    /* Allocates memory for the array */
    bytes = ( list->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return NULL;

    /* Populates the array with arraylist items */
    for (i = 0; i < list->size; i++)
        items[i] = list->data[i];

    return items;
}

Status arraylist_toArray(ArrayList *list, Array **array) {

    /* Do not create the array if currently empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;

    /* Generate the array of arraylist items */
    void **items = generateArray(list);
    if (items == NULL)
        return ALLOC_FAILURE;

    /* Allocate memory for the array struct */
    Array *temp = (Array *)malloc(sizeof(Array));
    if (temp == NULL) {
        free(items);
        return ALLOC_FAILURE;
    }

    /* Initializes the remaining struct members */
    temp->items = items;
    temp->len = list->size;
    *array = temp;

    return OK;
}

Status arraylist_iterator(ArrayList *list, Iterator **iter) {

    Iterator *temp = NULL;

    /* Checks if the list is currently empty */
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

void arraylist_destroy(ArrayList *list, void (*destructor)(void *)) {
    clearList(list, destructor);
    free(list->data);
    free(list);
}
