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

#ifndef _ARRAYLIST_H__
#define _ARRAYLIST_H__

#include "common.h"
#include "iterator.h"

/**
 * Interface for the ArrayList ADT.
 *
 * The ArrayList class is a collection of elements stored in a resizable array.
 *
 * Modeled after the Java 7 ArrayList interface.
 */
typedef struct arraylist ArrayList;

/**
 * Constructs a new array list instance with the specified starting capacity, then
 * stores the new instance into '*list'. If the capacity given is <= 0, a default
 * capacity is assigned.
 *
 * Params:
 *    list - The pointer address to store the new ArrayList instance.
 *    capacity - The default capacity of the arraylist.
 * Returns:
 *    STAT_SUCCESS - ArrayList was successfully created.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status arraylist_new(ArrayList **list, long capacity);

/**
 * Appends the specified element to the end of the array list.
 *
 * Params:
 *    list - The array list to operate on.
 *    item - The element to be appended to the array list.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status arraylist_add(ArrayList *list, void *item);

/**
 * Inserts the specified element at the specified position in the array list. Shifts
 * the element currently at that position (if any) and any subsequent elements to the
 * right (adds one to their indices).
 *
 * Params:
 *    list - The array list to operate on.
 *    i - The index at which the specified element is to be inserted.
 *    item - The element to be inserted.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_INVALID_INDEX - Index given is out of range (index < 0 || index > size()).
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status arraylist_insert(ArrayList *list, long i, void *item);

/**
 * Returns the element at the specified position in the array list.
 *
 * Params:
 *    list - The array list to operate on.
 *    i - The index of the element to retrieve.
 *    item - The pointer address to store the retrieved element into.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Array list is currently empty.
 *    STAT_INVALID_INDEX - Index given is out of range (index < 0 || index >= size()).
 */
Status arraylist_get(ArrayList *list, long i, void **item);

/**
 * Replaces the element at the specified position in the array list with the specified
 * element.
 *
 * Params:
 *    list - The array list to operate on.
 *    i - The index of the element to replace.
 *    item - The element to be stored at the specified position.
 *    previous - The pointer address to store the element previously at the specified
 *               position.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Array list is currently empty.
 *    STAT_INVALID_INDEX - Index given is out of range (index < 0 || index >= size()).
 */
Status arraylist_set(ArrayList *list, long i, void *item, void **previous);

/**
 * Removes the element at the specified position in the array list. Shifts any subsequent
 * elements to the left (subtracts one from their indices).
 *
 * Params:
 *    list - The array list to operate on.
 *    i - The index of the element to be removed.
 *    item - The pointer address to store the element that was removed from the list.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Array list is currently empty.
 *    STAT_INVALID_INDEX - Index given is out of range (index < 0 || index >= size()).
 */
Status arraylist_remove(ArrayList *list, long i, void **item);

/**
 * Increases the capacity of the array list, if necessary, to ensure that it can hold at
 * least the number of elements specified by the minimum capacity argument.
 *
 * Params:
 *    list - The array list to operate on.
 *    capacity - The desired minimum capacity.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status arraylist_ensureCapacity(ArrayList *list, long capacity);

/**
 * Trims the capacity of the array list instance to be the list's current size.
 *
 * Params:
 *    list - The array list to operate on.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status arraylist_trimToSize(ArrayList *list);

/**
 * Removes all elements from the array list. If 'destructor' is not NULL, it will be
 * invoked on each element in the array list after being removed.
 *
 * Params:
 *    list - The array list to operate on.
 *    destructor - Function to operate on each element after removal.
 * Returns:
 *    None
 */
void arraylist_clear(ArrayList *list, void (*destructor)(void *));

/**
 * Returns the number of elements in the array list.
 *
 * Params:
 *    list - The array list to operate on.
 * Returns:
 *    The array list's current size.
 */
long arraylist_size(ArrayList *list);

/**
 * Returns the array list's current capacity, that is, the maximum number of elements
 * it can hold before resizing is required.
 *
 * Params:
 *    list - The array list to operate on.
 * Returns:
 *    The array list's current capacity.
 */
long arraylist_capacity(ArrayList *list);

/**
 * Returns TRUE if the array list contains no elements, FALSE if otherwise.
 *
 * Params:
 *    list - The array list to operate on.
 * Returns:
 *    TRUE if the array list is empty, FALSE if not.
 */
Boolean arraylist_isEmpty(ArrayList *list);

/**
 * Allocates and generates an array containing all of the array list's elements in proper
 * sequence (from first to last element), then stores the array into '*array'. Caller is
 * responsible for freeing the array when finished.
 *
 * Params:
 *    list - The array list to operate on.
 *    array - Address where the generated array will be stored.
 *    len - Address where the length of the generated array will be stored.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Array list is currently empty.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status arraylist_toArray(ArrayList *list, void ***array, long *len);

/**
 * Creates an Iterator instance to iterate over the array list's elements in proper sequence
 * (from first to last element), then stores the iterator into '*iter'. Caller is responsible
 * for destroying the iterator instance when finished.
 *
 * Params:
 *    list - The array list to operate on.
 *    iter - Address where the new iterator will be stored.
 * Returns:
 *    STAT_SUCCESS - Operation was successful.
 *    STAT_STRUCT_EMPTY - Array list is currently empty.
 *    STAT_ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status arraylist_iterator(ArrayList *list, Iterator **iter);

/**
 * Destroys the array list instance by freeing all of its reserved memory. If 'destructor'
 * is not NULL, it will be invoked on each element before the arraylist is destroyed.
 *
 * Params:
 *    list - The array list to destroy.
 *    destructor - Function to operate on each element prior to array list destruction.
 * Returns:
 *    None
 */
void arraylist_destroy(ArrayList *list, void (*destructor)(void *));

#endif  /* _ARRAYLIST_H__ */
