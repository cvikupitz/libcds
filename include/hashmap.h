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

#ifndef _CDS_HASHMAP_H__
#define _CDS_HASHMAP_H__

#include "cds_common.h"
#include "iterator.h"

/**
 * Interface for the HmEntry ADT.
 *
 * This ADT stores the key-value pairings in the HashMap ADT and are provided back
 * to the caller from invocations to toArray() and iterator().
 */
typedef struct hm_entry HmEntry;

/**
 * Declaration for the HashMap ADT.
 *
 * Hashmap based implementation; holds elements based on key-value pairings.
 *
 * Modeled after the Java 7 HashMap interface.
 */
typedef struct hashmap HashMap;

/**
 * Constructs a new hashmap instance with the specified starting capacity and load
 * factor, then stores the new instance into '*map'. If the capacity specified is
 * <= 0, a default capacity is assigned. If the load factor specified is <= 0.0, a
 * default load factor is assigned.
 *
 * The hash function specified will be used by the hashmap to compute the bucket
 * placement for each entry in the hashmap, such that hash(a,N) will return a value
 * in the range [0, N-1]. For example, if using char * keys, you might define a hash
 * function like this:
 *
 *    #define PRIME 7L
 *    long hash(void *key, long N) {
 *        long val = 0L;
 *        char *ch;
 *        for (ch = (char *)key; *ch != '\0'; ch++)
 *        val = (*ch + (val * PRIME)) % N;
 *        return val;
 *    }
 *
 * The comparator function specified should return an integer comparing the two
 * specified keys, such that cmp(a, b) returns 0 when a == b, or !0 when a != b.
 *
 * The key destructor function should be a function that performs any de-allocation
 * needed on the hashmap's keys (if applicable). This function will be invoked on
 * the entry's key prior to being removed, the hashmap being cleared, or the hashmap
 * being destructed. If no de-allocation/destructor for the custom keys is required,
 * you may pass NULL as this parameter.
 *
 * Params:
 *    map - The pointer address to store the new HashMap instance.
 *    hash - The hashing function the map will use to compute the bucket placement.
 *    comparator - Function for comparing two keys in the hashmap.
 *    capacity - The hashmap's starting capacity.
 *    loadFactor - The hashmap's assigned load factor.
 *    keyDestructor - Function for de-allocating the hashmap's keys.
 * Returns:
 *    OK - HashMap was successfully created.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status hashmap_new(HashMap **map, long (*hash)(void *, long), int (*keyComparator)(void *, void *),
        long capacity, double loadFactor, void (*keyDestructor)(void *));

/**
 * Associates the specified value with the specified key in the hashmap. If the
 * hashmap previously contained a mapping for the key, the old value is replaced,
 * and stored into '*previous'.
 *
 * Params:
 *    map - The hashmap to operate on.
 *    key - The key with which the specified value is to be associated.
 *    value - The value to be associated with the specified key.
 *    previous - The pointer address to store the previous value into.
 * Returns:
 *    INSERTED - Key and entry was inserted.
 *    REPLACED - Entry was updated in the hashmap, and the old entry was
 *               stored into '*previous' due to the key already existing.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status hashmap_put(HashMap *map, void *key, void *value, void **previous);

/**
 * Returns TRUE if the hashmap contains a mapping for the specified key, false if
 * otherwise.
 *
 * Params:
 *    map - The hashmap to operate on.
 *    key - The key to search for.
 * Returns:
 *    TRUE if a mapping exists with the key, FALSE if not.
 */
Boolean hashmap_containsKey(HashMap *map, void *key);

/**
 * Fetches the value to which the specified key is mapped, and stores the result
 * into '*value'.
 *
 * Params:
 *    map - The hashmap to operate on.
 *    key - The key whose associated value is to be returned.
 *    value - The pointer address to store the fetched value into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - HashMap is currently empty.
 *    NOT_FOUND - Entry with the specified key was not found.
 */
Status hashmap_get(HashMap *map, void *key, void **value);

/**
 * Removes the mapping for the specified key from the hashmap if present.
 *
 * Params:
 *    map - The hashmap to operate on.
 *    key - The key whose mapping is to be removed from the map.
 *    value - The pointer address to store the removed value into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - HashMap is currently empty.
 *    NOT_FOUND - Entry with the specified key was not found.
 */
Status hashmap_remove(HashMap *map, void *key, void **value);

/**
 * Removes all elements from the hashmap. If 'destructor' is not NULL, it will be
 * invoked on each element in the hashmap after being removed.
 *
 * Params:
 *    map - The hashmap to operate on.
 *    destructor - Function to operate on each entry value after removal.
 * Returns:
 *    None
 */
void hashmap_clear(HashMap *map, void (*valueDestructor)(void *));

/**
 * Returns the number of elements in the hashmap.
 *
 * Params:
 *    map - The hashmap to operate on.
 * Returns:
 *    The hashmap's current size.
 */
long hashmap_size(HashMap *map);

/**
 * Returns TRUE if the hashmap contains no elements, FALSE if otherwise.
 *
 * Params:
 *    map - The hashmap to operate on.
 * Returns:
 *    TRUE if the hashmap is empty, FALSE if not.
 */
Boolean hashmap_isEmpty(HashMap *map);

/**
 * Allocates and generates an array containing all of the hashmap's keys in no
 * particular order, then stores the array into '*keys'. Caller is responsible for
 * freeing the array when finished.
 *
 * Params:
 *    map - The hashmap to operate on.
 *    keys - Array where the keys will be stored.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - HashMap is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status hashmap_keyArray(HashMap *map, Array **keys);

/**
 * Allocates and generates an array containing all of the hashmap's elements in no
 * particular order, then stores the array into '*entries'. Note that the items
 * provided in the array are 'HmEntry*' values. Caller is responsible for freeing
 * the array when finished.
 *
 * Params:
 *    map - The hashmap to operate on.
 *    entries - Array where the entries will be stored.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - HashMap is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status hashmap_entryArray(HashMap *map, Array **entries);

/**
 * Creates an Iterator instance to iterate over the the hashmap's elements in no
 * particular order, then stores the iterator into '*iter'. Note that the items
 * being iterated over are 'HmEntry*' values. Caller is responsible for destroying
 * the iterator instance when finished.
 *
 * Params:
 *    map - The hashmap to operate on.
 *    iter - Address where the new iterator will be stored.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - HashMap is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status hashmap_iterator(HashMap *map, Iterator **iter);

/**
 * Destroys the hashmap instance by freeing all of its reserved memory. If 'destructor'
 * is not NULL, it will be invoked on each element before the hashmap is destroyed.
 *
 * Params:
 *    map - The hashmap to destroy.
 *    valueDestructor - Function to operate on each entry value prior to hashmap destruction.
 * Returns:
 *    None
 */
void hashmap_destroy(HashMap *map, void (*valueDestructor)(void *));

/**
 * Returns the hashmap entry's stored key.
 *
 * Params:
 *    entry - The HmEntry to operate on.
 * Returns:
 *    The entry's key.
 */
void *hmentry_getKey(HmEntry *entry);

/**
 * Returns the hashmap entry's stored value.
 *
 * Params:
 *    entry - The HmEntry to operate on.
 * Returns:
 *    The entry's value.
 */
void *hmentry_getValue(HmEntry *entry);

/**
 * Sets the corresponding value of the entry with the new specified value.
 *
 * Params:
 *    entry - The HmEntry to operate on.
 *    value - The new value to store.
 * Returns:
 *    None
 */
void hmentry_setValue(HmEntry *entry, void *value);

#endif  /* _CDS_HASHMAP_H__ */
