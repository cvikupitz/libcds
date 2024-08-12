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

#include <pthread.h>
#include <stdlib.h>
#include "string_builder.h"
#include "ts_string_builder.h"

/**
 * Struct for the thread-safe string builder.
 */
struct ts_string_builder {
    pthread_mutex_t lock;
    StringBuilder *instance;
};

// Macro used for locking the string builder `s`
#define LOCK(s)    pthread_mutex_lock( &((s)->lock) )
// Macro used for unlocking the string builder `s`
#define UNLOCK(s)  pthread_mutex_unlock( &((s)->lock) )

Status ts_string_builder_new(ConcurrentStringBuilder **builder, long capacity, float growthFactor,
                             char *str) {

    ConcurrentStringBuilder *temp;
    Status status;
    pthread_mutexattr_t attr;

    temp = (ConcurrentStringBuilder *)malloc(sizeof(ConcurrentStringBuilder));
    if (temp == NULL) {
        return ALLOC_FAILURE;
    }

    status = string_builder_new(&(temp->instance), capacity, growthFactor, str);
    if (status != OK) {
        free(temp);
        return status;
    }

    /* Creates the pthread_mutex for locking */
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&(temp->lock), &attr);
    pthread_mutexattr_destroy(&attr);
    *builder = temp;

    return OK;
}

void ts_string_builder_lock(ConcurrentStringBuilder *builder) {
    LOCK(builder);
}

void ts_string_builder_unlock(ConcurrentStringBuilder *builder) {
    UNLOCK(builder);
}

Status ts_string_builder_appendChar(ConcurrentStringBuilder *builder, char ch) {

    LOCK(builder);
    Status status = string_builder_appendChar(builder->instance, ch);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_appendStr(ConcurrentStringBuilder *builder, char *str) {

    LOCK(builder);
    Status status = string_builder_appendStr(builder->instance, str);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_appendBoolean(ConcurrentStringBuilder *builder, Boolean b) {

    LOCK(builder);
    Status status = string_builder_appendBoolean(builder->instance, b);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_appendShort(ConcurrentStringBuilder *builder, short s) {

    LOCK(builder);
    Status status = string_builder_appendShort(builder->instance, s);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_appendInt(ConcurrentStringBuilder *builder, int i) {

    LOCK(builder);
    Status status = string_builder_appendInt(builder->instance, i);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_appendLong(ConcurrentStringBuilder *builder, long i) {

    LOCK(builder);
    Status status = string_builder_appendLong(builder->instance, i);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_appendFloat(ConcurrentStringBuilder *builder, float f) {

    LOCK(builder);
    Status status = string_builder_appendFloat(builder->instance, f);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_appendDouble(ConcurrentStringBuilder *builder, double d) {

    LOCK(builder);
    Status status = string_builder_appendDouble(builder->instance, d);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_appendSubStr(ConcurrentStringBuilder *builder, char *str, int start,
                                      int end) {

    LOCK(builder);
    Status status = string_builder_appendSubStr(builder->instance, str, start, end);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_appendStrSubSequence(ConcurrentStringBuilder *builder, char *str,
                                              int offset, int len) {

    LOCK(builder);
    Status status = string_builder_appendStrSubSequence(builder->instance, str, offset, len);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_appendStrBuilder(ConcurrentStringBuilder *builder,
                                          ConcurrentStringBuilder *other) {

    LOCK(builder);
    LOCK(other);
    Status status = string_builder_appendStrBuilder(builder->instance, other->instance);
    UNLOCK(other);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_insertChar(ConcurrentStringBuilder *builder, long offset, char ch) {

    LOCK(builder);
    Status status = string_builder_insertChar(builder->instance, offset, ch);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_insertStr(ConcurrentStringBuilder *builder, long offset, char *str) {

    LOCK(builder);
    Status status = string_builder_insertStr(builder->instance, offset, str);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_insertBoolean(ConcurrentStringBuilder *builder, long offset, Boolean b) {

    LOCK(builder);
    Status status = string_builder_insertBoolean(builder->instance, offset, b);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_insertShort(ConcurrentStringBuilder *builder, long offset, short s) {

    LOCK(builder);
    Status status = string_builder_insertShort(builder->instance, offset, s);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_insertInt(ConcurrentStringBuilder *builder, long offset, int i) {

    LOCK(builder);
    Status status = string_builder_insertInt(builder->instance, offset, i);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_insertLong(ConcurrentStringBuilder *builder, long offset, long l) {

    LOCK(builder);
    Status status = string_builder_insertLong(builder->instance, offset, l);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_insertFloat(ConcurrentStringBuilder *builder, long offset, float f) {

    LOCK(builder);
    Status status = string_builder_insertFloat(builder->instance, offset, f);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_insertDouble(ConcurrentStringBuilder *builder, long offset, double d) {

    LOCK(builder);
    Status status = string_builder_insertDouble(builder->instance, offset, d);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_insertSubStr(ConcurrentStringBuilder *builder, long offset, char *str,
                                      int start, int end) {

    LOCK(builder);
    Status status = string_builder_insertSubStr(builder->instance, offset, str, start, end);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_insertStrSubSequence(ConcurrentStringBuilder *builder, long index,
                                              char *str, int offset, int len) {

    LOCK(builder);
    Status status = string_builder_insertStrSubSequence(builder->instance, index, str, offset, len);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_insertStrBuilder(ConcurrentStringBuilder *builder, long offset,
                                          ConcurrentStringBuilder *other) {

    LOCK(builder);
    LOCK(other);
    Status status = string_builder_insertStrBuilder(builder->instance, offset, other->instance);
    UNLOCK(other);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_replace(ConcurrentStringBuilder *builder, long start, long end, char *str)
{

    LOCK(builder);
    Status status = string_builder_replace(builder->instance, start, end, str);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_charAt(ConcurrentStringBuilder *builder, long i, char *result) {

    LOCK(builder);
    Status status = string_builder_charAt(builder->instance, i, result);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_substring(ConcurrentStringBuilder *builder, long start, char **result) {

    LOCK(builder);

    Status status = string_builder_substring(builder->instance, start, result);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_subsequence(ConcurrentStringBuilder *builder, long start, long end,
                                     char **result) {

    LOCK(builder);
    Status status = string_builder_subsequence(builder->instance, start, end, result);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_getChars(ConcurrentStringBuilder *builder, long srcBegin, long srcEnd,
                                  char dst[], int dstBegin) {

    LOCK(builder);
    Status status = string_builder_getChars(builder->instance, srcBegin, srcEnd, dst, dstBegin);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_setCharAt(ConcurrentStringBuilder *builder, long index, char ch) {

    LOCK(builder);
    Status status = string_builder_setCharAt(builder->instance, index, ch);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_setLength(ConcurrentStringBuilder *builder, long len, char padding) {

    LOCK(builder);
    Status status = string_builder_setLength(builder->instance, len, padding);
    UNLOCK(builder);

    return status;
}

long ts_string_builder_indexOf(ConcurrentStringBuilder *builder, char *str) {

    LOCK(builder);
    long idx = string_builder_indexOf(builder->instance, str);
    UNLOCK(builder);

    return idx;
}

long ts_string_builder_indexOfFrom(ConcurrentStringBuilder *builder, char *str, long fromIndex) {

    LOCK(builder);
    long idx = string_builder_indexOfFrom(builder->instance, str, fromIndex);
    UNLOCK(builder);

    return idx;
}

long ts_string_builder_lastIndexOf(ConcurrentStringBuilder *builder, char *str) {

    LOCK(builder);
    long idx = string_builder_lastIndexOf(builder->instance, str);
    UNLOCK(builder);

    return idx;
}

long ts_string_builder_lastIndexOfFrom(ConcurrentStringBuilder *builder, char *str, long fromIndex)
{

    LOCK(builder);
    long idx = string_builder_lastIndexOfFrom(builder->instance, str, fromIndex);
    UNLOCK(builder);

    return idx;
}

int ts_string_builder_compareTo(ConcurrentStringBuilder *builder, ConcurrentStringBuilder *other) {

    LOCK(builder);
    LOCK(other);
    int result = string_builder_compareTo(builder->instance, other->instance);
    UNLOCK(other);
    UNLOCK(builder);

    return result;
}

Status ts_string_builder_delete(ConcurrentStringBuilder *builder, long start, long end) {

    LOCK(builder);
    Status status = string_builder_delete(builder->instance, start, end);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_deleteCharAt(ConcurrentStringBuilder *builder, long index) {

    LOCK(builder);
    Status status = string_builder_deleteCharAt(builder->instance, index);
    UNLOCK(builder);

    return status;
}

void ts_string_builder_reverse(ConcurrentStringBuilder *builder) {

    LOCK(builder);
    string_builder_reverse(builder->instance);
    UNLOCK(builder);
}

Status ts_string_builder_ensureCapacity(ConcurrentStringBuilder *builder, long capacity) {

    LOCK(builder);
    Status status = string_builder_ensureCapacity(builder->instance, capacity);
    UNLOCK(builder);

    return status;
}

Status ts_string_builder_trimToSize(ConcurrentStringBuilder *builder) {

    LOCK(builder);
    Status status = string_builder_trimToSize(builder->instance);
    UNLOCK(builder);

    return status;
}

void ts_string_builder_clear(ConcurrentStringBuilder *builder) {

    LOCK(builder);
    string_builder_clear(builder->instance);
    UNLOCK(builder);
}

long ts_string_builder_length(ConcurrentStringBuilder *builder) {

    LOCK(builder);
    long len = string_builder_length(builder->instance);
    UNLOCK(builder);

    return len;
}

long ts_string_builder_capacity(ConcurrentStringBuilder *builder) {

    LOCK(builder);
    long capacity = string_builder_capacity(builder->instance);
    UNLOCK(builder);

    return capacity;
}

Status ts_string_builder_toString(ConcurrentStringBuilder *builder, char **result) {

    LOCK(builder);
    Status status = string_builder_toString(builder->instance, result);
    UNLOCK(builder);

    return status;
}

void ts_string_builder_destroy(ConcurrentStringBuilder *builder) {

    LOCK(builder);
    string_builder_destroy(builder->instance);
    UNLOCK(builder);
    pthread_mutex_destroy(&(builder->lock));
    free(builder);
}
