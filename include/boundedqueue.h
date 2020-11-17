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

#ifndef _CDS_BOUNDED_QUEUE_H__
#define _CDS_BOUNDED_QUEUE_H__

#include "cds_common.h"
#include "iterator.h"

/**
 * Interface for the BoundedQueue ADT.
 *
 * The BoundedQueue class represents a first-in-first-out (FIFO) queue of objects.
 * In addition, the queue is bounded, meaning that it can only hold a set number of
 * elements at a time.
 *
 * Modeled after the Java 7 Queue interface.
 */
typedef struct bounded_queue BoundedQueue;

/**
 * Creates a new bounded queue instance, then stores the new instance into '*queue'.
 * If the capacity specified is <= 0, a default capacity is assigned.
 *
 * Params:
 *    queue - The pointer address to store the new BoundedQueue instance.
 *    capacity - The queue's upper-bound capacity.
 * Returns:
 *    STAT_SUCCESS - Queue was successfully created.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status boundedqueue_new(BoundedQueue **queue, long capacity);

/**
 * Inserts the specified element into the queue.
 *
 * Params:
 *    queue - The queue to operate on.
 *    item - The item to be inserted into the queue.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_FULL - Failed as the queue is currently full.
 */
Status boundedqueue_add(BoundedQueue *queue, void *item);

/**
 * Retrieves, but does not remove, the first element from the queue and stores the
 * result into '*first'.
 *
 * Params:
 *    queue - The queue to operate on.
 *    first - The pointer address to store the first element into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Queue is currently empty.
 */
Status boundedqueue_peek(BoundedQueue *queue, void **first);

/**
 * Removes the first element from the queue and stores the result into '*first'.
 *
 * Params:
 *    queue - The queue to operate on.
 *    first - The pointer address to store the removed first element into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Queue is currently empty.
 */
Status boundedqueue_poll(BoundedQueue *queue, void **first);

/**
 * Removes all elements from the queue. If 'destructor' is not NULL, it will be
 * invoked on each element in the queue after being removed.
 *
 * Params:
 *    queue - The queue to operate on.
 *    destructor - Function to operate on each element after removal.
 * Returns:
 *    None
 */
void boundedqueue_clear(BoundedQueue *queue, void (*destructor)(void *));

/**
 * Returns the number of elements in the queue.
 *
 * Params:
 *    queue - The queue to operate on.
 * Returns:
 *    The queue's current size.
 */
long boundedqueue_size(BoundedQueue *queue);

/**
 * Returns the queue's capacity, that is, the maximum elements it can hold.
 *
 * Params:
 *    queue - The queue to operate on.
 * Returns:
 *    The queue's capacity.
 */
long boundedqueue_capacity(BoundedQueue *queue);

/**
 * Returns TRUE if the queue contains no elements, FALSE if otherwise.
 *
 * Params:
 *    queue - The queue to operate on.
 * Returns:
 *    TRUE if the queue is empty, FALSE if not.
 */
Boolean boundedqueue_isEmpty(BoundedQueue *queue);

/**
 * Returns TRUE if the queue is currently full, FALSE if otherwise.
 *
 * Params:
 *    queue - The queue to operate on.
 * Returns:
 *    TRUE if the queue is full, FALSE if not.
 */
Boolean boundedqueue_isFull(BoundedQueue *queue);

/**
 * Allocates and generates an array containing all of the queue's elements in proper
 * sequence (from the first to last element), then stores the array into '*array'.
 * Caller is responsible for freeing the array when finished.
 *
 * Params:
 *    queue - The queue to operate on.
 *    array - Address where the new array will be stored.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Queue is currently empty.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status boundedqueue_toArray(BoundedQueue *queue, Array **array);

/**
 * Creates an Iterator instance to iterate over the queue's elements in proper
 * sequence (from first to last element), then stores the iterator into '*iter'.
 * Caller is responsible for destroying the iterator instance when finished.
 *
 * Params:
 *    queue - The queue to operate on.
 *    iter - Address where the new iterator will be stored.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Queue is currently empty.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status boundedqueue_iterator(BoundedQueue *queue, Iterator **iter);

/**
 * Destroys the queue instance by freeing all of its reserved memory. If
 * 'destructor' is not NULL, it will be invoked on each element before the queue
 * is destroyed.
 *
 * Params:
 *    queue - The queue to destroy.
 *    destructor - Function to operate on each element prior to queue destruction.
 * Returns:
 *    None
 */
void boundedqueue_destroy(BoundedQueue *queue, void (*destructor)(void *));

#endif  /* _CDS_BOUNDED_QUEUE_H__ */
