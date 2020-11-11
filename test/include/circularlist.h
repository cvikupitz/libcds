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

#ifndef _CDS_CIRCULAR_LIST_H__
#define _CDS_CIRCULAR_LIST_H__

#include "common.h"
#include "iterator.h"

/**
 * Interface for the CircularList ADT.
 *
 * The CircularList is a linked list where the head and tail are linked, allowing
 * the elements in the list to be 'rotated' in place, mocking a round-robin schema.
 */
typedef struct circular_list CircularList;

/**
 * Constructs a new circular list instance, then stores the new instance into '*list'.
 *
 * Params:
 *    list - The pointer address to store the new CircularList instance.
 * Returns:
 *    STAT_SUCCESS - CircularList was successfully created.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status circularlist_new(CircularList **list);

/**
 * Inserts the specified element into the front of the circular list.
 *
 * Params:
 *    list - The circular list to operate on.
 *    item - The element to insert.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status circularlist_addFirst(CircularList *list, void *item);

/**
 * Appends the specified element to the end of the circular list.
 *
 * Params:
 *    list - The circular list to operate on.
 *    item - The element to insert.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status circularlist_addLast(CircularList *list, void *item);

/**
 * Retrieves, but does not remove, the first element of the circular list, and
 * stores the element into '*first'.
 *
 * Params:
 *    list - The circular list to operate on.
 *    first - The pointer address to store the first element into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Circular list is currently empty.
 */
Status circularlist_first(CircularList *list, void **first);

/**
 * Retrieves, but does not remove, the last element of the circular list, and
 * stores the element into '*last'.
 *
 * Params:
 *    list - The circular list to operate on.
 *    last - The pointer address to store the last element into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Circular list is currently empty.
 */
Status circularlist_last(CircularList *list, void **last);

/**
 * Retrieves and removes the first element of the circular list, and stores the
 * removed element into '*first'.
 *
 * Params:
 *    list - The circular list to operate on.
 *    first - The pointer address to store the removed element into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Circular list is currently empty.
 */
Status circularlist_poll(CircularList *list, void **first);

/**
 * Rotates the elements in the circular list, such that, the first item is moved to
 * the back, and the next element becomes the front.
 *
 * Params:
 *    list - The circular list to operate on.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Circular list is currently empty.
 */
Status circularlist_rotate(CircularList *list);

/**
 * Removes all elements from the circular list. If 'destructor' is not NULL, it will
 * be invoked on each element in the circular list after being removed.
 *
 * Params:
 *    list - The circular list to operate on.
 *    destructor - Function to operate on each element after removal.
 * Returns:
 *    None
 */
void circularlist_clear(CircularList *list, void (*destructor)(void *));

/**
 * Returns the number of elements in the circular list.
 *
 * Params:
 *    list - The circular list to operate on.
 * Returns:
 *    The circular list's current size.
 */
long circularlist_size(CircularList *list);

/**
 * Returns TRUE if the circular list contains no elements, FALSE if otherwise.
 *
 * Params:
 *    list - The circular list to operate on.
 * Returns:
 *    TRUE if the circular list is empty, FALSE if not.
 */
Boolean circularlist_isEmpty(CircularList *list);

/**
 * Allocates and generates an array containing all of the circular list's elements
 * in proper sequence (from first to last element), then stores the array into '*array'.
 * Caller is responsible for freeing the array when finished.
 *
 * Params:
 *    list - The circular list to operate on.
 *    array - Address where the new array will be stored.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Circular list is currently empty.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status circularlist_toArray(CircularList *list, Array **array);

/**
 * Creates an Iterator instance to iterate over the circular list's elements in proper
 * sequence (from first to last element), then stores the iterator into '*iter'. Caller
 * is responsible for destroying the iterator instance when finished.
 *
 * Params:
 *    list - The circular list to operate on.
 *    iter - Address where the new iterator will be stored.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Circular list is currently empty.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status circularlist_iterator(CircularList *list, Iterator **iter);

/**
 * Destroys the circular list instance by freeing all of its reserved memory. If
 * 'destructor' is not NULL, it will be invoked on each element before the circular
 * list is destroyed.
 *
 * Params:
 *    list - The circular list to operate on.
 *    destructor - Function to operate on each element prior to circular list destruction.
 * Returns:
 *    None
 */
void circularlist_destroy(CircularList *list, void (*destructor)(void *));

#endif  /* _CDS_CIRCULAR_LIST_H__ */
