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

#ifndef _CDS_TS_TREESET_H__
#define _CDS_TS_TREESET_H__

#include "cds_common.h"
#include "ts_iterator.h"

/**
 * Declaration for the thread-safe TreeSet ADT.
 *
 * A red-black tree set storing elements based on their natural ordering defined
 * through a comparator provided at construction time. Provides self-balancing
 * capabilities for even distribution.
 *
 * Modeled after the Java 7 TreeSet interface.
 */
typedef struct ts_treeset ConcurrentTreeSet;

/**
 * Constructs a new treeset, then stores the new instance into '*tree'.
 *
 * The comparator function specified should return an integer comparing the two
 * specified values, such that cmp(a, b) returns 0 when a == b, <0 when a < b, and
 * >0 when a > b.
 *
 * Params:
 *    tree - The pointer address to store the new TreeSet instance.
 *    comparator - Function for comparing two items in the treeset.
 * Returns:
 *    OK - TreeSet was successfully created.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_treeset_new(ConcurrentTreeSet **tree, int (*comparator)(void *, void *));

/**
 * Locks the treeset, providing exclusive access to the calling thread. Caller
 * is responsible for unlocking the treeset to allow other threads access.
 *
 * Params:
 *    tree - The tree to operate on.
 * Returns:
 *    None
 */
void ts_treeset_lock(ConcurrentTreeSet *tree);

/**
 * Unlocks the treeset, releasing the exclusive access from the calling thread.
 *
 * Params:
 *    tree - The treeset to operate on.
 * Returns:
 *    None
 */
void ts_treeset_unlock(ConcurrentTreeSet *tree);

/**
 * Adds the specified element to the treeset if it is not already present.
 *
 * Params:
 *    tree - The treeset to operate on.
 *    item - The element to be added to the treeset.
 * Returns:
 *    OK - Operation was successful.
 *    ALREADY_EXISTS - Specified entry is already present.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_treeset_add(ConcurrentTreeSet *tree, void *item);

/**
 * Returns TRUE if the treeset contains the specified element, FALSE if not.
 *
 * Params:
 *    tree - The treeset to operate on.
 *    item - The item to search for.
 * Returns:
 *    TRUE if the element is present, FALSE if not.
 */
Boolean ts_treeset_contains(ConcurrentTreeSet *tree, void *item);

/**
 * Fetches the first (lowest) element currently in the treeset, then stores the
 * result into '*first'.
 *
 * Params:
 *    tree - The treeset to operate on.
 *    first - The pointer address to store the first element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - TreeSet is currently empty.
 */
Status ts_treeset_first(ConcurrentTreeSet *tree, void **first);

/**
 * Fetches the last (highest) element currently in the treeset, then stores the
 * result into '*last'.
 *
 * Params:
 *    tree - The treeset to operate on.
 *    last - The pointer address to store the last element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - TreeSet is currently empty.
 */
Status ts_treeset_last(ConcurrentTreeSet *tree, void **last);

/**
 * Fetches the greatest element in the treeset less than or equal to the given
 * element, then stores the result into '*floor'.
 *
 * Params:
 *    tree - The treeset to operate on.
 *    item - The element to match.
 *    floor - The pointer address to store the floor element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - TreeSet is currently empty.
 *    NOT_FOUND - No floor value exists.
 */
Status ts_treeset_floor(ConcurrentTreeSet *tree, void *item, void **floor);

/**
 * Fetches the least element in the treeset greater than or equal to the given
 * element, then stores the result into '*ceiling'.
 *
 * Params:
 *    tree - The treeset to operate on.
 *    item - The element to match.
 *    ceiling - The pointer address to store the ceiling element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - TreeSet is currently empty.
 *    NOT_FOUND - No ceiling value exists.
 */
Status ts_treeset_ceiling(ConcurrentTreeSet *tree, void *item, void **ceiling);

/**
 * Fetches the greatest element in the treeset strictly less than the given
 * element, then stores the result into '*lower'.
 *
 * Params:
 *    tree - The treeset to operate on.
 *    item - The element to match.
 *    lower - The pointer address to store the lower element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - TreeSet is currently empty.
 *    NOT_FOUND - No lower value exists.
 */
Status ts_treeset_lower(ConcurrentTreeSet *tree, void *item, void **lower);

/**
 * Fetches the least element in the treeset strictly greater than the given
 * element, then stores the result into '*higher'.
 *
 * Params:
 *    tree - The treeset to operate on.
 *    item - The element to match.
 *    higher - The pointer address to store the higher element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - TreeSet is currently empty.
 *    NOT_FOUND - No higher value exists.
 */
Status ts_treeset_higher(ConcurrentTreeSet *tree, void *item, void **higher);

/**
 * Retrieves and removes the first (lowest) element from the treeset, then
 * stores the result into '*first'.
 *
 * Params:
 *    tree - The treeset to operate on.
 *    first - The pointer address to store the removed first element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - TreeSet is currently empty.
 */
Status ts_treeset_pollFirst(ConcurrentTreeSet *tree, void **first);

/**
 * Retrieves and removes the last (highest) element from the treeset, then
 * stores the result into '*last'.
 *
 * Params:
 *    tree - The treeset to operate on.
 *    last - The pointer address to store the removed last element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - TreeSet is currently empty.
 */
Status ts_treeset_pollLast(ConcurrentTreeSet *tree, void **last);

/**
 * Removes the specified element from this set if it is present. If 'destructor'
 * is not NULL, it will be invoked on the element after removal.
 *
 * Params:
 *    tree - The treeset to operate on.
 *    item - The element to be removed.
 *    destructor - Function to operate on element after removal.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - TreeSet is currently empty.
 *    NOT_FOUND - Entry was not found.
 */
Status ts_treeset_remove(ConcurrentTreeSet *tree, void *item, void (*destructor)(void *));

/**
 * Removes all elements from the treeset. If 'destructor' is not NULL, it will be
 * invoked on each element in the treeset after being removed.
 *
 * Params:
 *    tree - The treeset to operate on.
 *    destructor - Function to operate on each element after removal.
 * Returns:
 *    None
 */
void ts_treeset_clear(ConcurrentTreeSet *tree, void (*destructor)(void *));

/**
 * Returns the number of elements in the treeset.
 *
 * Params:
 *    tree - The treeset to operate on.
 * Returns:
 *    The treeset's current size.
 */
long ts_treeset_size(ConcurrentTreeSet *tree);

/**
 * Returns TRUE if the treeset contains no elements, FALSE if otherwise.
 *
 * Params:
 *    tree - The treeset to operate on.
 * Returns:
 *    TRUE if the treeset is empty, FALSE if not.
 */
Boolean ts_treeset_isEmpty(ConcurrentTreeSet *tree);

/**
 * Allocates and generates an array containing all of the treeset's elements in
 * proper sequence (defined by the comparator, from least to greatest), then stores
 * the array into '*array'. Caller is responsible for freeing the array when finished.
 *
 * Params:
 *    tree - The treeset to operate on.
 *    array - Address where the new array will be stored.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - TreeSet is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_treeset_toArray(ConcurrentTreeSet *tree, Array **array);

/**
 * Creates an Iterator instance to iterate over the treeset's elements in proper
 * sequence (defined by the comparator, from least to greatest), then stores the iterator
 * into '*iter'. Caller is responsible for destroying the iterator instance when finished.
 *
 * Params:
 *    tree - The treeset to operate on.
 *    iter - Address where the new iterator will be stored.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - TreeSet is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_treeset_iterator(ConcurrentTreeSet *tree, ConcurrentIterator **iter);

/**
 * Destroys the treeset instance by freeing all of its reserved memory. If 'destructor'
 * is not NULL, it will be invoked on each element before the treeset is destroyed.
 *
 * Params:
 *    tree - The treeset to destroy.
 *    destructor - Function to operate on each element prior to treeset destruction.
 * Returns:
 *    None
 */
void ts_treeset_destroy(ConcurrentTreeSet *tree, void (*destructor)(void *));

#endif  /* _CDS_TS_TREESET_H__ */
