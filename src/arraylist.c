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
 * The structure for the arraylist ADT.
 */
struct arraylist {
    void **data;        /* The arraylist's elements */
    long size;          /* The arraylist's size */
    long capacity;      /* The arraylist's current capacity */
};

/* The default capacity to assign when the capacity give is invalid */
#define DEFAULT_CAPACITY 10L

Status arraylist_new(ArrayList **list, long capacity) {

    /* Allocate the struct, check for allocation failures */
    ArrayList *temp = (ArrayList *)malloc(sizeof(ArrayList));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Set up capacity, initialize the reminaing struct members */
    long cap = ( capacity <= 0L ) ? DEFAULT_CAPACITY : capacity;
    size_t bytes = (cap * sizeof(void *));
    void **array = (void **)malloc(bytes);

    if (array == NULL) {
        free(temp);
        return STAT_ALLOC_FAILURE;
    }

    temp->data = array;
    temp->size = 0L;
    temp->capacity = cap;
    *list = temp;

    return STAT_SUCCESS;
}

/* Macro used to validate the given index i */
#define INDEX_VALID(i,N) ( ( 0L <= (i) && (i) < (N) ) ? TRUE : FALSE )

/*
 * Extends the arraylist's capacity to the new specified capacity. Returns TRUE if
 * extension was successful, FALSE if not.
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

/*
 * Shifts all items in arraylist to the left onto the specified index. Performed to
 * fill gaps after removals.
 */
static void shiftLeft(ArrayList *list, long index) {
    long i;
    for (i = index; i < list->size - 1; i++)
        list->data[i] = list->data[i + 1];
}

/*
 * Shifts all items in arraylist to the right after the specified index. Performed to
 * create gaps for insertions.
 */
static void shiftRight(ArrayList *list, long index) {
    long i;
    for (i = list->size; i > index; i--)
        list->data[i] = list->data[i - 1];
}

Status arraylist_add(ArrayList *list, void *item) {

    /* Check the capacity, extends if needed */
    if (list->size == list->capacity)
        if (ensureCapacity(list, list->capacity * 2) == FALSE)
            return STAT_ALLOC_FAILURE;
    /* Append the new item to the arraylist */
    list->data[list->size++] = item;

    return STAT_SUCCESS;
}

Status arraylist_insert(ArrayList *list, long i, void *item) {

    /* Checks if the index is valid */
    if (INDEX_VALID(i, list->size + 1) == FALSE)
        return STAT_INVALID_INDEX;

    /* Check the capacity, extend if needed */
    if (list->size == list->capacity)
        if (ensureCapacity(list, list->capacity * 2) == FALSE)
            return STAT_ALLOC_FAILURE;

    /* Shift items to make room for insertion */
    shiftRight(list, i);
    list->data[i] = item;
    list->size++;

    return STAT_SUCCESS;
}

Status arraylist_get(ArrayList *list, long i, void **item) {

    /* Checks if the list is currently empty */
    if (arraylist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Checks if the index is valid */
    if (INDEX_VALID(i, list->size) == FALSE)
        return STAT_INVALID_INDEX;
    *item = list->data[i];

    return STAT_SUCCESS;
}

Status arraylist_set(ArrayList *list, long i, void *item, void **previous) {

    /* Checks if the list is currently empty */
    if (arraylist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Checks if the index is valid */
    if (INDEX_VALID(i, list->size) == FALSE)
        return STAT_INVALID_INDEX;

    *previous = list->data[i];
    list->data[i] = item;

    return STAT_SUCCESS;
}

Status arraylist_remove(ArrayList *list, long i, void **item) {

    /* Checks if the list is currently empty */
    if (arraylist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Checks if the index is valid */
    if (INDEX_VALID(i, list->size) == FALSE)
        return STAT_INVALID_INDEX;

    *item = list->data[i];
    /* Shift items to fill gap after removal */
    shiftLeft(list, i);
    list->size--;

    return STAT_SUCCESS;
}

Status arraylist_ensureCapacity(ArrayList *list, long capacity) {

    /* Only extend if capacity < newCapacity */
    if (list->capacity < capacity)
        if (ensureCapacity(list, capacity) == FALSE)
            return STAT_ALLOC_FAILURE;

    return STAT_SUCCESS;
}

Status arraylist_trimToSize(ArrayList *list) {

    /* Checks if the list is currently empty */
    if (arraylist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Only trim if size < capacity */
    if (list->size != list->capacity)
        if (ensureCapacity(list, list->size) == FALSE)
            return STAT_ALLOC_FAILURE;

    return STAT_SUCCESS;
}

/*
 * Local method to clear out the arraylist of its elements. If destructor != NULL,
 * it is invoked on each element after removal.
 */
static void clearList(ArrayList *list, void (*destructor)(void *)) {

    void *temp;

    while (arraylist_isEmpty(list) == FALSE) {
        temp = list->data[--list->size];
        if (destructor != NULL)
            (*destructor)(temp);
    }
}

void arraylist_clear(ArrayList *list, void (*destructor)(void *)) {
    clearList(list, destructor);
}

long arraylist_size(ArrayList *list) {
    return list->size;
}

long arraylist_capacity(ArrayList *list) {
    return list->capacity;
}

Boolean arraylist_isEmpty(ArrayList *list) {
    return ( list->size == 0L ) ? TRUE : FALSE;
}

/*
 * Local method to allocate and create an array representation of the arraylist.
 */
static Status generateArray(ArrayList *list, void ***array, long *len) {

    long i;
    size_t bytes;
    void **items = NULL;

    /* Do not create the array if currently empty */
    if (arraylist_isEmpty(list) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Allocates memory for the array */
    bytes = ( list->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    /* Populates the array with arraylist items */
    for (i = 0; i < list->size; i++)
        items[i] = list->data[i];
    *array = items;
    *len = list->size;

    return STAT_SUCCESS;
}

Status arraylist_toArray(ArrayList *list, void ***array, long *len) {
    return generateArray(list, array, len);
}

Status arraylist_iterator(ArrayList *list, Iterator **iter) {

    Iterator *temp = NULL;
    void **items = NULL;
    long len;

    /* Generates the array of arraylist items for iterator */
    Status status = generateArray(list, &items, &len);
    if (status != STAT_SUCCESS)
        return status;

    /* Creates a new iterator with the arraylist items */
    status = iterator_new(&temp, items, len);
    if (status != STAT_SUCCESS) {
        free(items);
        return status;
    }
    *iter = temp;

    return STAT_SUCCESS;
}

void arraylist_destroy(ArrayList *list, void (*destructor)(void *)) {
    clearList(list, destructor);
    free(list->data);
    free(list);
}
