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
#include <CUnit/Basic.h>
#include "arraylist.h"

/* Default ArrayList capacity */
#define CAPACITY 2L

/* Single item used for testing */
static char *singleItem = "Test";

/* Collection of items used for testing */
#define LEN 9
static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple", "gray", "white", "black"};

static void validateEmptyArrayList(ArrayList *list) {

    Array *arr;
    Iterator *iter;
    char *item;

    CU_ASSERT_TRUE( arraylist_get(list, 0L, (void **)&item) == STAT_STRUCT_EMPTY );
    CU_ASSERT_TRUE( arraylist_set(list, 0L, singleItem, (void **)&item) == STAT_STRUCT_EMPTY );
    CU_ASSERT_TRUE( arraylist_remove(list, 0L, (void **)&item) == STAT_STRUCT_EMPTY );
    CU_ASSERT_TRUE( arraylist_size(list) == 0L );
    CU_ASSERT_TRUE( arraylist_isEmpty(list) == TRUE );
    CU_ASSERT_TRUE( arraylist_iterator(list, &iter) == STAT_STRUCT_EMPTY );
    CU_ASSERT_TRUE( arraylist_toArray(list, &arr) == STAT_STRUCT_EMPTY );
}

static void testEmptyArrayList() {

    ArrayList *list;
    Status stat;

    stat = arraylist_new(&list, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyArrayList() - allocation failure");

    validateEmptyArrayList(list);
    arraylist_destroy(list, NULL);
    CU_PASS("testEmptyArrayList() - Test Passed");
}

static void testSingleItem() {

    ArrayList *list;
    Status stat;
    char *item;

    stat = arraylist_new(&list, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    CU_ASSERT_TRUE( arraylist_add(list, singleItem) == STAT_SUCCESS );
    CU_ASSERT_TRUE( arraylist_size(list) == 1L );
    CU_ASSERT_TRUE( arraylist_capacity(list) == CAPACITY );
    CU_ASSERT_TRUE( arraylist_isEmpty(list) == FALSE );
    CU_ASSERT_TRUE( arraylist_get(list, 0L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( arraylist_remove(list, 99L, (void **)&item) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( arraylist_remove(list, 0L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( arraylist_remove(list, 0L, (void **)&item) == STAT_STRUCT_EMPTY );

    validateEmptyArrayList(list);
    arraylist_destroy(list, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

static void testInsertions() {

    ArrayList *list;
    Status stat;
    int i;
    char *item;

    stat = arraylist_new(&list, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testInsertions() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( arraylist_add(list, array[i]) == STAT_SUCCESS );

    for (i = 0L; i < LEN; i++) {
        CU_ASSERT_TRUE( arraylist_get(list, i, (void **)&item) == STAT_SUCCESS );
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
    }

    (void)arraylist_insert(list, 0L, singleItem);
    (void)arraylist_insert(list, 3L, singleItem);
    (void)arraylist_insert(list, 6L, singleItem);
    CU_ASSERT_TRUE( arraylist_get(list, 0L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( arraylist_get(list, 3L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( arraylist_get(list, 6L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );

    arraylist_destroy(list, NULL);

    CU_PASS("testAdditions() - Test Passed");
}

static void testSetItem() {

    ArrayList *list;
    Status stat;
    int i;
    char *item;

    stat = arraylist_new(&list, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSetItem() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE(arraylist_add(list, array[i]) == STAT_SUCCESS);

    CU_ASSERT_TRUE( arraylist_set(list, 0L, singleItem, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, array[0]) == 0 );
    CU_ASSERT_TRUE( arraylist_set(list, 4L, singleItem, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, array[4]) == 0 );
    CU_ASSERT_TRUE( arraylist_set(list, 7L, singleItem, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, array[7]) == 0 );

    CU_ASSERT_TRUE( arraylist_get(list, 0L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( arraylist_get(list, 4L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( arraylist_get(list, 7L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );

    arraylist_destroy(list, NULL);

    CU_PASS("testSetItem() - Test Passed");
}

static void testSequentialDelete() {

    ArrayList *list;
    Status stat;
    int i;
    char *item;

    stat = arraylist_new(&list, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSequentialDelete() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( arraylist_add(list, array[i]) == STAT_SUCCESS );

    for (i = 0L; i < LEN; i++) {
        CU_ASSERT_TRUE( arraylist_remove(list, 0L, (void **)&item) == STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
    }

    arraylist_destroy(list, NULL);

    CU_PASS("testSequentialDelete() - Test Passed");
}

static void testRandomDelete() {

    ArrayList *list;
    Status stat;
    int i;
    char *item;

    stat = arraylist_new(&list, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testRandomDelete() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE(arraylist_add(list, array[i]) == STAT_SUCCESS);

    CU_ASSERT_TRUE( arraylist_remove(list, 7L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, array[7]) == 0 );
    CU_ASSERT_TRUE( arraylist_remove(list, 5L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, array[5]) == 0 );
    CU_ASSERT_TRUE( arraylist_remove(list, 1L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, array[1]) == 0 );

    arraylist_destroy(list, NULL);

    CU_PASS("testRandomDelete() - Test Passed");
}

static void testEnsureCapacity() {

    ArrayList *list;
    Status stat;

    stat = arraylist_new(&list, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEnsureCapacity() - allocation failure");

    CU_ASSERT_TRUE( arraylist_capacity(list) == CAPACITY );
    CU_ASSERT_TRUE( arraylist_ensureCapacity(list, CAPACITY - 1) == STAT_SUCCESS );
    CU_ASSERT_TRUE( arraylist_capacity(list) == CAPACITY );
    CU_ASSERT_TRUE( arraylist_ensureCapacity(list, CAPACITY + 20L) == STAT_SUCCESS );
    CU_ASSERT_TRUE( arraylist_capacity(list) == (CAPACITY + 20L) );

    arraylist_destroy(list, NULL);

    CU_PASS("testEnsureCapacity() - Test Passed");
}

static void testTrimToSize() {

    ArrayList *list;
    Status stat;

    stat = arraylist_new(&list, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testTrimToSize() - allocation failure");

    CU_ASSERT_TRUE( arraylist_trimToSize(list) == STAT_STRUCT_EMPTY );
    CU_ASSERT_TRUE( arraylist_add(list, singleItem) == STAT_SUCCESS );
    CU_ASSERT_TRUE( arraylist_trimToSize(list) == STAT_SUCCESS );

    arraylist_destroy(list, NULL);
    CU_PASS("testEnsureCapacity() - Test Passed");
}

static void testInvalidIndexAccess() {

    ArrayList *list;
    Status stat;
    int i;
    char *item;

    stat = arraylist_new(&list, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testInvalidIndexAccess() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( arraylist_add(list, array[i]) == STAT_SUCCESS );

    CU_ASSERT_TRUE( arraylist_insert(list, LEN + 1L, singleItem) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( arraylist_insert(list, LEN + 1L, singleItem) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( arraylist_insert(list, LEN + 10L, singleItem) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( arraylist_get(list, -1, (void **)&item) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( arraylist_get(list, LEN + 1L, (void **)&item) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( arraylist_get(list, LEN + 10L, (void **)&item) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( arraylist_set(list, -1, singleItem, (void **)&item) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( arraylist_set(list, LEN + 1L, singleItem, (void **)&item) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( arraylist_set(list, LEN + 10L, singleItem, (void **)&item) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( arraylist_remove(list, -1, (void **)&item) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( arraylist_remove(list, LEN + 1L, (void **)&item) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( arraylist_remove(list, LEN + 10L, (void **)&item) == STAT_INVALID_INDEX );

    arraylist_destroy(list, NULL);

    CU_PASS("testInvalidIndexAccess() - Test Passed");
}

static void testArrayListToArray() {

    Array *arr;
    ArrayList *list;
    Status stat;
    int i;

    stat = arraylist_new(&list, LEN);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testArrayListToArray() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( arraylist_add(list, array[i]) == STAT_SUCCESS );
    CU_ASSERT_TRUE( arraylist_toArray(list, &arr) == STAT_SUCCESS );
    CU_ASSERT_TRUE( arr->len == LEN );

    for (i = 0; i < arr->len; i++)
        CU_ASSERT_TRUE( strcmp(arr->items[i], array[i]) == 0 );

    FREE_ARRAY(arr)
    arraylist_destroy(list, NULL);

    CU_PASS("testArrayListToArray() - Test Passed");
}

static void testArrayListIterator() {

    ArrayList *list;
    Iterator *iter;
    Status stat;
    int i;
    char *item;

    stat = arraylist_new(&list, LEN);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testArrayListIterator() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( arraylist_add(list, array[i]) == STAT_SUCCESS );
    CU_ASSERT_TRUE( arraylist_iterator(list, &iter) == STAT_SUCCESS );

    i = 0;
    while (iterator_hasNext(iter) == TRUE) {
        CU_ASSERT_TRUE( iterator_next(iter, (void **)&item) == STAT_SUCCESS );
        CU_ASSERT_TRUE( strcmp(item, array[i++]) == 0 );
    }

    iterator_destroy(iter);
    arraylist_destroy(list, NULL);

    CU_PASS("testArrayListIterator() - Test Passed");
}

static void testArrayListClear() {

    ArrayList *list;
    Status stat;
    int i;

    stat = arraylist_new(&list, LEN);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testArrayListClear() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( arraylist_add(list, array[i]) == STAT_SUCCESS );
    arraylist_clear(list, NULL);
    validateEmptyArrayList(list);
    arraylist_destroy(list, NULL);

    CU_PASS("testArrayListClear() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite("ArrayList Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "ArrayList - Empty", testEmptyArrayList);
    CU_add_test(suite, "ArrayList - Single Item", testSingleItem);
    CU_add_test(suite, "ArrayList - Invalid Index Access", testInvalidIndexAccess);
    CU_add_test(suite, "ArrayList - Additions", testInsertions);
    CU_add_test(suite, "ArrayList - Set", testSetItem);
    CU_add_test(suite, "ArrayList - Sequential Delete", testSequentialDelete);
    CU_add_test(suite, "ArrayList - Random Delete", testRandomDelete);
    CU_add_test(suite, "ArrayList - Ensure", testEnsureCapacity);
    CU_add_test(suite, "ArrayList - Trim to Size", testTrimToSize);
    CU_add_test(suite, "ArrayList - Array", testArrayListToArray);
    CU_add_test(suite, "ArrayList - Iterator", testArrayListIterator);
    CU_add_test(suite, "ArrayList - Clear", testArrayListClear);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
