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

#ifndef _CDS_HEAP_H__
#define _CDS_HEAP_H__

#include "cds_common.h"
#include "iterator.h"

/**
 * Interface for the Heap ADT.
 *
 * A heap is a priority queue, where the elements of the priority queue are ordered
 * according to a comparator function provided at construction time. The head of the
 * queue is the least element with respect to the specified ordering.
 *
 * Modeled after the Java 7 PriorityQueue interface.
 */
typedef struct heap Heap;

/**
 * Constructs a new empty heap instance with the specified capacity, then stores the new
 * instance into '*heap'. If the capacity given is <= 0, a default capacity is assigned.
 * The capacity is the starting array capacity; it will resize itself prior to insertions
 * when the capacity is met.
 *
 * The comparator function specified should return an integer comparing the two specified
 * values, such that cmp(a, b) returns 0 when a == b, <0 when a < b, and >0 when a > b.
 *
 * Params:
 *    heap - The pointer address to store the new Heap instance.
 *    capacity - The heap's starting capacity.
 *    comparator - Function for comparing two items in the heap.
 * Returns:
 *    OK - Heap was successfully created.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status heap_new(Heap **heap, long capacity, int (*comparator)(void *, void *));

/**
 * Inserts the specified element into the heap.
 *
 * Params:
 *    heap - The heap to operate on.
 *    item - The element to add.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status heap_insert(Heap *heap, void *item);

/**
 * Retrieves, but does not remove, the top of heap, and stores the element into '*min'.
 *
 * Params:
 *    heap - The heap to operate on.
 *    min - The pointer address to store the head element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Heap is currently empty.
 */
Status heap_peek(Heap *heap, void **min);

/**
 * Removes the top element from the heap, and stores the result into '*min'.
 *
 * Params:
 *    heap - The heap to operate on.
 *    min - The pointer address to store the removed element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Heap is currently empty.
 */
Status heap_poll(Heap *heap, void **min);

/**
 * Removes all elements from the heap. If 'destructor' is not NULL, it will be
 * invoked on each element in the heap after being removed.
 *
 * Params:
 *    heap - The heap to operate on.
 *    destructor - Function to operate on each element after removal.
 * Returns:
 *    None
 */
void heap_clear(Heap *heap, void (*destructor)(void *));

/**
 * Returns the number of elements in the heap.
 *
 * Params:
 *    heap - The heap to operate on.
 * Returns:
 *    The heap's current size.
 */
long heap_size(Heap *heap);

/**
 * Returns TRUE if the heap contains no elements, FALSE if otherwise.
 *
 * Params:
 *    heap - The heap to operate on.
 * Returns:
 *    TRUE if the heap is empty, FALSE if not.
 */
Boolean heap_isEmpty(Heap *heap);

/**
 * Allocates and generates an array containing all of the heap's elements in proper
 * sequence (through a breadth-first traversal), then stores the array into '*array'.
 * Caller is responsible for freeing the array when finished.
 *
 * Params:
 *    heap - The heap to operate on.
 *    array - Address where the new array will be stored.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Heap is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status heap_toArray(Heap *heap, Array **array);

/**
 * Creates an Iterator instance to iterate over the heap's elements in proper sequence
 * (through a breadth-first traversal), then stores the iterator into '*iter'. Caller is
 * responsible for destroying the iterator instance when finished.
 *
 * Params:
 *    heap - The heap to operate on.
 *    iter - Address where the new iterator will be stored.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Heap is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status heap_iterator(Heap *heap, Iterator **iter);

/**
 * Destroys the heap instance by freeing all of its reserved memory. If 'destructor'
 * is not NULL, it will be invoked on each element before the heap is destroyed.
 *
 * Params:
 *    heap - The heap to destroy.
 *    destructor - Function to operate on each element prior to heap destruction.
 * Returns:
 *    None
 */
void heap_destroy(Heap *heap, void (*destructor)(void *));

#endif  /* _CDS_HEAP_H__ */
