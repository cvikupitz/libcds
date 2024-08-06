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
 * stores the new instance into `*buffer`. If `capacity` is <= 0, a default capacity
 * is assigned.
 *
 * Caller may also provide a string in which the buffer will be initialized with its
 * contents, or NULL to create a buffer instance with no content. If an initialization
 * string is provided, Caller must ensure that `str` is null-terminated (if not, undefined
 * behavior or errors may occur). If the intial capacity specified is less than the
 * length of `str`, the new default capacity will become the length of `str`.
 *
 * Params:
 *    buffer - The pointer address to store the new StringBuffer instance.
 *    capacity - The default capacity of the string buffer.
 *    str - The initial contents to provide the buffer, or NULL for an empty buffer.
 * Returns:
 *    OK - StringBuffer was successfully created.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_new(StringBuffer **buffer, long capacity, char *str);

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
 * Appends the string `str` to the buffer. If `str` is NULL, then
 * nothing will be appended. Caller must ensure that `str` is
 * null-terminated (if not, undefined behavior or errors may occur).
 *l
 * Params:
 *    buffer - The string buffer to operate on.
 *    str - The string to append.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_appendStr(StringBuffer *buffer, char *str);

/**
 * Appends the string representation of the boolean `b` to the buffer.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    b - The boolean to append.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_appendBoolean(StringBuffer *buffer, Boolean b);

/**
 * Appends the string representation of the short `s` to the buffer.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    s - The short to append.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_appendShort(StringBuffer *buffer, short s);

/**
 * Appends the string representation of the int `i` to the buffer.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    i - The integer to append.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_appendInt(StringBuffer *buffer, int i);

/**
 * Appends the string representation of the long `l` to the buffer.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    l - The long to append.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_appendLong(StringBuffer *buffer, long i);

/**
 * Appends the string representation of the float `f` to the buffer.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    f - The float to append.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_appendFloat(StringBuffer *buffer, float f);

/**
 * Appends the string representation of the double `d` to the buffer.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    d - The double to append.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_appendDouble(StringBuffer *buffer, double d);

/**
 * Appends a substring of `str` to the buffer, given the starting and ending
 * indecies. Characters of the argument `str`, starting at index `start`, are
 * appended, in order, to the contents of this sequence up to the index `end`.
 * The length of this sequence is increased by the value of `end` - `start`.
 *
 * If `str` is NULL, then nothing will be appended. Caller must ensure that
 * `str` is null-terminated (if not, undefined behavior or errors may occur).
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
Status stringbuffer_appendSubStr(StringBuffer *buffer, char *str, int start, int end);

/**
 * Appends a subsequence of `str` to the buffer. Characters of the char array `str`,
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
Status stringbuffer_appendStrSubSequence(StringBuffer *buffer, char *str, int offset, int len);

/**
 * Appends the specified StringBuffer `other` to the sequence `buffer`.
 *
 * The characters of the StringBuffer `other` argument are appended, in order,
 * to this sequence, increasing the length of this sequence by the length of
 * the argument.
 *
 * If `other` is null or has no content, then nothing will be appended.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    other - The string buffer to append.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_appendStrBuffer(StringBuffer *buffer, StringBuffer *other);

/**
 * Inserts the string representation of the character `ch` into this sequence at the
 * specified index `offset`.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    offset - The offset where to perform the insert.
 *    ch - The character to insert.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index was invalid; `offset` < 0 or `offset` > length().
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_insertChar(StringBuffer *buffer, long offset, char ch);

/**
 * Inserts the specified string `str` into this sequence at the specified index `offset`.
 * If `str` is null, then nothing will be inserted. It is the caller's responsibility to
 * ensure that `str` is null terminated. If not, undefined/erroneous behavior can occur.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    offset - The offset where to perform the insert.
 *    str - The string to insert.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index was invalid; `offset` < 0 or `offset` > length().
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_insertStr(StringBuffer *buffer, long offset, char *str);

/**
 * Inserts the string representation of the boolean `b` into this sequence at the
 * specified index `offset`.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    offset - The offset where to perform the insert.
 *    b - The boolean to insert.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index was invalid; `offset` < 0 or `offset` > length().
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_insertBoolean(StringBuffer *buffer, long offset, Boolean b);

/**
 * Inserts the string representation of the short `s` into this sequence at the
 * specified index `offset`.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    offset - The offset where to perform the insert.
 *    s - The short to insert.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index was invalid; `offset` < 0 or `offset` > length().
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_insertShort(StringBuffer *buffer, long offset, short s);

/**
 * Inserts the string representation of the int `i` into this sequence at the
 * specified index `offset`.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    offset - The offset where to perform the insert.
 *    i - The integer to insert.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index was invalid; `offset` < 0 or `offset` > length().
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_insertInt(StringBuffer *buffer, long offset, int i);

/**
 * Inserts the string representation of the long `l` into this sequence at the
 * specified index `offset`.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    offset - The offset where to perform the insert.
 *    l - The long to insert.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index was invalid; `offset` < 0 or `offset` > length().
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_insertLong(StringBuffer *buffer, long offset, long l);

/**
 * Inserts the string representation of the float `f` into this sequence at the
 * specified index `offset`.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    offset - The offset where to perform the insert.
 *    f - The float to insert.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index was invalid; `offset` < 0 or `offset` > length().
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_insertFloat(StringBuffer *buffer, long offset, float f);

/**
 * Inserts the string representation of the double `d` into this sequence at the
 * specified index `offset`.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    offset - The offset where to perform the insert.
 *    d - The double to insert.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index was invalid; `offset` < 0 or `offset` > length().
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_insertDouble(StringBuffer *buffer, long offset, double d);

/**
 * Inserts a substring of the specified string `str` into this sequence.
 *
 * The substring of `str` specified by `start` and `end` indecies are inserted, in
 * order, into this sequence at the specified destination offset, moving up any
 * characters originally above that position. The length of this sequence is
 * increased by `end` - `start`, with reallocation taking place as needed.
 *
 * The `start` argument must be nonnegative, and not greater than end. The `end`
 * argument must be greater than or equal to `start`, and less than or equal to the
 * length of `str`.
 *
 * If s is null, then nothing will be inserted. It is the caller's responsibility
 * to ensure that `str` is null terminated. If not, undefined/erroneous behavior
 * can occur.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    offset - The offset where to perform the insert.
 *    str - The string to insert.
 *    start - The starting index of `str` to insert (inclusive).
 *    end - The ending index of `str` to insert (exclusive).
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - One of the index parameters was invalid:
 *       1.) `offset` < 0 or `offset` > length()
 *       2.) `start` < 0 or `start` > `end`
 *       3.) `end` < 0 or `end` > length()
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_insertSubStr(StringBuffer *buffer, long offset, char *str, int start, int end);

/**
 * Inserts the string representation of a subarray of the `str` array argument into
 * this sequence. The subarray begins at the specified offset `offset` and extends
 * `len` characters. The characters of the subarray are inserted into this sequence
 * at the position indicated by `index`. The length of this sequence increases by
 * `len` chars.
 *
 * If `str` is NULL, then nothing will be inserted. Caller must ensure that `str`
 * is null-terminated (if not, undefined behavior or errors may occur).
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    index - The index where to perform the insert.
 *    str - The string to insert.
 *    start - The first index in `str` to insert (inclusive).
 *    len - The number of chars in `str` to insert.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - One of the index parameters was invalid:
 *       1.) `index` < 0 or `index` > length()
 *       2.) `offset` < 0 or `offset` > length()
 *       3.) `len` < 0
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_insertStrSubSequence(StringBuffer *buffer, long index, char *str, int offset, int len);

/**
 * Inserts the string representation of the StringBuffer `other` argument into the
 * sequence `buffer`. The content of `other` is inserted into this sequence at the
 * position indicated by `offset`. If `other` is NULL or has no content, then nothing
 * will be inserted.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    offset - The index where to perform the insert.
 *    other - The other StringBuffer whose contents to insert.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index was invalid; `offset` < 0 or `offset` > length()
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_insertStrBuffer(StringBuffer *buffer, long offset, StringBuffer *other);

/**
 * Replaces the characters in a substring of this sequence with characters in the
 * specified string `str`. The substring begins at the specified starting index
 * `start` and extends to the character at index `end` - 1, or to the end of the
 * sequence if no such character exists. First the characters in the substring
 * are removed and then `str` is inserted at start. This sequence will be
 * lengthened to accommodate `str` if necessary.
 *
 * If `str` is null, then nothing will be inserted. It is the caller's
 * responsibility to ensure that `str` is null terminated. If not, undefined/erroneous
 * behavior can occur.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    start - The starting index (inclusive).
 *    end - The ending index (exclusive).
 *    str - The string to replace the contents with.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index is invalid:
 *       1.) `start` < 0
 *       2.) `start` > `end`
 *       2.) `start` > length()
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_replace(StringBuffer *buffer, int start, int end, char *str);

/**
 * Fetches the char value in this buffer at the specified index `i` and stores the
 * result into `*result`. The first char value is at index 0, the next at index 1,
 * and so on, as in array indexing.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    i - The desired index to fetch.
 *    result - The pointer address to store the char into.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index is invalid; `i` < 0 or `i` >= length().
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_charAt(StringBuffer *buffer, long i, char *result);

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
Status stringbuffer_substring(StringBuffer *buffer, long start, char **result);

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
Status stringbuffer_subsequence(StringBuffer *buffer, long start, long end, char **result);

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
Status stringbuffer_getChars(StringBuffer *buffer, long srcBegin, long srcEnd, char dst[], int dstBegin);

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
 *    INVALID_INDEX - Index given in invalid; `index` < 0 or  `index` > length().
 */
Status stringbuffer_setCharAt(StringBuffer *buffer, long index, char ch);

/**
 * Sets the length of the character sequence. If `len` is less than this buffer's
 * character sequence, then all the characters in the character sequence starting
 * at index `len` are truncated. If `len` is greater than this buffer's character
 * sequence, then a sequence of `padding` characters are added at the end of the
 * character sequence.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    len - The new length.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - New length given is invalid (len < 0).
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status stringbuffer_setLength(StringBuffer *buffer, long len, char padding);

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
long stringbuffer_indexOf(StringBuffer *buffer, char *str);

/**
 * Returns the index within this string buffer of the first occurrence of the specified
 * substring `str`, starting at the specified index `fromIndex`. If no such value exists,
 * or `str` is NULL, then -1 is returned.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 *    str - The substring to search for.
 *    fromIndex - The index to start the search from.
 * Returns:
 *    The index of the first occurrence of the substring, searching from `fromIndex`,
 *    or -1 if no such index exists.
 */
long stringbuffer_indexOfFrom(StringBuffer *buffer, char *str, long fromIndex);
l
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
long stringbuffer_lastIndexOf(StringBuffer *buffer, char *str);

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
long stringbuffer_lastIndexOfFrom(StringBuffer *buffer, char *str, long fromIndex);

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
Status stringbuffer_delete(StringBuffer *buffer, long start, long end);

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
Status stringbuffer_deleteCharAt(StringBuffer *buffer, long index);

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
Status stringbuffer_ensureCapacity(StringBuffer *buffer, long capacity);

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
long stringbuffer_length(StringBuffer *buffer);

/**
 * Returns the string buffer's current capacity, that is, the maximum number of characters
 * it can hold before reallocating is required.
 *
 * Params:
 *    buffer - The string buffer to operate on.
 * Returns:
 *    The string buffer's current capacity.
 */
long stringbuffer_capacity(StringBuffer *buffer);

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
void stringbuffer_destroy(StringBuffer *buffer);

#endif  /* _CDS_STRING_BUFFER_H__ */
