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

#ifndef _CDS_TREEMAP_H__
#define _CDS_TREEMAP_H__

#include "common.h"
#include "iterator.h"

/**
 * Interface for the TmEntry ADT.
 *
 * This ADT stores the key-value pairings in the TreeMap ADT and are provided back
 * to the caller from invocations to toArray() and iterator().
 */
typedef struct tm_entry TmEntry;

/**
 * Declaration for the TreeMap ADT.
 *
 * A tree-like set storing elements based on key-value pairings. The treemap is sorted
 * by the stored keys based on their natural ordering defined through a comparator
 * provided at construction time. Provides self-balancing capabilities for even
 * distribution.
 *
 * Modeled after the Java 7 TreeMap interface.
 */
typedef struct treemap TreeMap;

/**
 * Constructs a new treemap, then stores the new instance into '*tree'.
 *
 * The comparator function specified should return an integer comparing the two
 * specified keys, such that cmp(a, b) returns 0 when a == b, <0 when a < b, and
 * >0 when a > b.
 *
 * The key destructor function should be a function that performs any de-allocation
 * needed on the treemap's keys (if applicable). This function will be invoked on
 * the entry's key prior to being removed, the treemap being cleared, or the treemap
 * being destructed. If no de-allocation/destructor for the custom keys is required,
 * you may pass NULL as this parameter.
 *
 * Params:
 *    tree - The pointer address to store the new TreeMap instance.
 *    comparator - Function for comparing two keys in the treemap.
 *    keyDestructor - Function for de-allocating the treemap's keys.
 * Returns:
 *    STAT_SUCCESS - TreeMap was successfully created.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status treemap_new(TreeMap **tree, int (*keyComparator)(void *, void *),
        void (*keyDestructor)(void *));

/**
 * Associates the specified value with the specified key in the treemap. If the
 * treemap previously contained a mapping for the key, the old value is replaced, and
 * stored into '*previous'.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    key - The key with which the specified value is to be associated.
 *    value - The value to be associated with the specified key.
 *    previous - The pointer address to store the previous value into.
 * Returns:
 *    STAT_ENTRY_INSERTED - Key and value was inserted.
 *    STAT_ENTRY_REPLACED - Value was updated in the treemap, and the old value was
 *                          stored into '*previous' due to the key already existing.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status treemap_put(TreeMap *tree, void *key, void *value, void **previous);

/**
 * Fetches the first (least) key currently in the treemap, then stores the result
 * into '*firstKey'.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    firstKey - The pointer address to store the first key into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - TreeMap is currently empty.
 */
Status treemap_firstKey(TreeMap *tree, void **firstKey);

/**
 * Fetches the key-value mapping associated with the least key in this map, then
 * stores the result into '*first'.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    first - The pointer address to store the first entry into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - TreeMap is currently empty.
 */
Status treemap_first(TreeMap *tree, TmEntry **first);

/**
 * Fetches the last (greatest) key currently in the treemap, then stores the result
 * into '*lastKey'.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    lastKey - The pointer address to store the last key into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - TreeMap is currently empty.
 */
Status treemap_lastKey(TreeMap *tree, void **lastKey);

/**
 * Fetches the key-value mapping associated with the greatest key in this map, then
 * stores the result into '*last'.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    last - The pointer address to store the last entry into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - TreeMap is currently empty.
 */
Status treemap_last(TreeMap *tree, TmEntry **last);

/**
 * Fetches the greatest key in the treemap less than or equal to the given key, then
 * stores the result into '*floorKey'.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    key - The key to match.
 *    floorKey - The pointer address to store the floor key into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - TreeMap is currently empty.
 *    STAT_NOT_FOUND - No floor key exists.
 */
Status treemap_floorKey(TreeMap *tree, void *key, void **floorKey);

/**
 * Fetches the greatest entry in the treemap less than or equal to the given
 * key, then stores the result into '*floor'.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    key - The key to match.
 *    floor - The pointer address to store the floor entry into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - TreeMap is currently empty.
 *    STAT_NOT_FOUND - No floor entry exists.
 */
Status treemap_floor(TreeMap *tree, void *key, TmEntry **floor);

/**
 * Fetches the least key in the treemap greater than or equal to the given key, then
 * stores the result into '*ceilingKey'.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    key - The key to match.
 *    ceilingKey - The pointer address to store the ceiling key into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - TreeMap is currently empty.
 *    STAT_NOT_FOUND - No ceiling key exists.
 */
Status treemap_ceilingKey(TreeMap *tree, void *key, void **ceilingKey);

/**
 * Fetches the least entry in the treemap greater than or equal to the given key,
 * then stores the result into '*ceiling'.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    key - The key to match.
 *    ceiling - The pointer address to store the ceiling entry into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - TreeMap is currently empty.
 *    STAT_NOT_FOUND - No ceiling entry exists.
 */
Status treemap_ceiling(TreeMap *tree, void *key, TmEntry **ceiling);

/**
 * Fetches the greatest key in the treemap strictly less than the given key, then
 * stores the result into '*lowerKey'.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    key - The key to match.
 *    lowerKey - The pointer address to store the lower key into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - TreeMap is currently empty.
 *    STAT_NOT_FOUND - No lower key exists.
 */
Status treemap_lowerKey(TreeMap *tree, void *key, void **lowerKey);

/**
 * Fetches the greatest entry in the treemap strictly less than the given key, then
 * stores the result into '*lower'.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    key - The key to match.
 *    lowerKey - The pointer address to store the lower entry into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - TreeMap is currently empty.
 *    STAT_NOT_FOUND - No lower entry exists.
 */
Status treemap_lower(TreeMap *tree, void *key, TmEntry **lower);

/**
 * Fetches the least key in the treemap strictly greater than the given key, then
 * stores the result into '*higherKey'.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    key - The key to match.
 *    higherKey - The pointer address to store the higher key into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - TreeMap is currently empty.
 *    STAT_NOT_FOUND - No higher key exists.
 */
Status treemap_higherKey(TreeMap *tree, void *key, void **higherKey);

/**
 * Fetches the least entry in the treemap strictly greater than the given key, then
 * stores the result into '*higher'.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    key - The key to match.
 *    higher - The pointer address to store the higher entry into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - TreeMap is currently empty.
 *    STAT_NOT_FOUND - No higher entry exists.
 */
Status treemap_higher(TreeMap *tree, void *key, TmEntry **higher);

/**
 * Returns TRUE if the treemap contains a mapping for the specified key, FALSE if
 * otherwise.
 *
 * Params:
 *    map - The treemap to operate on.
 *    key - The key to search for.
 * Returns:
 *    TRUE if a mapping exists with the key, FALSE if not.
 */
Boolean treemap_containsKey(TreeMap *tree, void *key);

/**
 * Fetches the value to which the specified key is mapped, and stores the result
 * into '*value'.
 *
 * Params:
 *    map - The treemap to operate on.
 *    key - The key whose associated value is to be returned.
 *    value - The pointer address to store the fetched value into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - TreeMap is currently empty.
 *    STAT_NOT_FOUND - Entry with the specified key was not found.
 */
Status treemap_get(TreeMap *tree, void *key, void **value);

/**
 * Retrieves and removes the first (least) entry from the treemap, then stores the
 * removed key and value into '*firstKey' and 'firstValue', respectively.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    firstKey - The pointer address to store the removed first key into.
 *    firstValue - The pointer address to store the removed first value into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - TreeMap is currently empty.
 */
Status treemap_pollFirst(TreeMap *tree, void **firstKey, void **firstValue);

/**
 * Retrieves and removes the last (greatest) entry from the treemap, then stores the
 * removed key and value into '*lastKey' and '*lastValue', respectively.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    lastKey - The pointer address to store the removed last key into.
 *    lastValue - The pointer address to store the removed last value into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - TreeMap is currently empty.
 */
Status treemap_pollLast(TreeMap *tree, void **lastKey, void **lastValue);

/**
 * Removes the mapping for this key from this treemap if present.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    key - The key whose mapping is to be removed from the map.
 *    value - The pointer address to store the removed value into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - TreeMap is currently empty.
 *    STAT_NOT_FOUND - Entry with the specified key was not found.
 */
Status treemap_remove(TreeMap *tree, void *key, void **value);

/**
 * Removes all elements from the treemap. If 'valueDestructor' is not NULL, it
 * will be invoked on each element in the treemap after being removed.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    valueDestructor - Function to operate on each entry's value after removal.
 * Returns:
 *    None
 */
void treemap_clear(TreeMap *tree, void (*valueDestructor)(void *));

/**
 * Returns the number of elements in the treemap.
 *
 * Params:
 *    tree - The treemap to operate on.
 * Returns:
 *    The treemap's current size.
 */
long treemap_size(TreeMap *tree);

/**
 * Returns TRUE if the treemap contains no elements, FALSE if otherwise.
 *
 * Params:
 *    tree - The treemap to operate on.
 * Returns:
 *    TRUE if the treemap is empty, FALSE if not.
 */
Boolean treemap_isEmpty(TreeMap *tree);

/**
 * Allocates and generates an array containing all of the treemap's keys in in proper
 * sequence (defined by the key comparator, from least to greatest), then stores the
 * array into '*keys'. Caller is responsible for freeing the array when finished.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    keys - Array where the keys will be stored.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - TreeMap is currently empty.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status treemap_keyArray(TreeMap *tree, Array **keys);

/**
 * Allocates and generates an array containing all of the treemap's entries in proper
 * sequence (defined by the key comparator, from least to greatest), then stores the
 * array into '*entries'. Note that the items provided in the array are 'TmEntry*' values.
 * Caller is responsible for freeing the array when finished.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    entries - Address where the entries will be stored.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - TreeMap is currently empty.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status treemap_entryArray(TreeMap *tree, Array **entries);

/**
 * Creates an Iterator instance to iterate over the treemap's elements in proper
 * sequence (defined by the key's comparator, from least to greatest), then stores the
 * iterator into '*iter'. Note that the items being iterated over are 'TmEntry*' values.
 * Caller is responsible for destroying the iterator instance when finished.
 *
 * Params:
 *    tree - The treemap to operate on.
 *    iter - Address where the new iterator will be stored.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - TreeMap is currently empty.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status treemap_iterator(TreeMap *tree, Iterator **iter);

/**
 * Destroys the treemap instance by freeing all of its reserved memory. If
 * 'valueDestructor' is not NULL, it will be invoked on each entry's value before the
 * treemap is destroyed.
 *
 * Params:
 *    tree - The treemap to destroy.
 *    valueDestructor - Function to operate on each entry's value prior to treemap destruction.
 * Returns:
 *    None
 */
void treemap_destroy(TreeMap *tree, void (*valueDestructor)(void *));

/**
 * Returns the treemap entry's stored key.
 *
 * Params:
 *    entry - The TmEntry ADT to operate on.
 * Returns:
 *    The entry's key.
 */
void *tmentry_getKey(TmEntry *entry);

/**
 * Returns the treemap entry's stored value.
 *
 * Params:
 *    entry - The TmEntry ADT to operate on.
 * Returns:
 *    The entry's value.
 */
void *tmentry_getValue(TmEntry *entry);

#endif  /* _CDS_TREEMAP_H__ */
