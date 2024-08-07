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

#ifndef _CDS_STRING_BUILDER_H__
#define _CDS_STRING_BUILDER_H__

#include "cds_common.h"

/**
 * Interface for the StringBuilder ADT.
 *
 * The StringBuilder is a mutable sequence of characters. The API provides a way for callers to
 * construct strings by appending and inserting string elements into the buffer. Other functions
 * such as string searching, subsequences, removals, reversing, and others are also provided.
 *
 * Modeled after the Java 11 StringBuilder interface.
 */
typedef struct string_builder StringBuilder;

/**
 * Constructs a new string builder instance with the specified starting capacity, then stores the
 * new instance into `*builder`. If `capacity` is <= 0, a default capacity is assigned.
 *
 * The growth factor is the percentage of growth to apply to the builder when it becomes full and
 * needs to be resized. The percentage is expressed as a float in the range (0.0, 1.0], so 0.01
 * would be 1% growth, 0.1 is 10%, 1.0 would be 100%, and so forth. For example, if the builder has
 * a capacity of 175 characters and becomes full, and the growth factor is set to 0.14 (14%), then
 * the builder capacity increases from 175 to 200:
 *    growth = ceiling(175 * 0.14)
 *           = ceiling(24.5)
 *           = 25
 * If the growth factor given is outside of the specified range, a default factor is assigned.
 *
 * Caller may also provide a string in which the builder will be initialized with its contents, or
 * NULL to create a builder instance with no content. If an initialization string is provided,
 * caller must ensure that `str` is null-terminated (if not, undefined behavior or errors may
 * occur). If the intial capacity specified is less than the length of `str`, the new default
 * capacity will become the length of `str`.
 *
 * Params:
 *    builder - The pointer address to store the new StringBuilder instance.
 *    capacity - The initial capacity of the string builder.
 *    growthFactor - The percentage to increase the capacity when resizing is needed.
 *    str - The initial contents to provide the builder, or NULL for an empty builder.
 * Returns:
 *    OK - StringBuilder was successfully created.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_new(StringBuilder **builder, long capacity, float growthFactor, char *str);

/**
 * Appends the character `ch` to the builder.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    ch - The character to append.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_appendChar(StringBuilder *builder, char ch);

/**
 * Appends the string `str` to the builder. If `str` is NULL, then the string "null" will be
 * appended. Caller must ensure that `str` is null-terminated (if not, undefined behavior or errors
 * may occur).
 *
 * Params:
 *    builder - The string builder to operate on.
 *    str - The string to append.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_appendStr(StringBuilder *builder, char *str);

/**
 * Appends the string representation of the boolean `b` to the builder.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    b - The boolean to append.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_appendBoolean(StringBuilder *builder, Boolean b);

/**
 * Appends the string representation of the short `s` to the builder.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    s - The short to append.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_appendShort(StringBuilder *builder, short s);

/**
 * Appends the string representation of the int `i` to the builder.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    i - The integer to append.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_appendInt(StringBuilder *builder, int i);

/**
 * Appends the string representation of the long `l` to the builder.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    l - The long to append.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_appendLong(StringBuilder *builder, long i);

/**
 * Appends the string representation of the float `f` to the builder.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    f - The float to append.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_appendFloat(StringBuilder *builder, float f);

/**
 * Appends the string representation of the double `d` to the builder.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    d - The double to append.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_appendDouble(StringBuilder *builder, double d);

/**
 * Appends a substring of `str` to the builder, given the starting and ending indecies. Characters
 * of the argument `str`, starting at index `start`, are appended, in order, to the contents of this
 * sequence up to the index `end`. The length of this sequence is increased by the value of
 * `end` - `start`.
 *
 * If `str` is NULL, then the string "null" will be appended as if the parameter `str` was "null".
 * Caller must ensure that `str` is null-terminated (if not, undefined behavior or errors may
 * occur).
 *
 * Params:
 *    builder - The string builder to operate on.
 *    str - The string to append.
 *    start - The starting index of `str` to append (inclusive).
 *    end - The ending index of `str` to append (exclusive).
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index specified is invalid:
 *       1.) `start` < 0
 *       2.) `start` > `end`
 *       3.) `end` > length
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_appendSubStr(StringBuilder *builder, char *str, int start, int end);

/**
 * Appends a subsequence of `str` to the builder. Characters of the char array `str`, starting at
 * index `offset`, are appended, in order, to the contents of this sequence. The length of this
 * sequence increases by the value of `len`.
 *
 * If `str` is NULL, then the string "null" will be appended as if the parameter `str` was "null".
 * Caller must ensure that `str` is null-terminated (if not, undefined behavior or errors may
 * occur).
 *
 * Params:
 *    builder - The string builder to operate on.
 *    str - The string to append.
 *    offset - Index of the first character in str to append.
 *    len - The number of characters to append.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index specified is invalid:
 *       1.) `offset` < 0
 *       2.) `len` < 0
 *       3.) `offset` + `len` > length
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_appendStrSubSequence(StringBuilder *builder, char *str, int offset, int len);

/**
 * Appends the specified StringBuilder `other` to the sequence `builder`.
 *
 * The characters of the StringBuilder `other` argument are appended, in order, to this sequence,
 * increasing the length of this sequence by the length of the argument.
 *
 * If `other` is null, then the string "null" will be appended. If `other` is not null, but has no
 * content, then nothing will be appended.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    other - The string builder to append.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_appendStrBuffer(StringBuilder *builder, StringBuilder *other);

/**
 * Inserts the string representation of the character `ch` into this sequence at the specified index
 * `offset`.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    offset - The offset where to perform the insert.
 *    ch - The character to insert.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index was invalid:
 *       1.) `offset` < 0
 *       2.) `offset` > length()
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_insertChar(StringBuilder *builder, long offset, char ch);

/**
 * Inserts the specified string `str` into this sequence at the specified index `offset`.
 *
 * If `str` is NULL, then the string "null" will be appended as if the parameter `str` was "null".
 * It is the caller's responsibility to ensure that `str` is null terminated. If not,
 * undefined/erroneous behavior can occur.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    offset - The offset where to perform the insert.
 *    str - The string to insert.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index was invalid:
 *       1.) `offset` < 0
 *       2.) `offset` > length()
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_insertStr(StringBuilder *builder, long offset, char *str);

/**
 * Inserts the string representation of the boolean `b` into this sequence at the specified index
 * `offset`.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    offset - The offset where to perform the insert.
 *    b - The boolean to insert.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index was invalid:
 *       1.) `offset` < 0
 *       2.) `offset` > length()
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_insertBoolean(StringBuilder *builder, long offset, Boolean b);

/**
 * Inserts the string representation of the short `s` into this sequence at the specified index
 * `offset`.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    offset - The offset where to perform the insert.
 *    s - The short to insert.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index was invalid:
 *       1.) `offset` < 0
 *       2.) `offset` > length()
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_insertShort(StringBuilder *builder, long offset, short s);

/**
 * Inserts the string representation of the int `i` into this sequence at the specified index
 * `offset`.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    offset - The offset where to perform the insert.
 *    i - The integer to insert.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index was invalid:
 *       1.) `offset` < 0
 *       2.) `offset` > length()
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_insertInt(StringBuilder *builder, long offset, int i);

/**
 * Inserts the string representation of the long `l` into this sequence at the specified index
 * `offset`.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    offset - The offset where to perform the insert.
 *    l - The long to insert.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index was invalid:
 *       1.) `offset` < 0
 *       2.) `offset` > length()
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_insertLong(StringBuilder *builder, long offset, long l);

/**
 * Inserts the string representation of the float `f` into this sequence at the specified index
 * `offset`.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    offset - The offset where to perform the insert.
 *    f - The float to insert.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index was invalid:
 *       1.) `offset` < 0
 *       2.) `offset` > length()
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_insertFloat(StringBuilder *builder, long offset, float f);

/**
 * Inserts the string representation of the double `d` into this sequence at the specified index
 * `offset`.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    offset - The offset where to perform the insert.
 *    d - The double to insert.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index was invalid:
 *       1.) `offset` < 0
 *       2.) `offset` > length()
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_insertDouble(StringBuilder *builder, long offset, double d);

/**
 * Inserts a substring of the specified string `str` into this sequence.
 *
 * The substring of `str` specified by `start` and `end` indecies are inserted, in order, into this
 * sequence at the specified destination offset, moving up any characters originally above that
 * position. The length of this sequence is increased by `end` - `start`, with reallocation taking
 * place as needed.
 *
 * The `start` argument must be nonnegative, and not greater than end. The `end` argument must be
 * greater than or equal to `start`, and less than or equal to the length of `str`.
 *
 * If `str` is NULL, then the string "null" will be appended as if the parameter `str` was "null".
 * It is the caller's responsibility to ensure that `str` is null terminated. If not,
 * undefined/erroneous behavior can occur.
 *
 * Params:
 *    builder - The string builder to operate on.
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
Status string_builder_insertSubStr(StringBuilder *builder, long offset, char *str, int start, int end);

/**
 * Inserts the string representation of a subarray of the `str` array argument into this sequence.
 * The subarray begins at the specified offset `offset` and extends `len` characters. The characters
 * of the subarray are inserted into this sequence at the position indicated by `index`. The length
 * of this sequence increases by `len` chars.
 *
 * If `str` is NULL, then the string "null" will be appended as if the parameter `str` was "null".
 * Caller must ensure that `str` is null-terminated (if not, undefined behavior or errors may
 * occur).
 *
 * Params:
 *    builder - The string builder to operate on.
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
Status string_builder_insertStrSubSequence(StringBuilder *builder, long index, char *str,
                                           int offset, int len);

/**
 * Inserts the string representation of the StringBuilder `other` argument into the sequence
 * `builder`. The content of `other` is inserted into this sequence at the position indicated by
 * `offset`.
 *
 * If `other` is null, then the string "null" will be appended. If `other` is not null, but has no
 * content, then nothing will be appended.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    offset - The index where to perform the insert.
 *    other - The other StringBuilder whose contents to insert.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index was invalid:
 *       1.) `offset` < 0
 *       2.) `offset` > length()
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_insertStrBuffer(StringBuilder *builder, long offset, StringBuilder *other);

/**
 * Replaces the characters in a substring of this sequence with characters in the specified string
 * `str`. The substring begins at the specified starting index `start` and extends to the character
 * at index `end` - 1, or to the end of the sequence if no such character exists. First the
 * characters in the substring are removed and then `str` is inserted at start. This sequence will
 * be lengthened to accommodate `str` if necessary.
 *
 * If `str` is null, then nothing will be inserted/replaced. It is the caller's responsibility to
 * ensure that `str` is null terminated. If not, undefined/erroneous behavior can occur.
 *
 * Params:
 *    builder - The string builder to operate on.
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
Status string_builder_replace(StringBuilder *builder, int start, int end, char *str);

/**
 * Fetches the char value in this builder at the specified index `i` and stores the result into
 * `*result`. The first char value is at index 0, the next at index 1, and so on, as in array
 * indexing.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    i - The desired index to fetch.
 *    result - The pointer address to store the char into.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index is invalid:
 *       1.) `i` < 0
 *       2.) `i` >= length()
 */
Status string_builder_charAt(StringBuilder *builder, long i, char *result);

/**
 * Allocates a string that is a substring of this builder, and stores the result into `*result`. The
 * substring negins at the specified index `start` and extends to the end of the current builder.
 * The returned string is an allocated string from the heap, and is the caller's responsibility to
 * free the string when done using (via stdlib's free() method).
 *
 * Params:
 *    builder - The string builder to operate on.
 *    start - The beginning index (inclusive).
 *    result - The pointer address to store the allocated string into.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index is invalid:
 *       1.) `start` < 0
 *       2.) `start` > length()
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_substring(StringBuilder *builder, long start, char **result);

/**
 * Allocates a string that is a subsequence of this builder, and stores the result into `*result`.
 * The returned string is an allocated string from the heap, and is the caller's responsibility to
 * free the string when done using (via stdlib's free() method).
 *
 * Params:
 *    builder - The string builder to operate on.
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
Status string_builder_subsequence(StringBuilder *builder, long start, long end, char **result);

/**
 * Characters are copied from this sequence into the destination character array `dst`. The first
 * character to be copied is at index `srcBegin`; the last character to be copied is at index
 * `srcEnd` - 1. The total number of characters to be copied is `srcEnd` - `srcBegin`. The
 * characters are copied into the subarray of `dst` starting at index `dstBegin` and ending at
 * index:
 *
 *    dstBegin + (srcEnd - srcBegin) - 1
 *
 * It is the caller's responsibility to ensure that the provided array `dst` is large enough to
 * store the copy results. If not, undefined/erroneous behavior can occur.
 *
 * Params:
 *    builder - The string builder to operate on.
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
Status string_builder_getChars(StringBuilder *builder, long srcBegin, long srcEnd, char dst[],
                               int dstBegin);

/**
 * The character at the specified index `index` is set to `ch`. All other characters in all other
 * indecies in the builder remain unaltered.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    index - The index in the builder to modify.
 *    ch - The new character.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index given in invalid:
 *       1.) `index` < 0
 *       2.) `index` > length()
 */
Status string_builder_setCharAt(StringBuilder *builder, long index, char ch);

/**
 * Sets the length of the character sequence. If `len` is less than this builder's character
 * sequence, then all the characters in the character sequence starting at index `len` are
 * truncated. If `len` is greater than this builder's character sequence, then a sequence of
 * `padding` characters are added at the end of the character sequence.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    len - The new length.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - New length given is invalid (len < 0).
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_setLength(StringBuilder *builder, long len, char padding);

/**
 * Returns the index within this string builder of the first occurrence of the specified substring
 * `str`. The first occurrence of the empty string "" is considered to occur at the first index (0).
 * If no such value exists, or `str` is NULL, then -1 is returned.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    str - The substring to search for.
 * Returns:
 *    The index of the last occurrence of the substring, or -1 if no such index exists.
 */
long string_builder_indexOf(StringBuilder *builder, char *str);

/**
 * Returns the index within this string builder of the first occurrence of the specified substring
 * `str`, starting at the specified index `fromIndex`. If no such value exists, or `str` is NULL,
 * then -1 is returned.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    str - The substring to search for.
 *    fromIndex - The index to start the search from.
 * Returns:
 *    The index of the first occurrence of the substring, searching from `fromIndex`,
 *    or -1 if no such index exists.
 */
long string_builder_indexOfFrom(StringBuilder *builder, char *str, long fromIndex);

/**
 * Returns the index within this string builder of the last occurrence of the specified substring
 * `str`. The last occurrence of the empty string "" is considered to occur at the last index (the
 * current length of the builder). If no such value exists, or `str` is NULL, then -1 is returned.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    str - The substring to search for.
 * Returns:
 *    The index of the last occurrence of the substring, or -1 if no such index exists.
 */
long string_builder_lastIndexOf(StringBuilder *builder, char *str);

/**
 * Returns the index within this string builder of the last occurrence of the specified substring
 * `str`, searching backward starting at the specified index. If no such value exists, or `str` is
 * NULL, then -1 is returned.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    str - The substring to search for.
 *    fromIndex - The index to start the search from.
 * Returns:
 *    The index of the last occurrence of the substring, searching backwards from
 *    `fromIndex`, or -1 if no such index exists.
 */
long string_builder_lastIndexOfFrom(StringBuilder *builder, char *str, long fromIndex);

/**
 * Compares the two string builder instances lexicographically.
 *
 * Params:
 *    builder - The first string builder to compare.
 *    other - The string builder to comapre with.
 * Returns:
 *    0 if both `builder` and `other` contain the same exact string sequence as each other, <0 if
 *    `builder` is lexographically less than `other`, or >0 if `builder` is lexographically greater
 *    than `other`.
 */
int string_builder_compareTo(StringBuilder *builder, StringBuilder *other);

/**
 * Removes the characters in a substring of this sequence from the builder. The substring begins at
 * the specified start and extends to the character at index `end` - 1 or to the end of the sequence
 * if no such character exists. If `start` is equal to `end`, no changes are made.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    start - The beginning index (inclusive).
 *    end - The ending index (exclusive).
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - String builder is currently empty.
 *    INVALID_INDEX - Index given is out of range:
 *       1.) `start` < 0
 *       2.) `start` >= length()
 *       3.) `start` > `end`
 */
Status string_builder_delete(StringBuilder *builder, long start, long end);

/**
 * Removes the character at the specified position in the string builder.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    i - The index of the character to be removed.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - String builder is currently empty.
 *    INVALID_INDEX - Index given is out of range:
 *       1.) `index` < 0
 *       2.) `index` >= length()
 */
Status string_builder_deleteCharAt(StringBuilder *builder, long index);

/**
 * Reverses the contents of the string builder in place.
 *
 * Params:
 *    builder - The string builder to operate on.
 * Returns:
 *    None
 */
void string_builder_reverse(StringBuilder *builder);

/**
 * Increases the capacity of the string builder, if necessary, to ensure that it can hold at most
 * the number of characters specified by the capacity argument before reallocation will occur.
 *
 * Params:
 *    builder - The string builder to operate on.
 *    capacity - The desired maximum capacity.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_ensureCapacity(StringBuilder *builder, long capacity);

/**
 * Trims the capacity of the string builder instance down to its current size (the number of
 * characters it currently holds).
 *
 * Params:
 *    builder - The string builder to operate on.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - String builder is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_trimToSize(StringBuilder *builder);

/**
 * Clears out all content currently within the string builder.
 *
 * Params:
 *    builder - The string builder to operate on.
 * Returns:
 *    None
 */
void string_builder_clear(StringBuilder *builder);

/**
 * Returns the string builder's current length, that is, the number of characters it currently
 * holds.
 *
 * Params:
 *    builder - The string builder to operate on.
 * Returns:
 *    The string builder's current length.
 */
long string_builder_length(StringBuilder *builder);

/**
 * Returns the string builder's current capacity, that is, the maximum number of characters it can
 * hold before reallocating is required.
 *
 * Params:
 *    builder - The string builder to operate on.
 * Returns:
 *    The string builder's current capacity.
 */
long string_builder_capacity(StringBuilder *builder);

/**
 * Allocates a new string from the heap, copies the string builder's current contents into said
 * string, and stores the results into '*result'. The string allocated is null-terminated. It is the
 * caller's responsibility to call `free()` on the string when it's no longer needed. If the builder
 * has no content, no string will be allocated.
 *
 * Params:
 *    builder - The string builder to operate on.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - String builder is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status string_builder_toString(StringBuilder *builder, char **result);

/**
 * Destroys the string builder instance by freeing all of its reserved memory.
 *
 * Params:
 *    builder - The string builder to destroy.
 * Returns:
 *    None
 */
void string_builder_destroy(StringBuilder *builder);

#endif  /* _CDS_STRING_BUILDER_H__ */
