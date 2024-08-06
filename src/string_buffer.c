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
#include <limits.h>
#include "string_buffer.h"

struct string_buffer {

};

Status stringbuffer_new(StringBuffer **buffer, long capacity, char *str);

Status stringbuffer_appendChar(StringBuffer *buffer, char ch){ return OK; }

Status stringbuffer_appendStr(StringBuffer *buffer, char *str){ return OK; }

Status stringbuffer_appendBoolean(StringBuffer *buffer, Boolean b){ return OK; }

Status stringbuffer_appendShort(StringBuffer *buffer, short s){ return OK; }

Status stringbuffer_appendInt(StringBuffer *buffer, int i){ return OK; }

Status stringbuffer_appendLong(StringBuffer *buffer, long i){ return OK; }

Status stringbuffer_appendFloat(StringBuffer *buffer, float f){ return OK; }

Status stringbuffer_appendDouble(StringBuffer *buffer, double d){ return OK; }

Status stringbuffer_appendSubStr(StringBuffer *buffer, char *str, int start, int end){ return OK; }

Status stringbuffer_appendStrSubSequence(StringBuffer *buffer, char *str, int offset, int len){ return OK; }

Status stringbuffer_appendStrBuffer(StringBuffer *buffer, StringBuffer *other){ return OK; }

Status stringbuffer_insertChar(StringBuffer *buffer, long offset, char ch){ return OK; }

Status stringbuffer_insertStr(StringBuffer *buffer, long offset, char *str){ return OK; }

Status stringbuffer_insertBoolean(StringBuffer *buffer, long offset, Boolean b){ return OK; }

Status stringbuffer_insertShort(StringBuffer *buffer, long offset, short s){ return OK; }

Status stringbuffer_insertInt(StringBuffer *buffer, long offset, int i){ return OK; }

Status stringbuffer_insertLong(StringBuffer *buffer, long offset, long l){ return OK; }

Status stringbuffer_insertFloat(StringBuffer *buffer, long offset, float f){ return OK; }

Status stringbuffer_insertDouble(StringBuffer *buffer, long offset, double d){ return OK; }

Status stringbuffer_insertSubStr(StringBuffer *buffer, long offset, char *str, int start, int end){ return OK; }

Status stringbuffer_insertStrSubSequence(StringBuffer *buffer, long index, char *str, int offset, int len){ return OK; }

Status stringbuffer_insertStrBuffer(StringBuffer *buffer, long offset, StringBuffer *other){ return OK; }

Status stringbuffer_replace(StringBuffer *buffer, int start, int end, char *str){ return OK; }

Status stringbuffer_charAt(StringBuffer *buffer, long i, char *result){ return OK; }

Status stringbuffer_substring(StringBuffer *buffer, long start, char **result){ return OK; }

Status stringbuffer_subsequence(StringBuffer *buffer, long start, long end, char **result){ return OK; }

Status stringbuffer_getChars(StringBuffer *buffer, long srcBegin, long srcEnd, char dst[], int dstBegin){ return OK; }

Status stringbuffer_setCharAt(StringBuffer *buffer, long index, char ch){ return OK; }

Status stringbuffer_setLength(StringBuffer *buffer, long len, char padding){ return OK; }

long stringbuffer_indexOf(StringBuffer *buffer, char *str){ return 0l; }

long stringbuffer_indexOfFrom(StringBuffer *buffer, char *str, long fromIndex){ return 0L; }

long stringbuffer_lastIndexOf(StringBuffer *buffer, char *str){ return 0L; }

long stringbuffer_lastIndexOfFrom(StringBuffer *buffer, char *str, long fromIndex){ return 0L; }

int stringbuffer_compareTo(StringBuffer *buffer, StringBuffer *other){ return 0; }

Status stringbuffer_delete(StringBuffer *buffer, long start, long end){ return OK; }

Status stringbuffer_deleteCharAt(StringBuffer *buffer, long index){ return OK; }

Status stringbuffer_reverse(StringBuffer *buffer){ return OK; }

Status stringbuffer_ensureCapacity(StringBuffer *buffer, long capacity){ return OK; }

Status stringbuffer_trimToSize(StringBuffer *buffer){ return OK; }

void stringbuffer_clear(StringBuffer *buffer){ return OK; }

long stringbuffer_length(StringBuffer *buffer) { return 0L; }

long stringbuffer_capacity(StringBuffer *buffer) { return 0L; }

Status stringbuffer_toString(StringBuffer *buffer, char **result) { return OK; }

void stringbuffer_destroy(StringBuffer *buffer) {}
