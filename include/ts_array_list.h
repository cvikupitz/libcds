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

#ifndef _CDS_TS_ARRAYLIST_H__
#define _CDS_TS_ARRAYLIST_H__

#include "cds_common.h"
#include "ts_iterator.h"

/**
 * Interface for the thread-safe ArrayList ADT.
 *
 * The ArrayList class is a collection of elements stored in a resizable array.
 *
 * Modeled after the Java 7 ArrayList interface.
 */
typedef struct ts_arraylist ConcurrentArrayList;

/**
 * Constructs a new array list instance with the specified starting capacity, then stores the new
 * instance into `*list`. If the capacity given is <= 0, a default capacity is assigned.
 *
 * Params:
 *    list - The pointer address to store the new ArrayList instance.
 *    capacity - The default capacity of the arraylist.
 * Returns:
 *    OK - ArrayList was successfully created.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_arraylist_new(ConcurrentArrayList **list, long capacity);

/**
 * Locks the arraylist, providing exclusive access to the calling thread. Caller is responsible for
 * unlocking the list to allow other threads access.
 *
 * Params:
 *    list - The arraylist to operate on.
 * Returns:
 *    None
 */
void ts_arraylist_lock(ConcurrentArrayList *list);

/**
 * Unlocks the arraylist, releasing the exclusive access from the calling thread.
 *
 * Params:
 *    list - The arraylist to operate on.
 * Returns:
 *    None
 */
void ts_arraylist_unlock(ConcurrentArrayList *list);

/**
 * Appends the specified element to the end of the array list.
 *
 * Params:
 *    list - The array list to operate on.
 *    item - The element to be appended to the array list.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_arraylist_add(ConcurrentArrayList *list, void *item);

/**
 * Inserts the specified element at the specified position in the array list. Shifts the element
 * currently at that position (if any) and any subsequent elements to the right (adds one to their
 * indices).
 *
 * Params:
 *    list - The array list to operate on.
 *    i - The index at which the specified element is to be inserted.
 *    item - The element to be inserted.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index given is invalid:
 *       1.) `i` < 0
 *       2.) `i` > size
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_arraylist_insert(ConcurrentArrayList *list, long i, void *item);

/**
 * Returns the element at the specified position in the array list.
 *
 * Params:
 *    list - The array list to operate on.
 *    i - The index of the element to retrieve.
 *    item - The pointer address to store the retrieved element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Array list is currently empty.
 *    INVALID_INDEX - Index given is invalid:
 *       1.) `i` < 0
 *       2.) `i` >= size
 */
Status ts_arraylist_get(ConcurrentArrayList *list, long i, void **item);

/**
 * Replaces the element at the specified position in the array list with the specified element.
 *
 * Params:
 *    list - The array list to operate on.
 *    i - The index of the element to replace.
 *    item - The element to be stored at the specified position.
 *    previous - The pointer address to store the element previously at the specified
 *               position.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Array list is currently empty.
 *    INVALID_INDEX - Index given is invalid:
 *       1.) `i` < 0
 *       2.) `i` >= size
 */
Status ts_arraylist_set(ConcurrentArrayList *list, long i, void *item, void **previous);

/**
 * Removes the element at the specified position in the array list. Shifts any subsequent elements
 * to the left (subtracts one from their indices).
 *
 * Params:
 *    list - The array list to operate on.
 *    i - The index of the element to be removed.
 *    item - The pointer address to store the element that was removed from the list.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Array list is currently empty.
 *    INVALID_INDEX - Index given is invalid:
 *       1.) `i` < 0
 *       2.) `i` >= size
 */
Status ts_arraylist_remove(ConcurrentArrayList *list, long i, void **item);

/**
 * Increases the capacity of the array list, if necessary, to ensure that it can hold at least the
 * number of elements specified by the minimum capacity argument.
 *
 * Params:
 *    list - The array list to operate on.
 *    capacity - The desired minimum capacity.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_arraylist_ensureCapacity(ConcurrentArrayList *list, long capacity);

/**
 * Trims the capacity of the array list instance to be the list's current size.
 *
 * Params:
 *    list - The array list to operate on.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - ArrayList is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_arraylist_trimToSize(ConcurrentArrayList *list);

/**
 * Removes all elements from the array list. If `destructor` is not NULL, it will be invoked on each
 * element in the array list after being removed.
 *
 * Params:
 *    list - The array list to operate on.
 *    destructor - Function to operate on each element after removal.
 * Returns:
 *    None
 */
void ts_arraylist_clear(ConcurrentArrayList *list, void (*destructor)(void *));

/**
 * Returns the number of elements in the array list.
 *
 * Params:
 *    list - The array list to operate on.
 * Returns:
 *    The array list's current size.
 */
long ts_arraylist_size(ConcurrentArrayList *list);

/**
 * Returns the array list's current capacity, that is, the maximum number of elements it can hold
 * before resizing is required.
 *
 * Params:
 *    list - The array list to operate on.
 * Returns:
 *    The array list's current capacity.
 */
long ts_arraylist_capacity(ConcurrentArrayList *list);

/**
 * Returns TRUE if the array list contains no elements, FALSE if otherwise.
 *
 * Params:
 *    list - The array list to operate on.
 * Returns:
 *    TRUE if the array list is empty, FALSE if not.
 */
Boolean ts_arraylist_isEmpty(ConcurrentArrayList *list);

/**
 * Allocates and generates an array containing all of the array list's elements in proper sequence
 * (from first to last element), then stores the array into `*array`. Caller is responsible for
 * freeing the array when finished.
 *
 * Params:
 *    list - The array list to operate on.
 *    array - Address where the new array will be stored.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Array list is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_arraylist_toArray(ConcurrentArrayList *list, Array **array);

/**
 * Creates an Iterator instance to iterate over the array list's elements in proper sequence (from
 * first to last element), then stores the iterator into `*iter`. Caller is responsible for
 * destroying the iterator instance when finished.
 *
 * Params:
 *    list - The array list to operate on.
 *    iter - Address where the new iterator will be stored.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Array list is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_arraylist_iterator(ConcurrentArrayList *list, ConcurrentIterator **iter);

/**
 * Destroys the array list instance by freeing all of its reserved memory. If `destructor` is not
 * NULL, it will be invoked on each element before the arraylist is destroyed.
 *
 * Params:
 *    list - The array list to destroy.
 *    destructor - Function to operate on each element prior to array list destruction.
 * Returns:
 *    None
 */
void ts_arraylist_destroy(ConcurrentArrayList *list, void (*destructor)(void *));

#endif  /* _CDS_TS_ARRAYLIST_H__ */
