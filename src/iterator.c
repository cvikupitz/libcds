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

#include <stdlib.h>
#include "iterator.h"

/*
 * The iterator ADT struct.
 */
struct iterator {
    void **items;       /* The collection of elements to iterate */
    long next;          /* The next item in iteration */
    long len;           /* Size of the item array */
};

Status iterator_new(Iterator **iter, void **items, long len) {

    Iterator *temp = (Iterator *)malloc(sizeof(Iterator));

    /* Check for allocation failure */
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Initialize the rest of iterator members */
    temp->items = items;
    temp->next = 0L;
    temp->len = len;
    *iter = temp;

    return STAT_SUCCESS;
}

Boolean iterator_hasNext(Iterator *iter) {
    return ( iter->next < iter->len ) ? TRUE : FALSE;
}

Status iterator_next(Iterator *iter, void **next) {

    /* Iteration already complete, return with status */
    if (iter->next == iter->len)
        return STAT_ITERATION_END;
    /* Advances to next item in iteration */
    *next = iter->items[iter->next++];

    return STAT_SUCCESS;
}

void iterator_destroy(Iterator *iter) {
    free(iter->items);
    free(iter);
}
