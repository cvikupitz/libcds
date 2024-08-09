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

#include <stdio.h>      // Required for `sprintf()`
#include <stdlib.h>     // Required for memory allocation like `malloc()`, `free()`, etc
#include <limits.h>     // Required for upper limits on int/long types to avoid overflowing
#include "string_builder.h"

struct string_builder {
    char *str;              // The inner string builder
    long index;             // Index of the next character to add, same as current size
    long capacity;          // The builder's current capacity (including the null terminator)
    float growthFactor;     // The growth factor to apply when expanding builder capacity
};

#define UNUSED __attribute__((unused))

// The default capacity to assign when the capacity give is invalid
#define DEFAULT_CAPACITY 16L
// The maximum capacity allowed (to avoid builder overflow)
// Must allow one extra byte for null terminator
#define MAX_CAPACITY ( LONG_MAX - 1L )
// The default growth factor to apply when user input is invalid or 0
#define DEFAULT_GROWTH_FACTOR 0.12f

// Macro to detect an overflow if longs `a` and `b` are added together
#define ADD_OVERFLOWS(a, b) ( ( b > ( MAX_CAPACITY - a ) ) ? TRUE : FALSE )

/**
 * Local method to compute the length of the specified string `str`.
 */
static int _get_str_length(char *str) {

    char *ch;
    for (ch = str; *ch != '\0'; ch++); // Navigate to end of string
    return ch - str;
}

/**
 * Helper method to ensure the capacity of the string builder, given the capacity to assign. The
 * capacity may be greater than or less than the current capacity, and the builder will be
 * reallocated as needed. Returns TRUE if successful, FALSE if allocation fails.
 */
static Boolean _ensure_capacity(StringBuilder *builder, long newCapacity) {

    size_t bytes = ( ( newCapacity + 1 ) * sizeof(char) );
    char *temp;

    // Attempts to reallocate the builder, returning false if fails
    if ((temp = (char *)realloc(builder->str, bytes)) == NULL) {
        return FALSE;
    }

    // Update attributes after extension
    builder->str = temp;
    long i;
    for (i = builder->capacity; i < newCapacity + 1; i++) {
        // Need to add null terminators in all new bytes
        builder->str[i] = '\0';
    }
    builder->capacity = newCapacity;

    return TRUE;
}

/**
 * Helper method to calculate the amount to increase the builder capacity by prior to appending or
 * inserting a string of the specified length `strLen`. A return value of 0 means that the builder
 * does not need expanding, and a value of -1 means that the builder cannot accomodate the string
 * length, even with resizing (due to long overflowing).
 */
static long _compute_next_capacity_increase(StringBuilder *builder, int strLen) {

    long increment = 0L;
    long bufferRemaining = builder->capacity - builder->index;

    // There's not enough room in builder to store the string, will need to increase capacity
    if (strLen > bufferRemaining) {

        /**
         * Checks the total number of characters that are available without causing an overflow with
         * this builder. If the string still can't be allocated after resizing the max capacity
         * allowed, then we cannot append and must return an allocation error.
         */
        long totalRemaining = ( MAX_CAPACITY - builder->capacity + bufferRemaining );
        if (strLen > totalRemaining) {
            return -1L;
        }

        /**
         * Computes the `increment`, that is, the amount to grow the builder. We will add the
         * remaining characters needed for the string, plus some additional room to avoid
         * reallocating every call. We also must check for builder overflow again, but can just set
         * it to the max capacity if so.
         */
        increment = strLen - bufferRemaining;
        long extraPadding = ( (long)( builder->capacity * builder->growthFactor ) + 1 );
        if ( ADD_OVERFLOWS(builder->capacity, increment + extraPadding) == FALSE ) {
            increment += extraPadding;
        } else {
            increment = MAX_CAPACITY - builder->capacity;
        }
    }

    return increment;
}

/**
 * Local method to insert the specified string `str` into the builder, starting at index `index` in
 * the builder.
 */
static Status _insert_str(StringBuilder *builder, long offset, char *str) {

    // Asserts the index supplied is valid
    if (offset < 0 || offset > builder->index) {
        return INVALID_INDEX;
    }

    char *ch;
    int len = _get_str_length(str);
    long i, increment = _compute_next_capacity_increase(builder, len);

    if (increment < 0L) {
        // Cannot expand for string, return allocation error
        return ALLOC_FAILURE;
    } else if (increment > 0L) {
        // Attempt to expand the builder for new string
        if (_ensure_capacity(builder, builder->capacity + increment) == FALSE) {
            return ALLOC_FAILURE;
        }
    }

    // If offset < length, need to shift characters over `len` spaces to right to fit in string
    if (offset < builder->index) {
        for (i = builder->index - 1; i >= offset; i--) {
            builder->str[i + len] = builder->str[i];
        }
    }

    // Insert characters into builder starting at index `offset`
    for (i = offset, ch = str; *ch != '\0'; ch++, i++) {
        builder->str[i] = *ch;
    }
    builder->index += len;

    return OK;
}

Status string_builder_new(StringBuilder **builder, long capacity, float growthFactor, char *str) {

    // Allocate the struct, check for allocation failures
    StringBuilder *temp = (StringBuilder *)malloc(sizeof(StringBuilder));
    if (temp == NULL) {
        return ALLOC_FAILURE;
    }

    // Set up capacity, initialize the reminaing struct members
    long cap = ( capacity <= 0 ) ? DEFAULT_CAPACITY : capacity;
    // Ensure capacity does not exceed max allowed
    if (cap > MAX_CAPACITY) {
        cap = MAX_CAPACITY;
    }

    // Allocates the inner string builder, return error if fails
    size_t bytes = ( ( cap + 1 ) * sizeof(char) );
    char *innerBuffer = (char *)malloc(bytes);
    if (innerBuffer == NULL) {
        free(temp);
        return ALLOC_FAILURE;
    }

    // Fills up the empty builder with null terminators
    long i;
    for (i = 0L; i < cap + 1; i++) {
        innerBuffer[i] = '\0';
    }

    // Initialize remaining struct properties
    temp->str = innerBuffer;
    temp->capacity = cap;
    temp->growthFactor = ( 0.0f < growthFactor ) && ( growthFactor <= 1.0f ) ?
                         growthFactor : DEFAULT_GROWTH_FACTOR;

    // If an initial string is provided, append it to the builder
    if (str != NULL) {
        if (_insert_str(temp, 0L, str) != OK) {
            free(temp->str);
            free(temp);
            return ALLOC_FAILURE;
        }
    }
    *builder = temp;

    return OK;
}

Status string_builder_appendChar(StringBuilder *builder, char ch) {

    char str[] = { ch, '\0' };
    return _insert_str(builder, builder->index, str);
}

Status string_builder_appendStr(StringBuilder *builder, char *str) {

    char *temp = ( str != NULL ) ? str : "null";
    return _insert_str(builder, builder->index, temp);
}

Status string_builder_appendBoolean(StringBuilder *builder, Boolean b) {

    char *str = ( b == TRUE ) ? "true" : "false";
    return _insert_str(builder, builder->index, str);
}

// The size of the temporary builder where non-string conversion results are placed
#define BUFFER_SIZE 1024

Status string_builder_appendShort(StringBuilder *builder, short s) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%hd", s);
    return _insert_str(builder, builder->index, temp);
}

Status string_builder_appendInt(StringBuilder *builder, int i) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%d", i);
    return _insert_str(builder, builder->index, temp);
}

Status string_builder_appendLong(StringBuilder *builder, long l) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%ld", l);
    return _insert_str(builder, builder->index, temp);
}

Status string_builder_appendFloat(StringBuilder *builder, float f) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%f", f);
    return _insert_str(builder, builder->index, temp);
}

Status string_builder_appendDouble(StringBuilder *builder, double d) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%lf", d);
    return _insert_str(builder, builder->index, temp);
}

static Boolean _load_substring_to_buffer(char *src, char dest[], long dstBegin, long dstEnd) {

    char *s = ( src != NULL ) ? src : "null";
    int strLen = _get_str_length(s);
    if ( ( dstBegin < 0 )  || ( dstBegin > dstEnd ) || ( dstEnd > strLen ) ) {
        return FALSE;
    }

    long i, j, subLen = ( dstEnd - dstBegin );
    for (i = 0, j = dstBegin; i < subLen; i++, j++) {
        dest[i] = s[j];
    }
    dest[i] = '\0';

    return TRUE;
}

Status string_builder_appendSubStr(StringBuilder *builder, char *str, int start, int end) {

    char temp[(end - start) + 1];
    if ((_load_substring_to_buffer(str, temp, start, end)) == FALSE) {
        return INVALID_INDEX;
    }
    return _insert_str(builder, builder->index, temp);
}

Status string_builder_appendStrSubSequence(StringBuilder *builder, char *str, int offset, int len) {

    char temp[len + 1];
    if (_load_substring_to_buffer(str, temp, offset, offset + len) == FALSE) {
        return INVALID_INDEX;
    }
    return _insert_str(builder, builder->index, temp);
}

Status string_builder_appendStrBuilder(StringBuilder *builder, StringBuilder *other) {

    if (other == NULL) {
        return _insert_str(builder, builder->index, "null");
    } else if (other->index == 0) {
        return OK;
    } else if (builder == other) {
        char buffer[other->index];
        _load_substring_to_buffer(other->str, buffer, 0, other->index);
        return _insert_str(builder, builder->index, buffer);
    } else {
        return _insert_str(builder, builder->index, other->str);
    }
}

Status string_builder_insertChar(StringBuilder *builder, long offset, char ch) {

    char str[] = { ch, '\0' };
    return _insert_str(builder, offset, str);
}

Status string_builder_insertStr(StringBuilder *builder, long offset, char *str) {

    char *temp = ( str != NULL ) ? str : "null";
    return _insert_str(builder, offset, temp);
}

Status string_builder_insertBoolean(StringBuilder *builder, long offset, Boolean b) {

    char *str = ( b == TRUE ) ? "true" : "false";
    return _insert_str(builder, offset, str);
}

Status string_builder_insertShort(StringBuilder *builder, long offset, short s) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%hd", s);
    return _insert_str(builder, offset, temp);
}

Status string_builder_insertInt(StringBuilder *builder, long offset, int i) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%d", i);
    return _insert_str(builder, offset, temp);
}

Status string_builder_insertLong(StringBuilder *builder, long offset, long l) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%ld", l);
    return _insert_str(builder, offset, temp);
}

Status string_builder_insertFloat(StringBuilder *builder, long offset, float f) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%f", f);
    return _insert_str(builder, offset, temp);
}

Status string_builder_insertDouble(StringBuilder *builder, long offset, double d) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%lf", d);
    return _insert_str(builder, offset, temp);
}

Status string_builder_insertSubStr(StringBuilder *builder, long offset, char *str, int start,
                                   int end) {

    char temp[(end - start) + 1];
    if ((_load_substring_to_buffer(str, temp, start, end)) == FALSE) {
        return INVALID_INDEX;
    }
    return _insert_str(builder, offset, temp);
}

Status string_builder_insertStrSubSequence(StringBuilder *builder, long index, char *str,
                                           int offset, int len) {

    char temp[len + 1];
    if ((_load_substring_to_buffer(str, temp, offset, offset + len)) == FALSE) {
        return INVALID_INDEX;
    }
    return _insert_str(builder, index, temp);
}

Status string_builder_insertStrBuilder(StringBuilder *builder, long offset, StringBuilder *other) {

    if (other == NULL) {
        return _insert_str(builder, offset, "null");
    } else if (other->index == 0) {
        return OK;
    } else if (builder == other) {
        char buffer[other->index];
        _load_substring_to_buffer(other->str, buffer, 0, other->index);
        return _insert_str(builder, offset, buffer);
    } else {
        return _insert_str(builder, offset, other->str);
    }
}

Status string_builder_replace(UNUSED StringBuilder *builder, UNUSED int start, UNUSED int end, UNUSED char *str) { return OK; }

Status string_builder_charAt(StringBuilder *builder, long i, char *result) {

    if (i < 0L || i >= builder->index) {
        return INVALID_INDEX;
    }

    *result = builder->str[i];
    return OK;
}

static Status _get_substring(StringBuilder *builder, long start, long end, char *dst, int dstBegin,
                             Boolean allocate) {

    if (start < 0 || end < 0 || end > builder->index || start > end || dstBegin < 0) {
        return INVALID_INDEX;
    }

    int strLen = ( end - start ) + 1;
    if (allocate == TRUE) {
        int i;
        char *temp;
        if ((temp = (char *)malloc(dstBegin + strLen)) == NULL) {
            return ALLOC_FAILURE;
        }
        for (i = 0; i < strLen; i++) {
            temp[i] = '\0';
        }
        dst = temp;
    }

    long i;
    int j;
    for (i = start, j = dstBegin; i < end; i++, j++) {
        dst[j] = builder->str[i];
    }

    return OK;
}

Status string_builder_substring(StringBuilder *builder, long start, char **result) {

    return _get_substring(builder, start, builder->index, *result, 0, TRUE);
}

Status string_builder_subsequence(StringBuilder *builder, long start, long end, char **result) {

    return _get_substring(builder, start, end, *result, 0, TRUE);
}

Status string_builder_getChars(StringBuilder *builder, long srcBegin, long srcEnd, char dst[],
                               int dstBegin) {

    return _get_substring(builder, srcBegin, srcEnd, dst, dstBegin, FALSE);
}

Status string_builder_setCharAt(StringBuilder *builder, long index, char ch) {

    if (index < 0 || index >= builder->index) {
        return INVALID_INDEX;
    }

    builder->str[index] = ch;
    return OK;
}

Status string_builder_setLength(StringBuilder *builder, long len, char padding) {

    if (len < 0) {
        return INVALID_INDEX;
    } else if (len == builder->index) {
        return OK;
    }

    Boolean shrink = ( len < builder->index ) ? TRUE : FALSE;
    if (_ensure_capacity(builder, len) == FALSE) {
        return ALLOC_FAILURE;
    }

    if (shrink) {
        builder->str[len] = '\0';
    } else {
        long i;
        for (i = builder->index; i < len; i++) {
            builder->str[i] = padding;
        }
        builder->str[i] = '\0';
    }
    builder->index = len;

    return OK;
}

static long _search_first_occurrence(UNUSED StringBuilder *builder, UNUSED char *sub, UNUSED long start) {

    if (sub == NULL || start < 0 || start >= builder->index) {
        return -1L;
    }
    //TODO
    return 0L;
}

long string_builder_indexOf(StringBuilder *builder, char *str) {

    return _search_first_occurrence(builder, str, 0L);
}

long string_builder_indexOfFrom(StringBuilder *builder, char *str, long fromIndex) {

    return _search_first_occurrence(builder, str, fromIndex);
}

static long _search_last_occurrence(UNUSED StringBuilder *builder, UNUSED char *sub, UNUSED long start) {

    if (sub == NULL || start < 0 || start >= builder->index) {
        return -1L;
    }
    //TODO
    return 0L;
}

long string_builder_lastIndexOf(StringBuilder *builder, char *str) {

    return _search_last_occurrence(builder, str, builder->index);
}

long string_builder_lastIndexOfFrom(StringBuilder *builder, char *str, long fromIndex) {

    return _search_last_occurrence(builder, str, fromIndex);
}

int string_builder_compareTo(StringBuilder *builder, StringBuilder *other) {

    if (builder == other) {
        return 0;
    }

    char *a, *b;
    for (a = builder->str, b = other->str; (*a != '\0') && (*b != '\0') && (*a == *b); a++, b++);

    return ( *a - *b );
}

static void _scrub_char_builder(char *builder, long start, long end) {

    long i;
    for (i = start; i < end; i++) {
        builder[i] = '\0';
    }
}

static Status _delete_substring(StringBuilder *builder, long start, long end) {

    if (builder->index == 0L) {
        return STRUCT_EMPTY;
    }
    if (start < 0 || start >= builder->index || start > end) {
        return INVALID_INDEX;
    }

    long i;
    long delta = end - start;
    for (i = end; i < builder->index; i++) {
        builder->str[i - delta] = builder->str[i];
    }

    _scrub_char_builder(builder->str, builder->index - delta, builder->index);
    builder->index -= delta;

    return OK;
}

Status string_builder_delete(StringBuilder *builder, long start, long end) {

    return _delete_substring(builder, start, end);
}

Status string_builder_deleteCharAt(StringBuilder *builder, long index) {

    return _delete_substring(builder, index, index + 1);
}

void string_builder_reverse(StringBuilder *builder) {

    char temp;
    long head, tail;
    for (head = 0, tail = builder->index - 1; head < tail; head++, tail--) {
        temp = builder->str[head];
        builder->str[head] = builder->str[tail];
        builder->str[tail] = temp;
    }
}

Status string_builder_ensureCapacity(StringBuilder *builder, long capacity) {

    // Only extend if capacity < newCapacity
    if (capacity > MAX_CAPACITY) {
        capacity = MAX_CAPACITY;
    }
    if (builder->capacity < capacity) {
        if (_ensure_capacity(builder, capacity) == FALSE) {
            return ALLOC_FAILURE;
        }
    }

    return OK;
}

Status string_builder_trimToSize(StringBuilder *builder) {

    if (builder->index == 0L)
        return STRUCT_EMPTY;

    if (builder->index != builder->capacity)
        if (_ensure_capacity(builder, builder->index) == FALSE)
            return ALLOC_FAILURE;

    return OK;
}

void string_builder_clear(StringBuilder *builder) {

    _scrub_char_builder(builder->str, 0L, builder->index);
}

long string_builder_length(StringBuilder *builder) {

    return builder->index;
}

long string_builder_capacity(StringBuilder *builder) {

    return builder->capacity;
}

Status string_builder_toString(StringBuilder *builder, char **result) {

    Status status = OK;
    long i;
    char *str;
    size_t bytes = ( ( builder->index + 1 ) * sizeof(char) );
    if ((str = (char *)malloc(bytes)) != NULL) {
        for (i = 0; i < builder->index; i++) {
            str[i] = builder->str[i];
        }
        str[i] = '\0';
        *result = str;
    } else {
        status = ALLOC_FAILURE;
    }

    return status;
}

void string_builder_destroy(StringBuilder *builder) {

    _scrub_char_builder(builder->str, 0, builder->index);
    free(builder->str);
    free(builder);
}
