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
#include <stdlib.h>
#include <limits.h>
#include "string_builder.h"

struct string_builder {
    char *str;              // The inner string builder
    long index;             // Index of the next character to add, same as current size
    long capacity;          // The builder's current capacity (including the null terminator)
    float growthFactor;     // The growth factor to apply when expanding builder capacity
};

// The default capacity to assign when the capacity give is invalid
#define DEFAULT_CAPACITY 16L
// The maximum capacity allowed (to avoid builder overflow)
// Must allow one extra byte for null terminator
#define MAX_CAPACITY ( LONG_MAX - 1L )
// The default growth factor to apply when user input is invalid or 0
#define DEFAULT_GROWTH_FACTOR 0.12f

// Macro to validate that index `idx` is valid, given the max index `max`
#define VALIDATE_INDEX(idx, max) if (idx < 0 || idx > max) { return INVALID_INDEX; }

// Macro to validate that the index range `start` and `end` are valid, given the max index `max`
#define VALIDATE_INDEX_RANGE(start, end, max) \
            if (start < 0 || end < 0 || start > end || start > max || end > max) \
            { return INVALID_INDEX; }

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

    Boolean expanding = newCapacity > builder->capacity ? TRUE : FALSE;
    size_t bytes = ( ( newCapacity + 1 ) * sizeof(char) );
    char *temp;

    // Attempts to reallocate the builder, returning false if fails
    if ((temp = (char *)realloc(builder->str, bytes)) == NULL) {
        return FALSE;
    }

    // Update attributes after extension
    builder->str = temp;
    if (expanding == TRUE) {
        long i;
        for (i = builder->capacity; i < newCapacity + 1; i++) {
            // Need to add null terminators in all new bytes
            builder->str[i] = '\0';
        }
    } else {
        builder->str[newCapacity] = '\0';
        builder->index = newCapacity;
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
 * Local method to insert the specified string `str` into the builder, starting at index `offset` in
 * the builder.
 */
static Status _insert_str(StringBuilder *builder, long offset, char *str) {

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
    temp->index = 0L;
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

/**
 * Helper method to load the specified buffer `dest` with a substring of `src`, index-based,
 * starting from `dstBegin` (inclusive) to `dstEnd` (exclusive). The char array `dest` is assumed
 * to be large enough to store the results.
 */
static void _load_substring_to_buffer(char *src, char dest[], long dstBegin, long dstEnd) {

    // For null strings, will use the string literal "null"
    if (src == NULL) {
        src = "null";
    }

    // Copies substring characters into buffer
    long i, j, subLen = ( dstEnd - dstBegin );
    for (i = 0, j = dstBegin; i < subLen; i++, j++) {
        dest[i] = src[j];
    }
    dest[i] = '\0';
}

Status string_builder_appendSubStr(StringBuilder *builder, char *str, int start, int end) {
// #define VALIDATE_INDEX_RANGE(start, end, max) \ 0, 33, 13
            // if (0 < 0 || 0 > 33 || 0 > 13 || 33 < 13) { return INVALID_INDEX; }

    int strLen = _get_str_length(str);
    VALIDATE_INDEX_RANGE(start, end, strLen);

    char temp[(end - start) + 1];
    _load_substring_to_buffer(str, temp, start, end);
    return _insert_str(builder, builder->index, temp);
}

Status string_builder_appendStrSubSequence(StringBuilder *builder, char *str, int offset, int len) {

    int strLen = _get_str_length(str);
    VALIDATE_INDEX_RANGE(offset, offset + len, strLen);

    char temp[len + 1];
    _load_substring_to_buffer(str, temp, offset, offset + len);
    return _insert_str(builder, builder->index, temp);
}

Status string_builder_appendStrBuilder(StringBuilder *builder, StringBuilder *other) {

    if (other == NULL) {
        // For null object, will use string literal "null"
        return _insert_str(builder, builder->index, "null");
    } else if (other->index == 0) {
        // If other builder is empty, then do nothing
        return OK;
    } else if (builder == other) {
        /*
         * If both instances are equal, need to copy builder's contents to array and add that to the
         * builder. Step is needed as the loop inside the insert string helper method will loop
         * infinitely.
         */
        char buffer[other->index];
        _load_substring_to_buffer(other->str, buffer, 0, other->index);
        return _insert_str(builder, builder->index, buffer);
    } else {
        // If both instances are different and not empty, append as normal
        return _insert_str(builder, builder->index, other->str);
    }
}

Status string_builder_insertChar(StringBuilder *builder, long offset, char ch) {

    VALIDATE_INDEX(offset, builder->index);
    char str[] = { ch, '\0' };
    return _insert_str(builder, offset, str);
}

Status string_builder_insertStr(StringBuilder *builder, long offset, char *str) {

    VALIDATE_INDEX(offset, builder->index);
    char *temp = ( str != NULL ) ? str : "null";
    return _insert_str(builder, offset, temp);
}

Status string_builder_insertBoolean(StringBuilder *builder, long offset, Boolean b) {

    VALIDATE_INDEX(offset, builder->index);
    char *str = ( b == TRUE ) ? "true" : "false";
    return _insert_str(builder, offset, str);
}

Status string_builder_insertShort(StringBuilder *builder, long offset, short s) {

    VALIDATE_INDEX(offset, builder->index);
    char temp[BUFFER_SIZE];
    sprintf(temp, "%hd", s);
    return _insert_str(builder, offset, temp);
}

Status string_builder_insertInt(StringBuilder *builder, long offset, int i) {

    VALIDATE_INDEX(offset, builder->index);
    char temp[BUFFER_SIZE];
    sprintf(temp, "%d", i);
    return _insert_str(builder, offset, temp);
}

Status string_builder_insertLong(StringBuilder *builder, long offset, long l) {

    VALIDATE_INDEX(offset, builder->index);
    char temp[BUFFER_SIZE];
    sprintf(temp, "%ld", l);
    return _insert_str(builder, offset, temp);
}

Status string_builder_insertFloat(StringBuilder *builder, long offset, float f) {

    VALIDATE_INDEX(offset, builder->index);
    char temp[BUFFER_SIZE];
    sprintf(temp, "%f", f);
    return _insert_str(builder, offset, temp);
}

Status string_builder_insertDouble(StringBuilder *builder, long offset, double d) {

    VALIDATE_INDEX(offset, builder->index);
    char temp[BUFFER_SIZE];
    sprintf(temp, "%lf", d);
    return _insert_str(builder, offset, temp);
}

Status string_builder_insertSubStr(StringBuilder *builder, long offset, char *str, int start,
                                   int end) {

    VALIDATE_INDEX(offset, builder->index);
    int strLen = _get_str_length(str);
    VALIDATE_INDEX_RANGE(start, end, strLen);

    char temp[(end - start) + 1];
    _load_substring_to_buffer(str, temp, start, end);
    return _insert_str(builder, offset, temp);
}

Status string_builder_insertStrSubSequence(StringBuilder *builder, long index, char *str,
                                           int offset, int len) {

    VALIDATE_INDEX(index, builder->index);
    int strLen = _get_str_length(str);
    VALIDATE_INDEX_RANGE(offset, offset + len, strLen);

    char temp[len + 1];
    _load_substring_to_buffer(str, temp, offset, offset + len);
    return _insert_str(builder, index, temp);
}

Status string_builder_insertStrBuilder(StringBuilder *builder, long offset, StringBuilder *other) {

    VALIDATE_INDEX(offset, builder->index);

    if (other == NULL) {
        // For null object, will use string literal "null"
        return _insert_str(builder, offset, "null");
    } else if (other->index == 0) {
        // If other builder is empty, then do nothing
        return OK;
    } else if (builder == other) {
        /*
         * If both instances are equal, need to copy builder's contents to array and add that to the
         * builder. Step is needed as the loop inside the insert string helper method will loop
         * infinitely.
         */
        char buffer[other->index];
        _load_substring_to_buffer(other->str, buffer, 0, other->index);
        return _insert_str(builder, offset, buffer);
    } else {
        // If both instances are different and not empty, append as normal
        return _insert_str(builder, offset, other->str);
    }
}

/**
 * Helper method to nullify all characters in array `buffer` from index `start` (inclusive) to
 * `end` (exlcusive). By nullify, the characters are set to null terminators.
 */
static void _scrub_char_builder(char *buffer, long start, long end) {

    long i;
    for (i = start; i < end; i++) {
        buffer[i] = '\0';
    }
}

/**
 * Helper method to delete a substring from the string builder `builder`, starting from index
 * `start` (inclusive) to `end` (exclusive). All characters are shifted to fill in the delted
 * characters as needed.
 */
static Status _delete_substring(StringBuilder *builder, long start, long end) {

    // Dont do anything if currently empty
    if (builder->index == 0L) {
        return STRUCT_EMPTY;
    }

    long i;
    long delta = end - start;
    for (i = end; i < builder->index; i++) {
        // Shifts all characters in buffer over to left
        builder->str[i - delta] = builder->str[i];
    }

    _scrub_char_builder(builder->str, builder->index - delta, builder->index);
    builder->index -= delta;

    return OK;
}

Status string_builder_replace(StringBuilder *builder, long start, long end, char *str) {

    if (end > builder->index) {
        end = builder->index;
    }
    VALIDATE_INDEX_RANGE(start, end, builder->index);

    int subLen = 0;
    long diff = end - start;
    int strLen = _get_str_length(str);

    /*
     * If string's length is greater than the index range provided, then this means that part of the
     * string will need to be appended. Only append the first N characters of the string, N being
     * the length - index range, and the rest can simply replace characters in the buffer.
     */
    if (strLen > diff) {
        subLen = strLen - diff;
        char subStr[subLen + 1];
        _load_substring_to_buffer(str, subStr, 0, subLen);
        if (_insert_str(builder, start, subStr) != OK) {
            return ALLOC_FAILURE;
        }
    }

    // Replace the remaining characters in the specified range with the given string
    long i, j;
    for (i = ( start + subLen ), j = subLen; j < strLen; i++, j++) {
        builder->str[i] = str[j];
    }

    return OK;
}

Status string_builder_charAt(StringBuilder *builder, long i, char *result) {

    VALIDATE_INDEX(i, builder->index - 1);
    *result = builder->str[i];
    return OK;
}

/**
 * Helper method to extract a substring from the builder `builder`, index-based, starting from index
 * `start` (inclusive) and ending at `end` (exclusive). Substring is loaded into `dst` from the
 * starting index `dstBegin`. Additonal parameter `allocate` is given to indicate whether `dstBegin`
 * will be allocated from the heap or not. If set to TRUE, it will, otherwise `dst` is assumed to be
 * a char array big enough to hold the result.
 *
 * Returns OK if successful, INVALID_INDEX if one of the indecies given is invalid, or ALLOC_FAILURE
 * if any allocation fails.
 */
static Status _get_substring(StringBuilder *builder, long start, long end, char *dst, int dstBegin,
                             Boolean allocate) {

    if (start < 0 || end < 0 || end > builder->index || start > end || dstBegin < 0) {
        return INVALID_INDEX;
    }

    char *temp;
    int strLen = ( end - start ) + 1;
    if (allocate == TRUE) {
        // If allocation set to true, will allocate char array from heap
        int i;
        if ((temp = (char *)malloc(strLen)) == NULL) {
            return ALLOC_FAILURE;
        }
        for (i = 0; i < strLen; i++) {
            // Need to nullify all characters in string
            temp[i] = '\0';
        }
        dst = temp;
    }

    long i;
    int j;
    for (i = start, j = dstBegin; i < end; i++, j++) {
        // Copies substring into the destination array
        dst[j] = builder->str[i];
    }

    return OK;
}

Status string_builder_substring(StringBuilder *builder, long start, char **result) {

    VALIDATE_INDEX(start, builder->index);
    return _get_substring(builder, start, builder->index, *result, 0, TRUE);
}

Status string_builder_subsequence(StringBuilder *builder, long start, long end, char **result) {

    VALIDATE_INDEX_RANGE(start, end, builder->index);
    return _get_substring(builder, start, end, *result, 0, TRUE);
}

Status string_builder_getChars(StringBuilder *builder, long srcBegin, long srcEnd, char dst[],
                               int dstBegin) {

    if (dstBegin < 0) {
        return INVALID_INDEX;
    }
    VALIDATE_INDEX_RANGE(srcBegin, srcEnd, builder->index);
    return _get_substring(builder, srcBegin, srcEnd, dst, dstBegin, FALSE);
}

Status string_builder_setCharAt(StringBuilder *builder, long index, char ch) {

    VALIDATE_INDEX(index, builder->index - 1);
    builder->str[index] = ch;
    return OK;
}

Status string_builder_setLength(StringBuilder *builder, long len, char padding) {

    if (len < 0) {
        // Validation check - invalid index given
        return INVALID_INDEX;
    } else if (len == builder->index) {
        // If length given equals current length, don't need to do anything
        return OK;
    }

    Boolean shrink = ( len < builder->index ) ? TRUE : FALSE;
    if (_ensure_capacity(builder, len) == FALSE) {
        return ALLOC_FAILURE;
    }

    if (shrink) {
        // Need to append null terminator at end if shrinking
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

/**
 * Helper method to perform a forward string comparison between `src` and `subStr`. Returns TRUE if
 * the strings match, FALSE if not. In addition, the number of characters that are compared between
 * the two strings before result is calculated is also stored in `compared`.
 */
static Boolean _compare_str_forward(char *str, char *subStr, int subLen, int *compared) {

    int i;
    char *c1, *c2;

    /*
     * Loops through both strings, comparing each character until one of the following conditions
     * are met:
     *   1.) The characters in both strings don't match
     *   2.) We reach the end of the first string
     *   3.) We reach the end of the substring
     */
    for (i = 0, c1 = str, c2 = subStr; *c1 != '\0' && *c2 != '\0' && *c1 == *c2; c1++, c2++, i++);
    *compared = i;

    // String is a match if number of characters compared equals the substring's length
    return i == subLen ? TRUE : FALSE;
}

/**
 * Helper method to search the string builder `builder` for the first occurence of the substring
 * `sub`, starting from index `start`. Returns the starting index in builder where first occurence
 * is, from index `start`, or -1 if not found. -1 will also be returned if `start` is invalid, or
 * `sub` is NULL.
 */
static long _search_first_occurrence(StringBuilder *builder, char *sub, long start) {

    // Validation checks, return -1 if does not pass
    if (sub == NULL || start >= builder->index) {
        return -1L;
    }

    long i;
    int compared;
    int subLen = _get_str_length(sub);
    if (subLen == 0) {
        // Base case - substring is empty, so return the starting index
        return start;
    }
    if (start < 0) {
        start = 0;
    }

    // Loop through the string, starting from first character
    for (i = start; i < builder->index; i++) {
        // Characters match, starts a search in the string
        if (builder->str[i] == sub[0]) {
            Boolean matched = _compare_str_forward(builder->str + i, sub, subLen, &compared);
            if (matched == TRUE) {
                // Return starting index of substring if match is found
                return i;
            } else {
                // If not a match, can skip over these searched characters to save some work
                i += compared;
            }
        }
    }

    return -1L;
}

long string_builder_indexOf(StringBuilder *builder, char *str) {

    return _search_first_occurrence(builder, str, 0L);
}

long string_builder_indexOfFrom(StringBuilder *builder, char *str, long fromIndex) {

    return _search_first_occurrence(builder, str, fromIndex);
}

/**
 * Helper method to perform a backwards string comparison between `src` and `subStr`. To better
 * assist with backwards comparison, caller should provide `strIdx` being the last index of `str`,
 * and `subLen` being the length of `subStr`. Returns TRUE if the strings match, FALSE if not. In
 * addition, the number of characters that are compared between the two strings before result is
 * calculated is also stored in `compared`.
 */
static Boolean _compare_str_backward(char *str, long strIdx, char *subStr, int subLen,
                                     int *compared) {

    int i;
    int subIdx = subLen - 1;

    /*
     * Loops through both strings backwards, comparing each character until one of the following
     * conditions are met:
     *   1.) The characters in both strings don't match
     *   2.) We reach the start of the first string
     *   3.) We reach the start of the substring
     */
    for (i = 0; ( str[strIdx] == subStr[subIdx] ) && ( strIdx >= 0 ) && ( subIdx >= 0 );
        i++, strIdx--, subIdx--);
    *compared = i;

    // String is a match if number of characters compared equals the substring's length
    return i == subLen ? TRUE : FALSE;
}

/**
 * Helper method to search the string builder `builder` for the last occurence of the substring
 * `sub`, starting from index `start`. Returns the starting index in builder where last occurence
 * is, from index `start`, or -1 if not found. -1 will also be returned if `start` is invalid, or
 * `sub` is NULL.
 */
static long _search_last_occurrence(StringBuilder *builder, char *sub, long start) {

    // Validation checks, return -1 if does not pass
    if (sub == NULL || start < 0) {
        return -1L;
    }

    long i;
    int compared;
    int subLen = _get_str_length(sub);
    if (subLen == 0) {
        // Base case - substring is empty, so return the starting index
        return start;
    }

    if (start >= builder->index) {
        start = builder->index - 1;
    }

    // Loop through the string, starting from last character
    for (i = start; i >= 0; i--) {
        // Characters match, starts a search in the string
        if (builder->str[i] == sub[subLen - 1]) {
            Boolean matched = _compare_str_backward(builder->str, i, sub, subLen, &compared);
            if (matched == TRUE) {
                // Return starting index of substring if match is found
                return ( i - subLen + 1 );
            } else {
                // If not a match, can skip over these searched characters to save some work
                i -= compared;
            }
        }
    }

    return -1L;
}

long string_builder_lastIndexOf(StringBuilder *builder, char *str) {

    return _search_last_occurrence(builder, str, builder->index - 1);
}

long string_builder_lastIndexOfFrom(StringBuilder *builder, char *str, long fromIndex) {

    return _search_last_occurrence(builder, str, fromIndex);
}

int string_builder_compareTo(StringBuilder *builder, StringBuilder *other) {

    // If both builders are same object, they are equal by definition
    if (builder == other) {
        return 0;
    }

    /*
     * Proceed through both strings comparing characters at same index in both until either:
     *   1.) characters aren't equal
     *   2.) we reach end of `builder`
     *   3.) we reach end of `other`
     * Then take difference of the next characters to get lexographical difference.
     */
    char *a, *b;
    for (a = builder->str, b = other->str; ( *a != '\0' ) && ( *b != '\0' ) && ( *a == *b );
         a++, b++);

    return *a - *b;
}

Status string_builder_delete(StringBuilder *builder, long start, long end) {

    VALIDATE_INDEX_RANGE(start, end, builder->index - 1);
    return _delete_substring(builder, start, end);
}

Status string_builder_deleteCharAt(StringBuilder *builder, long index) {

    VALIDATE_INDEX(index, builder->index - 1);
    return _delete_substring(builder, index, index + 1);
}

void string_builder_reverse(StringBuilder *builder) {

    char temp;
    long head, tail;

    /*
     * Performs a loop through the builder, one character pointing at the start and another at the
     * end. Each iteration simply swaps the two characters then the first char proceeds to next
     * index and the last char to the previous index. Loops until the two characters intersect or
     * pass each each other.
     */
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

    // Only ensure capacity if new capacity is greater than the current capacity
    if (builder->capacity < capacity) {
        if (_ensure_capacity(builder, capacity) == FALSE) {
            return ALLOC_FAILURE;
        }
    }

    return OK;
}

Status string_builder_trimToSize(StringBuilder *builder) {

    // If builder is empty, don't do anything
    if (builder->index == 0L) {
        return STRUCT_EMPTY;
    }

    // Only trim size down if there's still capacity leftover
    if (builder->index != builder->capacity) {
        if (_ensure_capacity(builder, builder->index) == FALSE) {
            return ALLOC_FAILURE;
        }
    }

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

    // Allocate memory for the string, copying over the builder if successful
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
