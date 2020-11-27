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

#ifndef _CDS_HASHSET_H__
#define _CDS_HASHSET_H__

#include "cds_common.h"
#include "iterator.h"

/**
 * Declaration for the HashSet ADT.
 *
 * Set based implementation; holds a collection of elements with no duplicates.
 *
 * Modeled after the Java 7 HashSet interface.
 */
typedef struct hashset HashSet;

/**
 * Constructs a new hashset instance with the specified starting capacity and load
 * factor, then stores the new instance into '*set'. If the capacity specified is
 * <= 0, a default capacity is assigned. If the load factor specified is <= 0.0,
 * a default load factor is assigned.
 *
 * The hash function specified should return an index number such that hash(obj, N)
 * will return the hashed value of 'obj' in an array of size N.
 *
 * The comparator function specified should return an integer comparing the two
 * specified values, such that cmp(a, b) returns 0 when a == b, <0 when a < b, and
 * >0 when a > b.
 *
 * Params:
 *    set - The pointer address to store the new HashSet instance.
 *    hash - Function to hash the values inside the hashset.
 *    comparator - Function for comparing two items in the hashset.
 *    capacity - The hashset's starting capacity.
 *    loadFactor - The hashset's assigned load factor.
 * Returns:
 *    OK - HashSet was successfully created.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status hashset_new(HashSet **set, long (*hash)(void *, long),
        int (*comparator)(void *, void *),
        long capacity, double loadFactor);

/**
 * Adds the specified element to the hashset if it is not already present.
 *
 * Params:
 *    set - The hashset to operate on.
 *    item - The element to add.
 * Returns:
 *    OK - Operation was successful.
 *    ALREADY_EXISTS - Specified entry is already present.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status hashset_add(HashSet *set, void *item);

/**
 * Returns TRUE if the hashset contains the specified element, FALSE if not.
 *
 * Params:
 *    set - The hashset to operate on.
 *    item - The item to search for.
 * Returns:
 *    TRUE if the element is present, FALSE if not.
 */
Boolean hashset_contains(HashSet *set, void *item);

/**
 * Removes the specified element from the hashset if it is present. If 'destructor'
 * is not NULL, it will be invoked on the element after removal.
 *
 * Params:
 *    set - The hashset to operate on.
 *    item - The element to remove.
 *    destructor - Function to operate on element after removal.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - HashSet is currently empty.
 *    NOT_FOUND - Entry with the specified key was not found.
 */
Status hashset_remove(HashSet *set, void *item, void (*destructor)(void *));

/**
 * Removes all elements from the hashset. If 'destructor' is not NULL, it will be
 * invoked on each element in the hashset after being removed.
 *
 * Params:
 *    set - The hashset to operate on.
 *    destructor - Function to operate on each element after removal.
 * Returns:
 *    None
 */
void hashset_clear(HashSet *set, void (*destructor)(void *));

/**
 * Returns the number of elements in the hashset.
 *
 * Params:
 *    set - The hashset to operate on.
 * Returns:
 *    The hashset's current size.
 */
long hashset_size(HashSet *set);

/**
 * Returns TRUE if the hashset contains no elements, FALSE if otherwise.
 *
 * Params:
 *    set - The hashset to operate on.
 * Returns:
 *    TRUE if the hashset is empty, FALSE if not.
 */
Boolean hashset_isEmpty(HashSet *set);

/**
 * Allocates and generates an array containing all of the hashset's elements in no
 * particular order, then stores the array into '*array'. Caller is responsible for
 * freeing the array when finished.
 *
 * Params:
 *    set - The hashset to operate on.
 *    array - Address where the new array will be stored.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - HashSet is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status hashset_toArray(HashSet *set, Array **array);

/**
 * Returns an Iterator instance to iterate over the the hashset's elements in no
 * particular order. Caller is responsible for destroying the iterator instance when
 * finished.
 *
 * Params:
 *    set - The hashset to operate on.
 *    iter - Address where the new iterator will be stored.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - HashSet is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status hashset_iterator(HashSet *set, Iterator **iter);

/**
 * Destroys the hashset instance by freeing all of its reserved memory. If 'destructor'
 * is not NULL, it will be invoked on each element before the hashset is destroyed.
 *
 * Params:
 *    set - The hashset to destroy.
 *    destructor - Function to operate on each element prior to hashset destruction.
 * Returns:
 *    None
 */
void hashset_destroy(HashSet *set, void (*destructor)(void *));

#endif  /* _CDS_HASHSET_H__ */
