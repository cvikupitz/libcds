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
#include <limits.h>     /* Importing to use ULLONG_MAX */
#include "string_buffer.h"

struct string_buffer {
    char *content;
    long index;
    unsigned long long capacity;
};

/* The default capacity to assign when the capacity give is invalid */
#define DEFAULT_CAPACITY 256L
/* The maximum capacity each buffer instance is allowed */
#define MAX_CAPACITY ( ULLONG_MAX - 1 )

Status stringbuffer_new(StringBuffer **buffer, char *str, long capacity) {

    StringBuffer *temp = (StringBuffer *)malloc(sizeof(StringBuffer));
    if (temp == NULL)
        return ALLOC_FAILURE;

    long cap = ( capacity <= 0L ) ? DEFAULT_CAPACITY : capacity;
    if (cap > MAX_CAPACITY)
        cap = MAX_CAPACITY;
    size_t bytes = ( cap * sizeof(char) );
    char *content = (char *)malloc(bytes);

    if (content == NULL) {
        free(temp);
        return ALLOC_FAILURE;l
    }

    long i;
    for (i = 0L; i < cap; i++)
        content[i] = '\0';
    temp->content = content;
    temp->size = 0L;
    temp->capacity = cap;
    *buffer = temp;

    return OK;
}

/* Macro to check if the buffer is currently empty */
#define IS_EMPTY(x)  ( ((x)->index == 0L) ? TRUE : FALSE )
/* Macro used to validate the given index i */
#define INDEX_VALID(i, N) ( ( 0L <= (i) && (i) < (N) ) ? TRUE : FALSE )

/*
 * Extends the arraylist's capacity to the new specified capacity. Returns TRUE if
 * extension was successful, or FALSE if not.
 */
static Boolean ensureCapacity(StringBuffer *buffer, unsigned long long newCapacity) {

    Boolean status = FALSE;
    size_t bytes = ( newCapacity * sizeof(char) + 1);
    char *temp = (char *)realloc(buffer->content, bytes);

    if (temp != NULL) {
        /* Update attributes after extension */
        list->data = temp;
        long i;
        for (i = list->capacity; i < newCapacity; i++)
            list->data[i] = NULL;
        list->capacity = newCapacity;
        status = TRUE;
    }

    return status;
}

// appends
Status stringbuffer_appendChar(StringBuffer *buffer, char ch);
Status stringbuffer_appendStr(StringBuffer *buffer, char *str);
Status stringbuffer_appendStrSlice(StringBuffer *buffer, char *str, int start, int end);
Status stringbuffer_appendStrFromOffset(StringBuffer *buffer, char *str, int offset, int len);
Status stringbuffer_appendStrBuffer(StringBuffer *buffer, StringBuffer *other);
Status stringbuffer_repeat(StringBuffer *buffer, char *str, int count);

// inserts
Status stringbuffer_insertChar(StringBuffer *buffer, long offset, char ch);
Status stringbuffer_insertStr(StringBuffer *buffer, long offset, char *str);
Status stringbuffer_insertStrSlice(StringBuffer *buffer, long offset, char *str, int start, int end);
Status stringbuffer_insertStrFromOffset(StringBuffer *buffer, long index, char *str, int offset, int len);
Status stringbuffer_insertStrBuffer(StringBuffer *buffer, long offset, StringBuffer *other);
Status stringbuffer_replace(StringBuffer *buffer, int start, int end, char *str);

// gets/sets
char stringbuffer_charAt(StringBuffer *buffer, long i) {

}

const char* stringbuffer_substring(StringBuffer *buffer, long start);
const char* stringbuffer_subsequence(StringBuffer *buffer, long start, long end);
Status stringbuilder_getChars(StringBuffer *buffer, int srcBegin, int srcEnd, char dst, int dstBegin);
Status stringbuilder_setCharAt(StringBuffer *buffer, long index, char ch);
Status stringbuilder_setLength(StringBuffer *buffer, long len);

// searches
long stringbuffer_indexOf(StringBuffer *buffer, char *str);
long stringbuffer_indexOfFrom(StringBuffer *buffer, char *str, long fromIndex);
long stringbuffer_lastIndexOf(StringBuffer *buffer, char *str);
long stringbuffer_lastIndexOfFrom(StringBuffer *buffer, char *str, long fromIndex);

// deletes
Status stringbuffer_delete(StringBuffer *buffer, long start, long end);
Status stringbuffer_deleteCharAt(StringBuffer *buffer, long index);

Status stringbuffer_reverse(StringBuffer *buffer) ;

Status stringbuffer_ensureCapacity(StringBuffer *buffer, unsigned long long capacity) {

    if (buffer->capacity < capacity)
        if (ensureCapacity(list, capacity) == FALSE)
            return ALLOC_FAILURE;

    return OK;
}

Status stringbuffer_trimToSize(StringBuffer *buffer) {

    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;

    /* Only trim if size < capacity */
    if (list->size != list->capacity)
        if (ensureCapacity(list, list->size) == FALSE)
            return ALLOC_FAILURE;

    return OK;
}

static void clean_char_buffer(char *buffer, long len) {
    long i;
    for (i = 0L; i < buffer->capacity; i++)
        buffer->content[i] = '\0';
}

void stringbuffer_clear(StringBuffer *buffer) {
    clean_char_buffer(buffer->content, buffer->capacity);
    buffer->index = 0L;
}

long stringbuffer_length(StringBuffer *buffer) {
    return buffer->index;
}

long stringbuffer_capacity(StringBuffer *buffer) {
    return buffer->capacity;
}

Status stringbuffer_toString(StringBuffer *buffer) {

    char *str;
    int i;
    size_t bytes = (( sizeof(char) * buffer->index ) + 1 );

    if (buffer->index == 0L)
        return STRUCT_EMPTY;
    if ((str = malloc(bytes)) == NULL)
        return ALLOC_ERROR;
    for (i = 0; i < buffer->index; i++)
        str[i] = buffer->content[i];
    str[i] = '\0';

    return OK;
}

void stringbuffer_destroy(*StringBuffer buffer) {
    clean_char_buffer(buffer->content, buffer->capacity);
    free(buffer->content);
    free(buffer);
}
