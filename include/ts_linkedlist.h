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

#ifndef _CDS_TS_LINKEDLIST_H__
#define _CDS_TS_LINKEDLIST_H__

#include "cds_common.h"
#include "ts_iterator.h"

/**
 * Interface for the thread-safe LinkedList ADT.
 *
 * Doubly-linked list implementation. Implements all optional list operations, and
 * permits all elements (including null).
 *
 * Modeled after the Java 7 LinkedList interface.
 */
typedef struct ts_linkedlist ConcurrentLinkedList;

/**
 * Constructs a new linked list instance, then stores the new instance into '*list'.
 *
 * Params:
 *    None
 * Returns:
 *    STAT_SUCCESS - LinkedList was successfully created.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_linkedlist_new(ConcurrentLinkedList **list);

/**
 * Locks the linked list, providing exclusive access to the calling thread. Caller
 * is responsible for unlocking the linked list to allow other threads access.
 *
 * Params:
 *    list - The linked list to operate on.
 * Returns:
 *    None
 */
void ts_linkedlist_lock(ConcurrentLinkedList *list);

/**
 * Unlocks the linked list, releasing the exclusive access from the calling thread.
 *
 * Params:
 *    list - The linked list to operate on.
 * Returns:
 *    None
 */
void ts_linkedlist_unlock(ConcurrentLinkedList *list);

/**
 * Inserts the specified element at the beginning of the linked list.
 *
 * Params:
 *    list - The linked list to operate on.
 *    item - The element to add.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_linkedlist_addFirst(ConcurrentLinkedList *list, void *item);

/**
 * Appends the specified element to the end of the linked list.
 *
 * Params:
 *    list - The linked list to operate on.
 *    item - The element to add.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_linkedlist_addLast(ConcurrentLinkedList *list, void *item);

/**
 * Inserts the specified element at the specified position in this list. Shifts the
 * element currently at that position (if any) and any subsequent elements to the
 * right (adds one to their indices).
 *
 * Params:
 *    list - The linked list to operate on.
 *    i - The index at which the specified element is to be inserted.
 *    item - The element to be inserted.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_INVALID_INDEX - Index given is out of range (index < 0 || index > size()).
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_linkedlist_insert(ConcurrentLinkedList *list, long i, void *item);

/**
 * Retrieves the first element from the linked list without removing it, and stores
 * the element into '*first'.
 *
 * Params:
 *    list - The linked list to operate on.
 *    first - The pointer address to store the first element into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Linked list is currently empty.
 */
Status ts_linkedlist_first(ConcurrentLinkedList *list, void **first);

/**
 * Retrieves the last element from the linked list without removing it, and stores
 * the element into '*last'.
 *
 * Params:
 *    list - The linked list to operate on.
 *    last - The pointer address to store the last element into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Linked list is currently empty.
 */
Status ts_linkedlist_last(ConcurrentLinkedList *list, void **last);

/**
 * Retrieves the element at the specified index from the linked list without removing
 * it, and stores the element into '*item'.
 *
 * Params:
 *    list - The linked list to operate on.
 *    i - The index of the element to return.
 *    item - The pointer address to store the fetched element into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Linked list is currently empty.
 *    STAT_INVALID_INDEX - Index given is out of range (index < 0 || index >= size()).
 */
Status ts_linkedlist_get(ConcurrentLinkedList *list, long i, void **item);

/**
 * Replaces the element at the specified position in the linked list with the specified
 * element. The old element is then placed into '*previous'.
 *
 * Params:
 *    list - The linked list to operate on.
 *    i - The index of the element to replace
 *    item - The element to be stored at the specified position
 *    previous - The pointer address to store the element previously at the specified
 *               position.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Linked list is currently empty.
 *    STAT_INVALID_INDEX - Index given is out of range (index < 0 || index >= size()).
 */
Status ts_linkedlist_set(ConcurrentLinkedList *list, long i, void *item, void **previous);

/**
 * Removes the first element from the linked list, and stores the result into '*first'.
 *
 * Params:
 *    list - The linked list to operate on.
 *    first - The pointer address to store the removed element into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Linked list is currently empty.
 */
Status ts_linkedlist_removeFirst(ConcurrentLinkedList *list, void **first);

/**
 * Removes the last element from the linked list, and stores the result into '*last'.
 *
 * Params:
 *    list - The linked list to operate on.
 *    last - The pointer address to store the removed element into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Linked list is currently empty.
 */
Status ts_linkedlist_removeLast(ConcurrentLinkedList *list, void **last);

/**
 * Removes the element at the specified position in the linked list. Shifts any
 * subsequent elements to the left (subtracts one from their indices). Stores the
 * removed element into '*item'.
 *
 * Params:
 *    list - The linked list to operate on.
 *    i - The index of the element to remove.
 *    item - The pointer address to store the element previously at the specified position.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Linked list is currently empty.
 *    STAT_INVALID_INDEX - Index given is out of range (index < 0 || index >= size()).
 */
Status ts_linkedlist_remove(ConcurrentLinkedList *list, long i, void **item);

/**
 * Removes all elements from the linked list. If 'destructor' is not NULL, it will be
 * invoked on each element in the linked list after being removed.
 *
 * Params:
 *    list - The linked list to operate on.
 *    destructor - Function to operate on each element after removal.
 * Returns:
 *    None
 */
void ts_linkedlist_clear(ConcurrentLinkedList *list, void (*destructor)(void *));

/**
 * Returns the number of elements in the linked list.
 *
 * Params:
 *    list - The linked list to operate on.
 * Returns:
 *    The linked list's current size.
 */
long ts_linkedlist_size(ConcurrentLinkedList *list);

/**
 * Returns TRUE if the linked list contains no elements, FALSE if otherwise.
 *
 * Params:
 *    list - The linked list to operate on.
 * Returns:
 *    TRUE if the linked list is empty, FALSE if not.
 */
Boolean ts_linkedlist_isEmpty(ConcurrentLinkedList *list);

/**
 * Allocates and generates an array containing all of the elements in the linked list in
 * proper sequence (from first to last element), then stores the array into '*array'. Caller
 * is responsible for freeing the array when finished.
 *
 * Params:
 *    list - The linked list to operate on.
 *    array - Address where the new array will be stored.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Linked list is currently empty.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_linkedlist_toArray(ConcurrentLinkedList *list, Array **array);

/**
 * Createss an Iterator instance to iterate over the linked list's elements in proper
 * sequence (from first to last element), then stores the iterator into '*iter'. Caller
 * is responsible for destroying the iterator instance when finished.
 *
 * Params:
 *    list - The linked list to operate on.
 *    iter - Address where the new iterator will be stored.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Linked list is currently empty.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_linkedlist_iterator(ConcurrentLinkedList *list, ConcurrentIterator **iter);

/**
 * Destroys the linked list instance by freeing all of its reserved memory. If 'destructor'
 * is not NULL, it will be invoked on each element before the linked list is destroyed.
 *
 * Params:
 *    list - The linked list to destroy.
 *    destructor - Function to operate on each element prior to linked list destruction.
 * Returns:
 *    None
 */
void ts_linkedlist_destroy(ConcurrentLinkedList *list, void (*destructor)(void *));

#endif  /* _CDS_TS_LINKEDLIST_H__ */
