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

#ifndef _CDS_STRING_BUFFER_H__
#define _CDS_STRING_BUFFER_H__

#include "cds_common.h"

typedef struct string_buffer StringBuffer;

Status stringbuffer_new(StringBuffer **buffer, char *str, long capacity);

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
char stringbuffer_charAt(StringBuffer *buffer, long i);
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

Status stringbuffer_reverse(StringBuffer *buffer);
Status stringbuffer_ensureCapacity(StringBuffer *buffer, long capacity);
Status stringbuffer_trimToSize(StringBuffer *buffer);

void stringbuffer_clear(StringBuffer *buffer);

long stringbuffer_length(StringBuffer *buffer);

long stringbuffer_capacity(StringBuffer *buffer);

const char *stringbuffer_toString(StringBuffer *buffer);

void stringbuffer_destroy(*StringBuffer buffer);

#endif  /* _CDS_STRING_BUFFER_H__ */


/*
TODO:
append/insert int, float, boolean, etc.
chars()
code point operations

*/