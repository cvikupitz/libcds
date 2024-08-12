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

#include <stdlib.h>
#include <CUnit/Basic.h>
#include "string_builder.h"

#define DEFAULT_CAPACITY 100L
#define DEFAULT_GROWTH_FACTOR 0.5f

static void _test_constructor1() {

    StringBuilder *builder;
    Status status;
    char *str;

    status = string_builder_new(&builder, DEFAULT_CAPACITY, DEFAULT_GROWTH_FACTOR, NULL);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( string_builder_length(builder), 0L );
    CU_ASSERT_EQUAL( string_builder_capacity(builder), DEFAULT_CAPACITY );
    status = string_builder_toString(builder, &str);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( strcmp(str, ""), 0 );
    free(str);
    string_builder_destroy(builder);
}

static void _test_constructor2() {

    StringBuilder *builder;
    Status status;
    char *str;
    char *init = "This is a test.";
    int initLen = strlen(init);

    status = string_builder_new(&builder, DEFAULT_CAPACITY, DEFAULT_GROWTH_FACTOR, init);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( string_builder_length(builder), initLen );
    CU_ASSERT_EQUAL( string_builder_capacity(builder), DEFAULT_CAPACITY );
    status = string_builder_toString(builder, &str);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( strcmp(str, init), 0 );
    free(str);
    string_builder_destroy(builder);
}

static void _test_append_char() {

    StringBuilder *builder;
    Status status;
    char *str = "Hello World!";
    int strLen = strlen(str);

    status = string_builder_new(&builder, DEFAULT_CAPACITY, DEFAULT_GROWTH_FACTOR, NULL);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( string_builder_length(builder), 0L );
    CU_ASSERT_EQUAL( string_builder_capacity(builder), DEFAULT_CAPACITY );

    int i;
    for (i = 0; i < strLen; i++) {
        status = string_builder_appendChar(builder, str[i]);
        CU_ASSERT_EQUAL( status, OK );
    }

    char *temp;
    status = string_builder_toString(builder, &temp);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( strcmp(str, temp), 0 );
    CU_ASSERT_EQUAL( string_builder_length(builder), strLen );
    free(temp);
    string_builder_destroy(builder);
}

static void _test_append_str() {

    StringBuilder *builder;
    Status status;
    char *str = "Geeks for Geeks";
    int strLen = strlen(str);

    status = string_builder_new(&builder, DEFAULT_CAPACITY, DEFAULT_GROWTH_FACTOR, NULL);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( string_builder_length(builder), 0L );
    CU_ASSERT_EQUAL( string_builder_capacity(builder), DEFAULT_CAPACITY );

    status = string_builder_appendStr(builder, str);
    CU_ASSERT_EQUAL( status, OK );

    char *temp;
    status = string_builder_toString(builder, &temp);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( strcmp(str, temp), 0 );
    CU_ASSERT_EQUAL( string_builder_length(builder), strLen );
    free(temp);
    string_builder_destroy(builder);
}

static void _test_append_boolean1() {

    StringBuilder *builder;
    Status status;
    Boolean b = TRUE;

    status = string_builder_new(&builder, DEFAULT_CAPACITY, DEFAULT_GROWTH_FACTOR, NULL);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( string_builder_length(builder), 0L );
    CU_ASSERT_EQUAL( string_builder_capacity(builder), DEFAULT_CAPACITY );

    status = string_builder_appendBoolean(builder, b);
    CU_ASSERT_EQUAL( status, OK );

    char *temp;
    status = string_builder_toString(builder, &temp);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( strcmp("true", temp), 0 );
    CU_ASSERT_EQUAL( string_builder_length(builder), 4 );
    free(temp);
    string_builder_destroy(builder);
}

static void _test_append_boolean2() {

    StringBuilder *builder;
    Status status;
    Boolean b = FALSE;

    status = string_builder_new(&builder, DEFAULT_CAPACITY, DEFAULT_GROWTH_FACTOR, NULL);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( string_builder_length(builder), 0L );
    CU_ASSERT_EQUAL( string_builder_capacity(builder), DEFAULT_CAPACITY );

    status = string_builder_appendBoolean(builder, b);
    CU_ASSERT_EQUAL( status, OK );

    char *temp;
    status = string_builder_toString(builder, &temp);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( strcmp("false", temp), 0 );
    CU_ASSERT_EQUAL( string_builder_length(builder), 5 );
    free(temp);
    string_builder_destroy(builder);
}

static void _test_append_short() {

    StringBuilder *builder;
    Status status;
    short s = 2024;

    status = string_builder_new(&builder, DEFAULT_CAPACITY, DEFAULT_GROWTH_FACTOR, NULL);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( string_builder_length(builder), 0L );
    CU_ASSERT_EQUAL( string_builder_capacity(builder), DEFAULT_CAPACITY );

    status = string_builder_appendShort(builder, s);
    CU_ASSERT_EQUAL( status, OK );

    char *temp;
    status = string_builder_toString(builder, &temp);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( strcmp("2024", temp), 0 );
    CU_ASSERT_EQUAL( string_builder_length(builder), 4 );
    free(temp);
    string_builder_destroy(builder);
}

static void _test_append_int() {

    StringBuilder *builder;
    Status status;
    int s = 196429;

    status = string_builder_new(&builder, DEFAULT_CAPACITY, DEFAULT_GROWTH_FACTOR, NULL);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( string_builder_length(builder), 0L );
    CU_ASSERT_EQUAL( string_builder_capacity(builder), DEFAULT_CAPACITY );

    status = string_builder_appendInt(builder, s);
    CU_ASSERT_EQUAL( status, OK );

    char *temp;
    status = string_builder_toString(builder, &temp);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( strcmp("196429", temp), 0 );
    CU_ASSERT_EQUAL( string_builder_length(builder), 6 );
    free(temp);
    string_builder_destroy(builder);
}

static void _test_append_long() {

    StringBuilder *builder;
    Status status;
    long l = 999450331L;

    status = string_builder_new(&builder, DEFAULT_CAPACITY, DEFAULT_GROWTH_FACTOR, NULL);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( string_builder_length(builder), 0L );
    CU_ASSERT_EQUAL( string_builder_capacity(builder), DEFAULT_CAPACITY );

    status = string_builder_appendLong(builder, l);
    CU_ASSERT_EQUAL( status, OK );

    char *temp;
    status = string_builder_toString(builder, &temp);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( strcmp("999450331", temp), 0 );
    CU_ASSERT_EQUAL( string_builder_length(builder), 9 );
    free(temp);
    string_builder_destroy(builder);
}

static void _test_append_sub_string1() {

    StringBuilder *builder;
    Status status;
    char *init = "0123456789";
    int initLen = strlen(init);
    char *other = "Hello, World!";
    int otherLen = strlen(other);

    status = string_builder_new(&builder, DEFAULT_CAPACITY, DEFAULT_GROWTH_FACTOR, init);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( string_builder_length(builder), initLen );
    CU_ASSERT_EQUAL( string_builder_capacity(builder), DEFAULT_CAPACITY );

    // Test invalid inputs
    CU_ASSERT_EQUAL( string_builder_appendSubStr(builder, other, -1, otherLen), INVALID_INDEX );
    CU_ASSERT_EQUAL( string_builder_appendSubStr(builder, other, -30, otherLen), INVALID_INDEX );
    CU_ASSERT_EQUAL( string_builder_appendSubStr(builder, other, otherLen + 1, otherLen), INVALID_INDEX );
    CU_ASSERT_EQUAL( string_builder_appendSubStr(builder, other, otherLen + 62, otherLen), INVALID_INDEX );
    CU_ASSERT_EQUAL( string_builder_appendSubStr(builder, other, 0, otherLen + 20), INVALID_INDEX );
    CU_ASSERT_EQUAL( string_builder_appendSubStr(builder, other, 0, otherLen + 98), INVALID_INDEX );
    CU_ASSERT_EQUAL( string_builder_appendSubStr(builder, other, -1, otherLen + 1999), INVALID_INDEX );
    CU_ASSERT_EQUAL( string_builder_appendSubStr(builder, other, -87, otherLen + 2), INVALID_INDEX );

    /*
VALIDATE_INDEX_RANGE(start, end, max) \
            if (0 < 0 || 33 < 0 || 0 > 33 || 0 > 13 || 33 < 13) \
            { return INVALID_INDEX; }
    */

    char *temp;
    status = string_builder_toString(builder, &temp);
    CU_ASSERT_EQUAL( status, OK );
    CU_ASSERT_EQUAL( strcmp(init, temp), 0 );
    CU_ASSERT_EQUAL( string_builder_length(builder), initLen );
    free(temp);
    string_builder_destroy(builder);
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite("StringBuilder Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "StringBuilder - Constructor #1", _test_constructor1);
    CU_add_test(suite, "StringBuilder - Constructor #2", _test_constructor2);
    CU_add_test(suite, "StringBuilder - Append Char", _test_append_char);
    CU_add_test(suite, "StringBuilder - Append Str", _test_append_str);
    CU_add_test(suite, "StringBuilder - Append Boolean #1", _test_append_boolean1);
    CU_add_test(suite, "StringBuilder - Append Boolean #2", _test_append_boolean2);
    CU_add_test(suite, "StringBuilder - Append Short", _test_append_short);
    CU_add_test(suite, "StringBuilder - Append Int", _test_append_int);
    CU_add_test(suite, "StringBuilder - Append Long", _test_append_long);
    CU_add_test(suite, "StringBuilder - Append SubStr #1", _test_append_sub_string1);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
