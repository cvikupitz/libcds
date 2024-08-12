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

/**
 * Struct for the heap ADT.
 */
struct heap {
    int (*cmp)(void *, void *);     // Function for comparing the heap's elements
    void **data;                    // Array of the heap elements
    long size;                      // The heap's current size
    long capacity;                  // The heap's current capacity
};

// The default capacity to assign when the capacity give is invalid
#define DEFAULT_CAPACITY 16L

Status heap_new(Heap **heap, long capacity, int (*comparator)(void *, void *)) {

    // Allocate the struct, check for allocation failure
    Heap *temp = (Heap *)malloc(sizeof(Heap));
    if (temp == NULL) {
        return ALLOC_FAILURE;
    }

    // Evaluate the capacity, initialize the remaining members
    long cap = (capacity <= 0L) ? DEFAULT_CAPACITY : capacity;
    size_t bytes = (cap * sizeof(void *));
    void **array = (void **)malloc(bytes);

    // Checks for allocation failures
    if (array == NULL) {
        free(heap);
        return ALLOC_FAILURE;
    }

    // Initializes the remainder of struct members
    long i;
    for (i = 0L; i < cap; i++) {
        array[i] = NULL;
    }
    temp->data = array;
    temp->size = 0L;
    temp->capacity = cap;
    temp->cmp = comparator;
    *heap = temp;

    return OK;
}

// Returns the left child index of `i`
#define LEFT_CHILD(i)    ( ( 2 * (i) ) + 1 )
// Returns the right child index of `i`
#define RIGHT_CHILD(i)   ( ( 2 * (i) ) + 2 )
// Returns the parent index of `i`
#define PARENT(i)        ( ( (i) - 1 ) / 2 )

// Returns TRUE if `i` has a left child index in array of size `N`, FALSE if not
#define HAS_LEFT(i, N)    ( ( (LEFT_CHILD(i)) < (N) ) ? TRUE : FALSE )
// Returns TRUE if i has a right child index in array of size `N`, FALSE if not
#define HAS_RIGHT(i, N)   ( ( (RIGHT_CHILD(i)) < (N) ) ? TRUE : FALSE )
// Returns TRUE if `i` has a parent index, FALSE if not
#define HAS_PARENT(i)     ( ( (PARENT(i)) >= 0 ) ? TRUE : FALSE )

// Macro for comparing heap items given the indecies `x` and `y`; used for readability
#define CMP(x, y)         ( (*heap->cmp)(heap->data[x], heap->data[y]) )

// Macro to check if the heap `h` is currently empty
#define IS_EMPTY(h)  ( ((h)->size == 0L) ? TRUE : FALSE )

/**
 * Swaps two elements in the array of items given the two indecies `i` and `j`.
 */
static void _swap(void **data, long i, long j) {
    void *temp = data[i];
    data[i] = data[j];
    data[j] = temp;
}

/**
 * Updates the heap via heapifying upwards. Needs to be done after an insertion.
 */
static void _upheap(Heap *heap) {

    long index = ( heap->size - 1 );
    while (HAS_PARENT(index) == TRUE && CMP(PARENT(index), index) > 0) {
        // Swaps the lesser items towards the top of the heap
        _swap(heap->data, PARENT(index), index);
        index = PARENT(index);
    }
}

/**
 * Updates the heap via heapifying downwards. Needs to be done after a deletion.
 */
static void _downheap(Heap *heap) {

    long index = 0L;
    while (HAS_LEFT(index, heap->size) == TRUE) {
        long childIndex = LEFT_CHILD(index);
        // Advance downwards while lesser items are below
        if (HAS_RIGHT(index, heap->size) == TRUE && CMP(RIGHT_CHILD(index), LEFT_CHILD(index)) < 0)
        {
            childIndex = RIGHT_CHILD(index);
        }

        if (CMP(index, childIndex) < 0) {
            break;
        }
        // Swaps the greater items towards the bottom of the heap
        _swap(heap->data, index, childIndex);
        index = childIndex;
    }
}

/**
 * Doubles the heap's capacity; returns TRUE if successful, FALSE if not.
 */
static Boolean _ensure_capacity(Heap *heap) {

    Boolean status = FALSE;
    long newCapacity = heap->capacity * 2;
    size_t bytes = ( newCapacity * sizeof(void *) );
    void **temp = realloc(heap->data, bytes);

    if (temp != NULL) {
        // Update the heap's properties
        heap->data = temp;
        long i;
        for (i = heap->size; i < newCapacity; i++) {
            heap->data[i] = NULL;
        }
        heap->capacity = newCapacity;
        status = TRUE;
    }

    return status;
}

Status heap_insert(Heap *heap, void *item) {

    // Checks the capacity, extend if needed
    if (heap->size == heap->capacity) {
        if (_ensure_capacity(heap) == FALSE) {
            return ALLOC_FAILURE;
        }
    }

    heap->data[heap->size++] = item;
    // Upheap to update the heap
    _upheap(heap);

    return OK;
}

Status heap_peek(Heap *heap, void **min) {

    // Checks if the heap is empty
    if (IS_EMPTY(heap) == TRUE) {
        return STRUCT_EMPTY;
    }
    // Retrieves the min item, saves into pointer
    *min = heap->data[0];

    return OK;
}

Status heap_poll(Heap *heap, void **min) {

    // Checks if the heap is empty
    if (IS_EMPTY(heap) == TRUE) {
        return STRUCT_EMPTY;
    }

    // Retrieves the min item, saves into pointer
    *min = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->data[--heap->size] = NULL;
    // Downheap to update the heap
    _downheap(heap);

    return OK;
}

/**
 * Helper method to clear out the heap `heap` of all its elements, applying the destructor method
 * `destructor` on each element (or if NULL, nothing will be done to the elements).
 */
static void _clear_heap(Heap *heap, void (*destructor)(void *)) {

    long i;
    for (i = 0L; i < heap->size; i++) {
        if (destructor != NULL) {
            (*destructor)(heap->data[i]);
        }
        heap->data[i] = NULL;
    }
}

void heap_clear(Heap *heap, void (*destructor)(void *)) {
    _clear_heap(heap, destructor);
    heap->size = 0;
}

long heap_size(Heap *heap) {
    return heap->size;
}

Boolean heap_isEmpty(Heap *heap) {
    return IS_EMPTY(heap);
}

/**
 * Helper method to generate an array representation of the heap. Returns the allocated array with
 * the populated elements, or NULL if failed (allocation error).
 */
static void **_generate_array(Heap *heap) {

    long i = 0L;
    size_t bytes;
    void **items = NULL;

    // Allocates memory for the array
    bytes = ( heap->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL) {
        return NULL;
    }

    // Populates the array with the heap items
    for (i = 0L; i < heap->size; i++) {
        items[i] = heap->data[i];
    }

    return items;
}

Status heap_toArray(Heap *heap, Array **array) {

    // Do not create the array if currently empty
    if (IS_EMPTY(heap) == TRUE) {
        return STRUCT_EMPTY;
    }

    // Generate the array of heap items
    void **items = _generate_array(heap);
    if (items == NULL) {
        return ALLOC_FAILURE;
    }

    // Allocates memory for the array struct
    Array *temp = (Array *)malloc(sizeof(Array));
    if (temp == NULL) {
        free(items);
        return ALLOC_FAILURE;
    }

    // Initializes the remaining struct members
    temp->items = items;
    temp->len = heap->size;
    *array = temp;

    return OK;
}

Status heap_iterator(Heap *heap, Iterator **iter) {

    Iterator *temp = NULL;

    // Does not create the array if currently empty
    if (IS_EMPTY(heap) == TRUE) {
        return STRUCT_EMPTY;
    }

    // Generates the array of items for iterator
    void **items = _generate_array(heap);
    if (items == NULL) {
        return ALLOC_FAILURE;
    }

    // Creates a new iterator with the items
    Status status = iterator_new(&temp, items, heap->size);
    if (status != OK) {
        free(items);
        return status;
    }
    *iter = temp;

    return OK;
}

void heap_destroy(Heap *heap, void (*destructor)(void *)) {
    _clear_heap(heap, destructor);
    free(heap->data);
    free(heap);
}
