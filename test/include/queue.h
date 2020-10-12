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

#ifndef _QUEUE_H__
#define _QUEUE_H__

#include "common.h"
#include "iterator.h"

/**
 * Interface for the Queue ADT.
 *
 * The Queue class represents a first-in-first-out (FIFO) queue of objects. This
 * is an unbounded version, meaning that the queue does not impose a capacity on
 * the number of items it may contain. Inserts can be performed as long as memory
 * or the system allows it.
 *
 * Modeled after the Java 7 Queue interface.
 */
typedef struct queue Queue;

/**
 * Creates a new queue instance, then stores the new instance into '*queue'.
 *
 * Params:
 *    queue - The pointer address to store the new Queue instance.
 * Returns:
 *    STAT_SUCCESS - Queue was successfully created.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status queue_new(Queue **queue);

/**
 * Inserts the specified element into the queue.
 *
 * Params:
 *    queue - The queue to operate on.
 *    item - The item to be inserted into the queue.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status queue_enqueue(Queue *queue, void *item);

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
Status queue_peek(Queue *queue, void **first);

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
Status queue_dequeue(Queue *queue, void **first);

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
void queue_clear(Queue *queue, void (*destructor)(void *));

/**
 * Returns the number of elements in the queue.
 *
 * Params:
 *    queue - The queue to operate on.
 * Returns:
 *    The queue's current size.
 */
long queue_size(Queue *queue);

/**
 * Returns TRUE if the queue contains no elements, FALSE if otherwise.
 *
 * Params:
 *    queue - The queue to operate on.
 * Returns:
 *    TRUE if the queue is empty, FALSE if not.
 */
Boolean queue_isEmpty(Queue *queue);

/**
 * Allocates and generates an array containing all of the queue's elements in proper
 * sequence (from the first to last element), then stores the array into '*array'.
 * Caller is responsible for freeing the array when finished.
 *
 * Params:
 *    queue - The queue to operate on.
 *    array - Address where the generated array will be stored.
 *    len - Address where the length of the generated array will be stored.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Queue is currently empty.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status queue_toArray(Queue *queue, void ***array, long *len);

/**
 * Creates an Iterator instance to iterate over the queue's elements in proper sequence
 * (from first to last element), then stores the iterator into '*iter'. Caller is
 * responsible for destroying the iterator instance when finished.
 *
 * Params:
 *    queue - The queue to operate on.
 *    iter - Address where the new iterator will be stored.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Queue is currently empty.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status queue_iterator(Queue *queue, Iterator **iter);

/**
 * Destroys the queue instance by freeing all of its reserved memory. If 'destructor'
 * is not NULL, it will be invoked on each element before the queue is destroyed.
 *
 * Params:
 *    queue - The queue to destroy.
 *    destructor - Function to operate on each element prior to queue destruction.
 * Returns:
 *    None
 */
void queue_destroy(Queue *queue, void (*destructor)(void *));

#endif  /* _QUEUE_H__ */