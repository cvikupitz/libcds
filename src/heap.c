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
#include "heap.h"

/*
 * The struct for the heap ADT
 */
struct heap {
    int (*cmp)(void *, void *); /* Function for comparing the heap's elements */
    void **data;                /* Array to store heap's elements */
    long size;                  /* The heap's size */
    long capacity;              /* The heap's current capacity */
};

/* The default capacity to assign when the capacity give is invalid */
#define DEFAULT_CAPACITY 16L

Status heap_new(Heap **heap, long capacity, int (*comparator)(void *, void *)) {

    /* Allocate the struct, check for allocation failure */
    Heap *temp = (Heap *)malloc(sizeof(Heap));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Evaluate the capacity, initialize the remaining members */
    long cap = (capacity <= 0L) ? DEFAULT_CAPACITY : capacity;
    size_t bytes = (cap * sizeof(void *));
    void **array = (void **)malloc(bytes);

    if (array == NULL) {
        free(heap);
        return STAT_ALLOC_FAILURE;
    }

    temp->data = array;
    temp->size = 0L;
    temp->capacity = cap;
    temp->cmp = comparator;
    *heap = temp;

    return STAT_SUCCESS;
}

/* Returns the left child index of i */
#define LEFT_CHILD(i)  ( (2 * i) + 1 )
/* Returns the right child index of i */
#define RIGHT_CHILD(i) ( (2 * i) + 2 )
/* Returns the parent index of i */
#define PARENT(i)      ( (i - 1) / 2 )

/* Returns TRUE if i has a left child index, FALSE if not */
#define HAS_LEFT(i,N)  ( LEFT_CHILD(i) < N ) ? TRUE : FALSE
/* Returns TRUE if i has a right child index, FALSE if not */
#define HAS_RIGHT(i,N) ( RIGHT_CHILD(i) < N ) ? TRUE : FALSE
/* Returns TRUE if i has a parent index, FALSE if not */
#define HAS_PARENT(i)  ( PARENT(i) >= 0 ) ? TRUE : FALSE

/* Macro for comparing heap items given the indecies; used for readability */
#define CMP(x,y) (*heap->cmp)(heap->data[x], heap->data[y])

/*
 * Swaps two elements in the array of items given the two indecies.
 */
static void swap(void **data, long i, long j) {
    void *temp = data[i];
    data[i] = data[j];
    data[j] = temp;
}

/*
 * Local method to update the heap via heapifying upwards. Needs to be done after
 * an insertion.
 */
static void upheap(Heap *heap) {

    long index = ( heap->size - 1 );
    while (HAS_PARENT(index) == TRUE && CMP(PARENT(index), index) > 0) {
        /* Swaps the lesser items towards the top of the heap */
        swap(heap->data, PARENT(index), index);
        index = PARENT(index);
    }
}

/*
 * Local method to update the heap via heapifying downwards. Needs to be done after
 * a deletion.
 */
static void downheap(Heap *heap) {

    long index = 0L;
    while (HAS_LEFT(index, heap->size) == TRUE) {
        long childIndex = LEFT_CHILD(index);
        /* Advance downwards while lesser items are below */
        if (HAS_RIGHT(index, heap->size) == TRUE
                && CMP(RIGHT_CHILD(index), LEFT_CHILD(index)) < 0)
            childIndex = RIGHT_CHILD(index);

        if (CMP(index, childIndex) < 0)
            break;
        /* Swaps the greater items towards the bottom of the heap */
        swap(heap->data, index, childIndex);
        index = childIndex;
    }
}

/*
 * Doubles the heap's capacity; returns TRUE if successful, FALSE if not.
 */
static Boolean ensureCapacity(Heap *heap) {

    Boolean status = FALSE;
    size_t bytes = ( heap->capacity * 2 ) * sizeof(void *);
    void **temp = realloc(heap->data, bytes);

    if (temp != NULL) {
        /* Update the heap's properties */
        heap->data = temp;
        heap->capacity *= 2;
        status = TRUE;
    }

    return status;
}

Status heap_insert(Heap *heap, void *item) {

    /* Checks the capacity, extend if needed */
    if (heap->size == heap->capacity)
        if (ensureCapacity(heap) == FALSE)
            return STAT_ALLOC_FAILURE;

    heap->data[heap->size++] = item;
    /* Upheap to update the heap */
    upheap(heap);

    return STAT_SUCCESS;
}

Status heap_peek(Heap *heap, void **min) {

    /* Checks if the heap is empty */
    if (heap_isEmpty(heap) == TRUE)
        return STAT_STRUCT_EMPTY;
    *min = heap->data[0];

    return STAT_SUCCESS;
}

Status heap_poll(Heap *heap, void **min) {

    /* Checks if the heap is empty */
    if (heap_isEmpty(heap) == TRUE)
        return STAT_STRUCT_EMPTY;

    *min = heap->data[0];
    heap->data[0] = heap->data[--heap->size];
    /* Downheap to update the heap */
    downheap(heap);

    return STAT_SUCCESS;
}

/*
 * Local method to clear out the heap of its elements. If destructor != NULL, it is
 * invoked on each element after removal.
 */
static void clearHeap(Heap *heap, void (*destructor)(void *)) {

    void *temp;

    while (heap_poll(heap, &temp) == STAT_SUCCESS)
        if (destructor != NULL)
            (*destructor)(temp);
}

void heap_clear(Heap *heap, void (*destructor)(void *)) {
    clearHeap(heap, destructor);
}

long heap_size(Heap *heap) {
    return heap->size;
}

Boolean heap_isEmpty(Heap *heap) {
    return ( heap->size == 0L ) ? TRUE : FALSE;
}

/*
 * Local method to allocate and create an array representation of the heap.
 */
static Status generateArray(Heap *heap, void ***array, long *len) {

    long i = 0L;
    size_t bytes;
    void **items = NULL;

    /* Does not create the array if currently empty */
    if (heap_isEmpty(heap) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Allocates memory for the array */
    bytes = ( heap->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    /* Populates the array with the heap items */
    for (i = 0L; i < heap->size; i++)
        items[i] = heap->data[i];
    *array = items;
    *len = heap->size;

    return STAT_SUCCESS;
}

Status heap_toArray(Heap *heap, void ***array, long *len) {
    return generateArray(heap, array, len);
}

Status heap_iterator(Heap *heap, Iterator **iter) {

    Iterator *temp = NULL;
    void **items = NULL;
    long len;

    /* Generates the array of stack items for iterator */
    Status status = generateArray(heap, &items, &len);
    if (status != STAT_SUCCESS)
        return status;

    /* Creates a new iterator with the heap items */
    status = iterator_new(&temp, items, len);
    if (status != STAT_SUCCESS) {
        free(items);
        return status;
    }
    *iter = temp;

    return STAT_SUCCESS;
}

void heap_destroy(Heap *heap, void (*destructor)(void *)) {
    clearHeap(heap, destructor);
    free(heap->data);
    free(heap);
}
