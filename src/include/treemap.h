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

#ifndef _TREEMAP_H__
#define _TREEMAP_H__

#include "common.h"
#include "iterator.h"

typedef struct tm_entry TmEntry;

typedef struct treemap TreeMap;

Status treemap_new(TreeMap **tree, int (*comparator)(void *, void *), void (*keyDestructor)(void *));

Status treemap_put(TreeMap **tree, void *key, void *entry, void **previous);

Status treemap_firstKey(TreeMap *tree, void **firstKey);

Status treemap_first(TreeMap *tree, TmEntry **first);

Status treemap_lastKey(TreeMap *tree, void **lastKey);

Status treemap_last(TreeMap *tree, TmEntry **last);

Status treemap_floorKey(TreeMap *tree, void *key, void **floorKey);

Status treemap_floor(TreeMap *tree, void *key, TmEntry **floor);

Status treemap_ceilingKey(TreeMap *tree, void *key, void **ceilingKey);

Status treemap_ceiling(TreeMap *tree, void *key, TmEntry **ceiling);

Status treemap_lowerKey(TreeMap *tree, void *key, void **lowerKey);

Status treemap_lower(TreeMap *tree, void *key, TmEntry **lower);

Status treemap_higherKey(TreeMap *tree, void *key, void **higherKey);

Status treemap_higher(TreeMap *tree, void *key, TmEntry **higher);

Boolean treemap_containsKey(TreeMap *tree, void *key);

Status treemap_get(TreeMap *tree, void *key, void **value);

Status treemap_pollFirst(TreeMap *tree, void **firstKey, void **firstValue);

Status treemap_pollLast(TreeMap *tree, void **lastKey, void **lastValue);

Status treemap_remove(TreeMap *tree, void *key, void **value);

void treemap_clear(TreeMap *tree, void (*valueDestructor)(void *));

long treemap_size(TreeMap *tree);

Boolean treemap_isEmpty(TreeMap *tree);

Status treemap_keyArray(TreeMap *tree, void ***keys, long *len);

Status treemap_entryArray(TreeMap *tree, TmEntry ***entries, long *len);

Status treemap_iterator(TreeMap *tree, Iterator **iter);

void treemap_destroy(TreeMap *tree, void (*valueDestructor)(void *));

void *tmentry_getKey(TmEntry *entry);

void *tmentry_getValue(TmEntry *entry);

#endif  /* _TREEMAP_H__ */
