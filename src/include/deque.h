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

#ifndef _DEQUE_H__
#define _DEQUE_H__

#include "common.h"
#include "iterator.h"

/**
 * Interface for the Deque ADT.
 *
 * The Deque class represents a linear collection that supports element insertion
 * and removal at both ends.
 *
 * Modeled after the Java 7 Deque interface.
 */
typedef struct deque Deque;

/**
 * Constructs a new deque instance, then stores the new instance into '*deque'.
 *
 * Params:
 *    deque - The pointer address to store the new Deque instance.
 * Returns:
 *    STAT_SUCCESS - Deque was successfully created.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status deque_new(Deque **deque);

/**
 * Inserts the specified element into the front of the deque.
 *
 * Params:
 *    deque - The deque to operate on.
 *    item - The element to insert into the deque.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status deque_addFirst(Deque *deque, void *item);

/**
 * Inserts the specified element into the back of the deque.
 *
 * Params:
 *    deque - The deque to operate on.
 *    item - The element to insert into the deque.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status deque_addLast(Deque *deque, void *item);

/**
 * Retrieves, but does not remove, the head of the deque, and stores the element
 * into '*first'.
 *
 * Params:
 *    deque - The deque to operate on.
 *    first - The pointer address to store the first element into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Deque is currently empty.
 */
Status deque_first(Deque *deque, void **first);

/**
 * Retrieves, but does not remove, the tail of the deque, and stores the element
 * into '*last'.
 *
 * Params:
 *    deque - The deque to operate on.
 *    last - The pointer address to store the last element into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Deque is currently empty.
 */
Status deque_last(Deque *deque, void **last);

/**
 * Retrieves and removes the head of the deque, and stores the removed element
 * into '*first'.
 *
 * Params:
 *    deque - The deque to operate on.
 *    first - The pointer address to store the removed element into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Deque is currently empty.
 */
Status deque_removeFirst(Deque *deque, void **first);

/**
 * Retrieves and removes the tail of the deque, and stores the removed element
 * into '*last'.
 *
 * Params:
 *    deque - The deque to operate on.
 *    last - The pointer address to store the removed element into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Deque is currently empty.
 */
Status deque_removeLast(Deque *deque, void **last);

/**
 * Removes all elements from the deque. If 'destructor' is not NULL, it will be
 * invoked on each element in the deque after being removed.
 *
 * Params:
 *    deque - The deque to operate on.
 *    destructor - Function to operate on each element after removal.
 * Returns:
 *    None
 */
void deque_clear(Deque *deque, void (*destructor)(void *));

/**
 * Returns the number of elements in the deque.
 *
 * Params:
 *    deque - The deque to operate on.
 * Returns:
 *    The deque's current size.
 */
long deque_size(Deque *deque);

/**
 * Returns TRUE if the deque contains no elements, FALSE if otherwise.
 *
 * Params:
 *    deque - The deque to operate on.
 * Returns:
 *    TRUE if the deque is empty, FALSE if not.
 */
Boolean deque_isEmpty(Deque *deque);

/**
 * Allocates and generates an array containing all of the deque's elements in proper
 * sequence (from head to tail element), then stores the array into '*array'. Caller is
 * responsible for freeing the array when finished.
 *
 * Params:
 *    deque - The deque to operate on.
 *    array - Address where the new array will be stored.
 *    len - Address where the length of the generated array will be stored.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Deque is currently empty.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status deque_toArray(Deque *deque, Array **array);

/**
 * Creates an Iterator instance to iterate over the deque's elements in proper sequence
 * (from head to tail element), then stores the iterator into '*iter'. Caller is responsible
 * for destroying the iterator instance when finished.
 *
 * Params:
 *    deque - The deque to operate on.
 *    iter - Address where the new iterator will be stored.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Deque is currently empty.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status deque_iterator(Deque *deque, Iterator **iter);

/**
 * Destroys the deque instance by freeing all of its reserved memory. If 'destructor'
 * is not NULL, it will be invoked on each element before the deque is destroyed.
 *
 * Params:
 *    deque - The deque to destroy.
 *    destructor - Function to operate on each element prior to deque destruction.
 * Returns:
 *    None
 */
void deque_destroy(Deque *deque, void (*destructor)(void *));

#endif  /* _DEQUE_H__ */
