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

/**
 * Interface for the StringBuffer ADT.
 *
 * The StringBuffer is a mutable sequence of characters. The API provides a way for callers
 * to construct strings by appending and inserting string elements into the buffer. Other
 * functions such as string searching, subsequences, removals, reversing, and others are
 * also provided.
 *
 * Modeled after the Java 7 StringBuilder interface. It was decided to keep this ADT
 * named as the StringBuffer rather than StringBuilder as C does not support the builder
 * constructor design pattern like Java does.
 */
typedef struct string_buffer StringBuffer;

/**
 * Constructs a new string buffer instance with the specified starting capacity, then
 * stores the new instance into '*buffer'. If the capacity given is <= 0, a default
 * capacity is assigned.
 *
 * Caller may also provide a string in which the buffer will be initialized with its
 * contents, or NULL to create a buffer instance with no content. If an initialization
 * string is provided, Caller must ensure that `str` is null-terminated (if not, undefined
 * behavior or errors may occur).
 *
 * Params:
 *    buffer - The pointer address to store the new StringBuffer instance.
 *    capacity - The default capacity of the string buffer.
 *    str - The initial contents to provide the buffer, or NULL for an empty buffer.
 * Returns:
 *    OK - StringBuffer was successfully created.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_new(StringBuffer **buffer, unsigned long capacity, char *str);

/**
 * Appends the character `ch` to the buffer.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    ch - The character to append.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_appendChar(StringBuffer *buffer, char ch);

/**
 * Appends the string `str` to the buffer. Caller must ensure that `str` is
 * null-terminated (if not, undefined behavior or errors may occur).
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    str - The string to append.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_appendStr(StringBuffer *buffer, char *str);

/**
 * Appends a subsequence of `str` to the buffer, given the starting and ending
 * indecies. Characters of the argument `str`, starting at index `start`, are
 * appended, in order, to the contents of this sequence up to the (exclusive)
 * index `end`. The length of this sequence is increased by the value of
 * `end` - `start`.
 *
 * If `str is NULL, then nothing will be appended. Caller must ensure that `str` is
 * null-terminated (if not, undefined behavior or errors may occur).
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    str - The string to append.
 *    start - The starting index of `str` to append (inclusive).
 *    end - The ending index of `str` to append (exclusive).
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index specified is invalid (start < 0 or start > end or
 *                    end > length of str).
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_appendStrSlice(StringBuffer *buffer, char *str, int start, int end);

/**
 * Appends a substring of `str` to the buffer. Characters of the char array `str`,
 * starting at index `offset`, are appended, in order, to the contents of this
 * sequence. The length of this sequence increases by the value of `len`.
 *
 * If `str` is NULL, then nothing will be appended. Caller must ensure that `str`
 * is null-terminated (if not, undefined behavior or errors may occur).
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    str - The string to append.
 *    offset - Index of the first character in str to append.
 *    len - The number of characters to append.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index specified is invalid (start < 0 or start > end or
 *                    end > length of str).
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_appendStrFromOffset(StringBuffer *buffer, char *str, int offset, int len);


Status stringbuffer_appendStrBuffer(StringBuffer *buffer, StringBuffer *other);

Status stringbuffer_repeat(StringBuffer *buffer, char *str, int count);

// inserts
Status stringbuffer_insertChar(StringBuffer *buffer, unsigned long offset, char ch);
Status stringbuffer_insertStr(StringBuffer *buffer, unsigned long offset, char *str);
Status stringbuffer_insertStrSlice(StringBuffer *buffer, unsigned long offset, char *str, int start, int end);
Status stringbuffer_insertStrFromOffset(StringBuffer *buffer, unsigned long index, char *str, int offset, int len);
Status stringbuffer_insertStrBuffer(StringBuffer *buffer, unsigned long offset, StringBuffer *other);
Status stringbuffer_replace(StringBuffer *buffer, int start, int end, char *str);

// gets/sets
char stringbuffer_charAt(StringBuffer *buffer, long i);

/**
 * Allocates a string that is a substring of this buffer, and stores the result into
 * `*result`. The substring negins at the specified index `start` and extends to the
 * end of the current buffer. The returned string is an allocated string from the heap,
 * and is the caller's responsibility to free the string when done using (via stdlib's
 * free() method).
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    start - The beginning index (inclusive).
 *    result - The pointer address to store the allocated string into.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index is invalid; `start` < 0 or `start` > length().
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_substring(StringBuffer *buffer, unsigned long start, char **result);

/**
 * Allocates a string that is a subsequence of this buffer, and stores the result into
 * `*result`. The returned string is an allocated string from the heap, and is the
 * caller's responsibility to free the string when done using (via stdlib's free() method).
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    start - The starting index (inclusive).
 *    end - The ending index (exclusive).
 *    result - The pointer address to store the allocated string into.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - One of the indecies given is invalid:
 *      1.) `start` < 0 or `end < 0
 *      2.) `end` > length()
 *      3.) `start` > `end`
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_subsequence(StringBuffer *buffer, unsigned long start, unsigned long end, char **result);

/**
 * Characters are copied from this sequence into the destination character array `dst`.
 * The first character to be copied is at index `srcBegin`; the last character to be
 * copied is at index `srcEnd` - 1. The total number of characters to be copied is
 * `srcEnd` - `srcBegin`. The characters are copied into the subarray of `dst` starting
 * at index `dstBegin` and ending at index:
 *
 *    dstBegin + (srcEnd - srcBegin) - 1
 *
 * It is the caller's responsibility to ensure that the provided array `dst` is large
 * enough to store the copy results. If not, undefined/erroneous behavior can occur.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    srcBegin - Index where to start the copy (inclusive).
 *    srcEnd - Index where copying will stop.
 *    dst - The array where the data is copied into.
 *    dstBegin - Offset in `dst` where copying will begin.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - One of the indecies given is invalid:
 *       1.) `srcBegin` < 0
 *       2.) `dstBegin` < 0
 *       3.) `srcBegin` > `srcEnd`
 *       4.) `srcEnd` > length()
 */
Status stringbuffer_getChars(StringBuffer *buffer, unsigned long srcBegin, unsigned long srcEnd, char[] dst, int dstBegin);

/**
 * The character at the specified index `index` is set to `ch`. All other characters
 * in all other indecies in the buffer remain unaltered.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    index - The index in the buffer to modify.
 *    ch - The new character.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index given in invalid (is < 0 or > length).
 */
Status stringbuffer_setCharAt(StringBuffer *buffer, unsigned long index, char ch);

/**
 * Sets the length of the buffer's character sequence. The sequence is changed to a
 * new character sequence whose length is specified by the argument `len`. For every
 * nonnegative index k less than `len`, the character at index k in the new character
 * sequence is the same as the character at index k in the old sequence if k is less
 * than the length of the old character sequence; otherwise, it is the null character
 * '\u0000'. In other words, if the `len` argument is less than the current length, the
 * length is changed to the specified length.
 *
 * If `len` is greater than or equal to the current length, sufficient null characters
 * ('\u0000') are appended so that length becomes the `len` argument. The `len` argument
 * must be greater than or equal to 0.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    len - The new length.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - New length given is invalid (len < 0).
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_setLength(StringBuffer *buffer, unsigned long len);

/**
 * Returns the index within this string buffer of the first occurrence of the specified
 * substring `str`. The first occurrence of the empty string "" is considered to occur
 * at the first index (0). If no such value exists, or `str` is NULL, then -1 is returned.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    str - The substring to search for.
 * Returns:
 *    The index of the last occurrence of the substring, or -1 if no such index exists.
 */
unsigned long stringbuffer_indexOf(StringBuffer *buffer, char *str);

/**
 * Returns the index within this string buffer of the first occurrence of the specified
 * substring `str`, starting at the specified index. If no such value exists, or `str`
 * is NULL, then -1 is returned.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    str - The substring to search for.
 *    fromIndex - The index to start the search from.
 * Returns:
 *    The index of the first occurrence of the substring, searching from `fromIndex`,
 *    or -1 if no such index exists.
 */
unsigned long stringbuffer_indexOfFrom(StringBuffer *buffer, char *str, unsigned long fromIndex);

/**
 * Returns the index within this string buffer of the last occurrence of the specified
 * substring `str`. The last occurrence of the empty string "" is considered to occur
 * at the last index (the current length of the buffer). If no such value exists, or
 * `str` is NULL, then -1 is returned.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    str - The substring to search for.
 * Returns:
 *    The index of the last occurrence of the substring, or -1 if no such index exists.
 */
unsigned long stringbuffer_lastIndexOf(StringBuffer *buffer, char *str);

/**
 * Returns the index within this string buffer of the last occurrence of the specified
 * substring `str`, searching backward starting at the specified index. If no such value
 * exists, or `str` is NULL, then -1 is returned.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    str - The substring to search for.
 *    fromIndex - The index to start the search from.
 * Returns:
 *    The index of the last occurrence of the substring, searching backwards from
 *    `fromIndex`, or -1 if no such index exists.
 */
unsigned long stringbuffer_lastIndexOfFrom(StringBuffer *buffer, char *str, unsigned long fromIndex);

/**
 * Compares the two string buffer instances lexicographically.
 *
 * Params:
 *    buffer - The first string buffer to compare.
 *    other - The string buffer to comapre with.
 * Returns:
 *    0 if both `buffer` and `other` contain the same exact string sequence as
 *    each other, <0 if `buffer` is lexographically less than `other`, or >0
 *    if `buffer` is lexographically greater than `other`.
 */
int stringbuffer_compareTo(StringBuffer *buffer, StringBuffer *other);

/**
 * Removes the characters in a substring of this sequence from the buffer. The
 * substring begins at the specified start and extends to the character at index
 * `end` - 1 or to the end of the sequence if no such character exists. If
 * `start` is equal to `end`, no changes are made.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    start - The beginning index (inclusive).
 *    end - The ending index (exclusive).
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - String buffer is currently empty.
 *    INVALID_INDEX - Index given is out of range (start < 0 or start >= length()
 *                    or start > end).
 */
Status stringbuffer_delete(StringBuffer *buffer, unsigned long start, unsigned long end);

/**
 * Removes the character at the specified position in the string buffer.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    i - The index of the character to be removed.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - String buffer is currently empty.
 *    INVALID_INDEX - Index given is out of range (index < 0 or index >= length()).
 */
Status stringbuffer_deleteCharAt(StringBuffer *buffer, unsigned long index);

/**
 * Reverses the contents of the string buffer in place.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Not performed as buffer has no content.
 */
Status stringbuffer_reverse(StringBuffer *buffer);

/**
 * Increases the capacity of the string buffer, if necessary, to ensure that it can
 * hold at most the number of characters specified by the capacity argument before
 * reallocation will occur.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    capacity - The desired maximum capacity.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_ensureCapacity(StringBuffer *buffer, unsigned long capacity);

/**
 * Trims the capacity of the string buffer instance down to its current size (the
 * number of characters it currently holds).
 *
 * Params:
 *    buffer - The string buffer to operate on.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - String buffer is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_trimToSize(StringBuffer *buffer);

/**
 * Clears out all content currently within the string buffer.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 * Returns:
 *    None
 */
void stringbuffer_clear(StringBuffer *buffer);

/**
 * Returns the string buffer's current length, that is, the number of characters it currently
 * holds.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 * Returns:
 *    The string buffer's current length.
 */
unsigned long stringbuffer_length(StringBuffer *buffer);

/**
 * Returns the string buffer's current capacity, that is, the maximum number of characters
 * it can hold before reallocating is required.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 * Returns:
 *    The string buffer's current capacity.
 */
unsigned long stringbuffer_capacity(StringBuffer *buffer);

/**
 * Allocates a new string from the heap, copies the string buffer's current contents into
 * said string, and stores the results into '*result'. The string allocated is null-terminated.
 * It is the caller's responsibility to call `free()` on the string when it's no longer
 * needed. If the buffer has no content, no string will be allocated.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - String buffer is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_toString(StringBuffer *buffer, char **result);

/**
 * Destroys the string buffer instance by freeing all of its reserved memory.
 *
 * Params:
 *    buffer - The string buffer to destroy.
 * Returns:
 *    None
 */
void stringbuffer_destroy(*StringBuffer buffer);

#endif  /* _CDS_STRING_BUFFER_H__ */
