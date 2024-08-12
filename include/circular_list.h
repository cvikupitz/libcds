/**
 * MIT License
 *
 * Copyright (c) 2020 Cole Vikupitz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _CDS_CIRCULAR_LIST_H__
#define _CDS_CIRCULAR_LIST_H__

#include "cds_common.h"
#include "iterator.h"

/**
 * Interface for the CircularList ADT.
 *
 * The CircularList is a linked list where the head and tail are linked, allowing the elements in
 * the list to be `rotated` in place. Provides a similar API as the LinkedList, and in addition
 * supports rotating the elements in the list.
 */
typedef struct circular_list CircularList;

/**
 * Constructs a new circular list instance, then stores the new instance into `*list`.
 *
 * Params:
 *    list - The pointer address to store the new CircularList instance.
 * Returns:
 *    OK - CircularList was successfully created.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status circularlist_new(CircularList **list);

/**
 * Inserts the specified element into the front of the circular list.
 *
 * Params:
 *    list - The circular list to operate on.
 *    item - The element to insert.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status circularlist_addFirst(CircularList *list, void *item);

/**
 * Inserts the specified element to the end of the circular list.
 *
 * Params:
 *    list - The circular list to operate on.
 *    item - The element to insert.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status circularlist_addLast(CircularList *list, void *item);

/**
 * Inserts the specified element at the specified position in the circular list. Shifts the element
 * currently at that position (if any) and any subsequent elements to the right (adds one to their
 * indices).
 *
 * Params:
 *    list - The circular list to operate on.
 *    i - The index at which the specified element is to be inserted.
 *    item - The element to be inserted.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index given is invalid:
 *       1.) `i` < 0
 *       2.) `i` >= size
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status circularlist_insert(CircularList *list, long i, void *item);

/**
 * Retrieves, but does not remove, the first element of the circular list, and stores the element
 * into `*first`.
 *
 * Params:
 *    list - The circular list to operate on.
 *    first - The pointer address to store the first element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Circular list is currently empty.
 */
Status circularlist_first(CircularList *list, void **first);

/**
 * Retrieves, but does not remove, the last element of the circular list, and stores the element
 * into `*last`.
 *
 * Params:
 *    list - The circular list to operate on.
 *    last - The pointer address to store the last element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Circular list is currently empty.
 */
Status circularlist_last(CircularList *list, void **last);

/**
 * Retrieves the element at the specified index from the circular list without removing it, and
 * stores the element into `*item`.
 *
 * Params:
 *    list - The circular list to operate on.
 *    i - The index of the element to return.
 *    item - The pointer address to store the fetched element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Circular list is currently empty.
 *    INVALID_INDEX - Index given is invalid:
 *       1.) `i` < 0
 *       2.) `i` >= size
 */
Status circularlist_get(CircularList *list, long i, void **item);

/**
 * Replaces the element at the specified position in the circular list with the specified element.
 * The old element is then placed into `*previous`.
 *
 * Params:
 *    list - The circular list to operate on.
 *    i - The index of the element to replace
 *    item - The element to be stored at the specified position
 *    previous - The pointer address to store the element previously at the specified
 *               position.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Circular list is currently empty.
 *    INVALID_INDEX - Index given is invalid:
 *       1.) `i` < 0
 *       2.) `i` >= size
 */
Status circularlist_set(CircularList *list, long i, void *item, void **previous);

/**
 * Retrieves and removes the first element of the circular list, and stores the removed element into
 * `*first`.
 *
 * Params:
 *    list - The circular list to operate on.
 *    first - The pointer address to store the removed element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Circular list is currently empty.
 */
Status circularlist_removeFirst(CircularList *list, void **first);

/**
 * Retrieves and removes the last element of the circular list, and stores the removed element into
 * `*last`.
 *
 * Params:
 *    list - The circular list to operate on.
 *    first - The pointer address to store the removed element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Circular list is currently empty.
 */
Status circularlist_removeLast(CircularList *list, void **last);

/**
 * Removes the element at the specified position in the circular list. Shifts any subsequent
 * elements to the left (subtracts one from their indices). Stores the removed element into `*item`.
 *
 * Params:
 *    list - The circular list to operate on.
 *    i - The index of the element to remove.
 *    item - The pointer address to store the element previously at the specified position.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Circular list is currently empty.
 *    INVALID_INDEX - Index given is invalid:
 *       1.) `i` < 0
 *       2.) `i` >= size
 */
Status circularlist_remove(CircularList *list, long i, void **item);

/**
 * Rotates the elements in the circular list forward, such that, the first item is moved to the
 * back, and the next element becomes the front. If the list is currently empty, no rotations are
 * performed.
 *
 * Params:
 *    list - The circular list to operate on.
 * Returns:
 *    None
 */
void circularlist_rotateForward(CircularList *list);

/**
 * Rotates the elements in the circular list backward, such that, the last item is moved to the
 * front, and the previous element becomes the last. If the list is currently empty, no rotations
 * are performed.
 *
 * Params:
 *    list - The circular list to operate on.
 * Returns:
 *    None
 */
void circularlist_rotateBackward(CircularList *list);

/**
 * Removes all elements from the circular list. If `destructor` is not NULL, it will be invoked on
 * each element in the circular list after being removed.
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
 * Allocates and generates an array containing all of the circular list's elements in proper
 * sequence (from first to last element), then stores the array into `*array`. Caller is responsible
 * for freeing the array when finished.
 *
 * Params:
 *    list - The circular list to operate on.
 *    array - Address where the new array will be stored.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Circular list is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status circularlist_toArray(CircularList *list, Array **array);

/**
 * Creates an Iterator instance to iterate over the circular list's elements in proper sequence
 * (from first to last element), then stores the iterator into `*iter`. Caller is responsible for
 * destroying the iterator instance when finished.
 *
 * Params:
 *    list - The circular list to operate on.
 *    iter - Address where the new iterator will be stored.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Circular list is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status circularlist_iterator(CircularList *list, Iterator **iter);

/**
 * Destroys the circular list instance by freeing all of its reserved memory. If `destructor` is not
 * NULL, it will be invoked on each element before the circular list is destroyed.
 *
 * Params:
 *    list - The circular list to operate on.
 *    destructor - Function to operate on each element prior to circular list destruction.
 * Returns:
 *    None
 */
void circularlist_destroy(CircularList *list, void (*destructor)(void *));

#endif  /* _CDS_CIRCULAR_LIST_H__ */
