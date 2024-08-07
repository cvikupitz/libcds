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

#include <stdio.h>
#include <stdlib.h>     /* Required for memory allocation like `malloc()`, `realloc()`, `free()`, etc */
#include <limits.h>     /* Required for upper limits on int/long types to avoid buffer overflowing */
#include "string_buffer.h"

struct string_buffer {
    char *str;
    long index;
    long capacity;
};

/* The default capacity to assign when the capacity give is invalid */
#define DEFAULT_CAPACITY 16L
/* The maximum capacity allowed (to avoid buffer overflow), must allow one extra byte for null terminator */
#define MAX_CAPACITY ( LONG_MAX - 1L )
/* The percentage (expressed as a decimal [0.0, 1.0]) the buffer will increase in capacity when required */
#define GROWTH_FACTOR 0.08f

#define UNUSED __attribute__((unused))

/*
 * Local method to compute the length of the specified string `str`.
 */
static int _get_str_length(char *str) {

    char *ch;
    for (ch = str; *ch != '\0'; ch++) /* Navigate to end of string */;
    return ( ch - str );
}

Status string_buffer_new(StringBuffer **buffer, long capacity, char *str) {

    /* Allocate the struct, check for allocation failures */
    StringBuffer *temp = (StringBuffer *)malloc(sizeof(StringBuffer));
    if (temp == NULL) {
        return ALLOC_FAILURE;
    }

    /* Set up capacity, initialize the reminaing struct members */
    long cap = ( capacity <= 0L ) ? DEFAULT_CAPACITY : capacity;
    /* If initial string is supplied, must ensure starting capacity >= string length */
    if (str != NULL) {
        int len = _get_str_length(str);
        if (cap < len) {
            cap = len;
        }
    }
    /* Ensure capacity does not exceed max allowed */
    if (cap > MAX_CAPACITY) {
        cap = MAX_CAPACITY;
    }

    size_t bytes = ( (cap + 1L) * sizeof(char) );
    char *innerBuffer = (char *)malloc(bytes);

    /* Checks for allocation failures */
    if (innerBuffer == NULL) {
        free(temp);
        return ALLOC_FAILURE;
    }

    /* Initializes the remainder of struct members */
    long i;
    for (i = 0L; i < cap; i++) {
        innerBuffer[i] = '\0';
    }
    temp->str = innerBuffer;
    temp->index = 0L;
    temp->capacity = cap;
    *buffer = temp;

    return OK;
}

/* Macro to detect an overflow if `a` and `b` are added together */
#define ADD_OVERFLOWS(a, b) ( ( b > ( MAX_CAPACITY - a ) ) ? TRUE : FALSE )

static Boolean _ensure_capacity(StringBuffer *buffer, long newCapacity) {

    Boolean status = FALSE;
    size_t bytes = ( (newCapacity + 1) * sizeof(char) );
    char *temp = (char *)realloc(buffer->str, bytes);

    if (temp != NULL) {
        /* Update attributes after extension */
        buffer->str = temp;
        long i;
        for (i = buffer->capacity; i < newCapacity + 1; i++) {
            buffer->str[i] = '\0';
        }
        buffer->capacity = newCapacity;
        status = TRUE;
    }

    return status;
}

static long _compute_next_capacity_increase(StringBuffer *buffer, int len) {

    long delta = 0L;
    long curr_rem = ( buffer->capacity - buffer->index );

    /* There's not enough room in buffer to store the string, need to increase capacity */
    if (len > curr_rem) {

        /*
         * Checks the total number of characters that are available without
         * causing an overflow with this buffer. If the string still can't be
         * allocated after resizing the max capacity allowed, then we cannot append
         * and must return an allocation error.
         */
        long total_rem = ( MAX_CAPACITY - buffer->capacity + curr_rem );
        if (len > total_rem) {
            return -1L;
        }

        /*
         * Computes the `delta`, that is, the amount to grow the buffer. We
         * will add the remaining characters needed for the string, plus
         * some additional room to avoid reallocating every call. We also must
         * check for buffer overflow again, but can just set it to the max
         * capacity if so.
         */
        delta = (long)(buffer->capacity * GROWTH_FACTOR) + 1L;
        if ( ADD_OVERFLOWS(buffer->capacity, delta) ) {
            delta = MAX_CAPACITY - buffer->capacity;
        }
    }

    return delta;
}

static Status _insert_str(StringBuffer *buffer, long offset, char *str) {

    char *ch;
    int len = _get_str_length(str);
    long delta = _compute_next_capacity_increase(buffer, len);

    if (delta < 0L) {
        return ALLOC_FAILURE;
    } else if (delta > 0L) {
        if (_ensure_capacity(buffer, buffer->capacity + delta) == FALSE) {
            return ALLOC_FAILURE;
        }
    }

    //FIXME - shift chars right as needed

    for (ch = str; *ch != '\0'; ch++) {
        buffer->str[buffer->index++] = *ch;
    }

    return OK;
}

Status string_buffer_appendChar(StringBuffer *buffer, char ch) {

    char str[] = { ch, '\0' };
    return _insert_str(buffer, buffer->index, str);
}

Status string_buffer_appendStr(StringBuffer *buffer, char *str) {

    if (str == NULL) {
        return OK;
    }
    return _insert_str(buffer, buffer->index, str);
}

Status string_buffer_appendBoolean(StringBuffer *buffer, Boolean b) {

    char *str = ( b == TRUE ) ? "true" : "false";
    return _insert_str(buffer, buffer->index, str);
}

/* The size of the temporary buffer where non-string conversion results are placed */
#define BUFFER_SIZE 2048

Status string_buffer_appendShort(StringBuffer *buffer, short s) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%hd", s);
    return _insert_str(buffer, buffer->index, temp);
}

Status string_buffer_appendInt(StringBuffer *buffer, int i) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%d", i);
    return _insert_str(buffer, buffer->index, temp);
}

Status string_buffer_appendLong(StringBuffer *buffer, long l) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%ld", l);
    return _insert_str(buffer, buffer->index, temp);
}

Status string_buffer_appendFloat(StringBuffer *buffer, float f) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%f", f);
    return _insert_str(buffer, buffer->index, temp);
}

Status string_buffer_appendDouble(StringBuffer *buffer, double d) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%lf", d);
    return _insert_str(buffer, buffer->index, temp);
}

Status string_buffer_appendSubStr(StringBuffer *buffer, char *str, int start, int end) {

    if ( (start < 0) || (start > end) || (end > buffer->index) ) {
        return INVALID_INDEX;
    }

    if (str == NULL) {
        return OK;
    }

    int i, j, len = (end - start) + 1;
    char temp[len];
    for (i = 0, j = start; i < len; i++, j++) {
        temp[i] = str[j];
    }
    temp[i] = '\0';

    return _insert_str(buffer, buffer->index, temp);
}

Status string_buffer_appendStrSubSequence(StringBuffer *buffer, char *str, int offset, int len) {

    if ( (offset < 0) || (len < 0) || (offset + len > buffer->index) ) {
        return INVALID_INDEX;
    }

    if (str == NULL) {
        return OK;
    }

    int i, j = offset;
    char temp[len + 1];
    for (i = 0; i < len; i++, j++) {
        temp[i] = str[j];
    }
    temp[i] = '\0';

    return _insert_str(buffer, buffer->index, temp);
}

Status string_buffer_appendStrBuffer(StringBuffer *buffer, StringBuffer *other) {

    if (other == NULL || other->index == 0L) {
        return OK;
    }

    return _insert_str(buffer, buffer->index, other->str);
}

Status string_buffer_insertChar(StringBuffer *buffer, long offset, char ch) {

    char str[] = { ch, '\0' };
    return _insert_str(buffer, offset, str);
}

Status string_buffer_insertStr(StringBuffer *buffer, long offset, char *str) {

    if (str == NULL) {
        return OK;
    }
    return _insert_str(buffer, offset, str);
}

Status string_buffer_insertBoolean(StringBuffer *buffer, long offset, Boolean b) {

    char *str = ( b == TRUE ) ? "true" : "false";
    return _insert_str(buffer, offset, str);
}

Status string_buffer_insertShort(StringBuffer *buffer, long offset, short s) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%hd", s);
    return _insert_str(buffer, offset, temp);
}

Status string_buffer_insertInt(StringBuffer *buffer, long offset, int i) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%d", i);
    return _insert_str(buffer, offset, temp);
}

Status string_buffer_insertLong(StringBuffer *buffer, long offset, long l) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%ld", l);
    return _insert_str(buffer, offset, temp);
}

Status string_buffer_insertFloat(StringBuffer *buffer, long offset, float f) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%f", f);
    return _insert_str(buffer, offset, temp);
}

Status string_buffer_insertDouble(StringBuffer *buffer, long offset, double d) {

    char temp[BUFFER_SIZE];
    sprintf(temp, "%lf", d);
    return _insert_str(buffer, offset, temp);
}

Status string_buffer_insertSubStr(UNUSED StringBuffer *buffer, UNUSED long offset, UNUSED char *str, UNUSED int start, UNUSED int end) { return OK; }

Status string_buffer_insertStrSubSequence(UNUSED StringBuffer *buffer, UNUSED long index, UNUSED char *str, UNUSED int offset, UNUSED int len) { return OK; }

Status string_buffer_insertStrBuffer(UNUSED StringBuffer *buffer, UNUSED long offset, UNUSED StringBuffer *other) { return OK; }

Status string_buffer_replace(UNUSED StringBuffer *buffer, UNUSED int start, UNUSED int end, UNUSED char *str) { return OK; }

Status string_buffer_charAt(UNUSED StringBuffer *buffer, UNUSED long i, UNUSED char *result) { return OK; }

Status string_buffer_substring(UNUSED StringBuffer *buffer, UNUSED long start, UNUSED char **result) { return OK; }

Status string_buffer_subsequence(UNUSED StringBuffer *buffer, UNUSED long start, UNUSED long end, UNUSED char **result) { return OK; }

Status string_buffer_getChars(UNUSED StringBuffer *buffer, UNUSED long srcBegin, UNUSED long srcEnd, UNUSED char dst[], UNUSED int dstBegin) { return OK; }

Status string_buffer_setCharAt(UNUSED StringBuffer *buffer, UNUSED long index, UNUSED char ch) { return OK; }

Status string_buffer_setLength(UNUSED StringBuffer *buffer, UNUSED long len, UNUSED char padding) { return OK; }

long string_buffer_indexOf(UNUSED StringBuffer *buffer, UNUSED char *str) { return 0l; }

long string_buffer_indexOfFrom(UNUSED StringBuffer *buffer, UNUSED char *str, UNUSED long fromIndex) { return 0L; }

long string_buffer_lastIndexOf(UNUSED StringBuffer *buffer, UNUSED char *str) { return 0L; }

long string_buffer_lastIndexOfFrom(UNUSED StringBuffer *buffer, UNUSED char *str, UNUSED long fromIndex) { return 0L; }

int string_buffer_compareTo(StringBuffer *buffer, StringBuffer *other) {

    if (buffer == other) {
        return 0;
    }

    char *a, *b;
    for (a = buffer->str, b = other->str; *a != '\0' && *b != '\0' && *a == *b; a++, b++);
    return ( *a - *b );
}

static void _scrub_char_buffer(char *buffer, long start, long end) {

    long i;
    for (i = start; i < end; i++) {
        buffer[i] = '\0';
    }
}

static Status _delete_substring(StringBuffer *buffer, long start, long end) {

    if (buffer->index == 0L) {
        return STRUCT_EMPTY;
    }
    if (start < 0 || start >= buffer->index || start > end) {
        return INVALID_INDEX;
    }

    long i;
    long delta = end - start;
    for (i = end; i < buffer->index; i++) {
        buffer->str[i - delta] = buffer->str[i];
    }

    _scrub_char_buffer(buffer->str, buffer->index - delta, buffer->index);
    buffer->index -= delta;

    return OK;
}

Status string_buffer_delete(StringBuffer *buffer, long start, long end) {

    return _delete_substring(buffer, start, end);
}

Status string_buffer_deleteCharAt(StringBuffer *buffer, long index) {

    return _delete_substring(buffer, index, index + 1);
}

void string_buffer_reverse(StringBuffer *buffer) {

    char temp;
    long head, tail;
    for (head = 0, tail = buffer->index; head < tail; head++, tail--) {
        temp = buffer->str[head];
        buffer->str[head] = buffer->str[tail];
        buffer->str[tail] = temp;
    }
}

Status string_buffer_ensureCapacity(StringBuffer *buffer, long capacity) {

    /* Only extend if capacity < newCapacity */
    if (capacity > MAX_CAPACITY) {
        capacity = MAX_CAPACITY;
    }
    if (buffer->capacity < capacity)
        if (_ensure_capacity(buffer, capacity) == FALSE)
            return ALLOC_FAILURE;

    return OK;
}

Status string_buffer_trimToSize(StringBuffer *buffer) {

    if (buffer->index == 0L)
        return STRUCT_EMPTY;

    if (buffer->index != buffer->capacity)
        if (_ensure_capacity(buffer, buffer->index) == FALSE)
            return ALLOC_FAILURE;

    return OK;
}

void string_buffer_clear(StringBuffer *buffer) {

    _scrub_char_buffer(buffer->str, 0L, buffer->index);
}

long string_buffer_length(StringBuffer *buffer) {

    return buffer->index;
}

long string_buffer_capacity(StringBuffer *buffer) {

    return buffer->capacity;
}

Status string_buffer_toString(StringBuffer *buffer, char **result) {

    if (buffer->index == 0L) {
        return STRUCT_EMPTY;
    }

    Status status = OK;
    long i;
    char *str;
    size_t bytes = ((buffer->index + 1) * sizeof(char));
    if ((str = (char *)malloc(bytes)) != NULL) {
        for (i = 0; i < buffer->index; i++) {
            str[i] = buffer->str[i];
        }
        str[i] = '\0';
        *result = str;
    } else {
        status = ALLOC_FAILURE;
    }

    return status;
}

void string_buffer_destroy(StringBuffer *buffer) {

    _scrub_char_buffer(buffer->str, 0, buffer->index);
    free(buffer->str);
    free(buffer);
}
