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
#include "circularlist.h"

/* Single item used for testing */
static char *singleItem = "Test";

/* Collection of items used for testing */
#define LEN 9
static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple", "gray", "white", "black"};

static void validateEmptyCircularList(CircularList *list) {

    Array *arr;
    Iterator *iter;
    char *item;

    CU_ASSERT_TRUE( circularlist_first(list, (void **)&item) == STAT_STRUCT_EMPTY );
    CU_ASSERT_TRUE( circularlist_last(list, (void **)&item) == STAT_STRUCT_EMPTY );
    CU_ASSERT_TRUE( circularlist_set(list, 0L, singleItem, (void **)&item) == STAT_STRUCT_EMPTY );
    CU_ASSERT_TRUE( circularlist_remove(list, 0L, (void **)&item) == STAT_STRUCT_EMPTY );
    CU_ASSERT_TRUE( circularlist_size(list) == 0L );
    CU_ASSERT_TRUE( circularlist_isEmpty(list) == TRUE );
    CU_ASSERT_TRUE( circularlist_iterator(list, &iter) == STAT_STRUCT_EMPTY );
    CU_ASSERT_TRUE( circularlist_toArray(list, &arr) == STAT_STRUCT_EMPTY );
}

static void testEmptyCircularList() {

    CircularList *list;
    Status stat;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyCircularList() - allocation failure");

    validateEmptyCircularList(list);
    circularlist_destroy(list, NULL);

    CU_PASS("testEmptyCircularList() - Test Passed");
}

static void testSingleItem() {

    CircularList *list;
    Status stat;
    char *item;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    CU_ASSERT_TRUE( circularlist_addLast(list, singleItem) == STAT_SUCCESS );
    CU_ASSERT_TRUE( circularlist_size(list) == 1L );
    CU_ASSERT_TRUE( circularlist_isEmpty(list) == FALSE );
    CU_ASSERT_TRUE( circularlist_get(list, 0L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( circularlist_remove(list, 99L, (void **)&item) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( circularlist_remove(list, 0L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( circularlist_remove(list, 0L, (void **)&item) == STAT_STRUCT_EMPTY );

    validateEmptyCircularList(list);
    circularlist_destroy(list, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

static void testInvalidIndexAccess() {

    CircularList *list;
    Status stat;
    char *item;
    int i;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testInvalidIndexAccess() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( circularlist_addLast(list, array[i]) == STAT_SUCCESS );

    CU_ASSERT_TRUE( circularlist_insert(list, LEN + 1L, singleItem) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( circularlist_insert(list, LEN + 1L, singleItem) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( circularlist_insert(list, LEN + 10L, singleItem) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( circularlist_get(list, -1, (void **)&item) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( circularlist_get(list, LEN + 1L, (void **)&item) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( circularlist_get(list, LEN + 10L, (void **)&item) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( circularlist_set(list, -1, singleItem, (void **)&item) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( circularlist_set(list, LEN + 1L, singleItem, (void **)&item) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( circularlist_set(list, LEN + 10L, singleItem, (void **)&item) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( circularlist_remove(list, -1, (void **)&item) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( circularlist_remove(list, LEN + 1L, (void **)&item) == STAT_INVALID_INDEX );
    CU_ASSERT_TRUE( circularlist_remove(list, LEN + 10L, (void **)&item) == STAT_INVALID_INDEX );

    circularlist_destroy(list, NULL);

    CU_PASS("testInvalidIndexAccess() - Test Passed");
}

static void testAddFirst() {

    CircularList *list;
    Status stat;
    int i, j;
    char *item;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testAddFirst() - allocation failure");

    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE( circularlist_addFirst(list, array[i]) == STAT_SUCCESS );
        CU_ASSERT_TRUE( circularlist_first(list, (void **)&item) == STAT_SUCCESS );
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
        CU_ASSERT_TRUE( circularlist_last(list, (void **)&item) == STAT_SUCCESS );
        CU_ASSERT_TRUE( strcmp(item, array[0]) == 0 );
    }

    for (i = 0, j = LEN - 1; i < LEN; i++, j--) {
        CU_ASSERT_TRUE(circularlist_get(list, i, (void **)&item) == STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(item, array[j]) == 0 );
    }

    circularlist_destroy(list, NULL);

    CU_PASS("testAddFirst() - Test Passed");
}

static void testAddLast() {

    CircularList *list;
    Status stat;
    int i;
    char *item;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testAddLast() - allocation failure");

    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE( circularlist_addLast(list, array[i]) == STAT_SUCCESS );
        CU_ASSERT_TRUE( circularlist_first(list, (void **)&item) == STAT_SUCCESS );
        CU_ASSERT_TRUE( strcmp(item, array[0]) == 0 );
        CU_ASSERT_TRUE( circularlist_last(list, (void **)&item) == STAT_SUCCESS );
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
    }

    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE( circularlist_get(list, i, (void **)&item) == STAT_SUCCESS );
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
    }

    circularlist_destroy(list, NULL);

    CU_PASS("testAddLast() - Test Passed");
}

static void testInsertions() {

    CircularList *list;
    Status stat;
    int i;
    char *item;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testInsertions() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( circularlist_addLast(list, array[i]) == STAT_SUCCESS );
    for (i = 0L; i < LEN; i++) {
        CU_ASSERT_TRUE( circularlist_get(list, i, (void **)&item) == STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
    }

    (void)circularlist_insert(list, 0L, singleItem);
    (void)circularlist_insert(list, 3L, singleItem);
    (void)circularlist_insert(list, 6L, singleItem);
    CU_ASSERT_TRUE( circularlist_get(list, 0L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( circularlist_get(list, 3L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( circularlist_get(list, 6L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );

    circularlist_destroy(list, NULL);

    CU_PASS("testAdditions() - Test Passed");
}

static void testSetItem() {

    CircularList *list;
    Status stat;
    int i;
    char *item;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSetItem() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( circularlist_addLast(list, array[i]) == STAT_SUCCESS );

    CU_ASSERT_TRUE( circularlist_set(list, 0L, singleItem, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, array[0]) == 0 );
    CU_ASSERT_TRUE( circularlist_set(list, 4L, singleItem, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, array[4]) == 0 );
    CU_ASSERT_TRUE( circularlist_set(list, 7L, singleItem, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, array[7]) == 0 );

    CU_ASSERT_TRUE( circularlist_get(list, 0L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( circularlist_get(list, 4L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( circularlist_get(list, 7L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );

    circularlist_destroy(list, NULL);

    CU_PASS("testSetItem() - Test Passed");
}

static void testSequentialDelete() {

    CircularList *list;
    Status stat;
    int i;
    char *item;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSequentialDelete() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( circularlist_addLast(list, array[i]) == STAT_SUCCESS );

    for (i = 0L; i < LEN; i++) {
        CU_ASSERT_TRUE( circularlist_remove(list, 0L, (void **)&item) == STAT_SUCCESS );
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
    }

    circularlist_destroy(list, NULL);

    CU_PASS("testSequentialDelete() - Test Passed");
}

static void testRandomDelete() {

    CircularList *list;
    Status stat;
    int i;
    char *item;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testRandomDelete() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( circularlist_addLast(list, array[i]) == STAT_SUCCESS );

    CU_ASSERT_TRUE( circularlist_remove(list, 7L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, array[7]) == 0 );
    CU_ASSERT_TRUE( circularlist_remove(list, 5L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, array[5]) == 0 );
    CU_ASSERT_TRUE( circularlist_remove(list, 1L, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, array[1]) == 0 );

    circularlist_destroy(list, NULL);

    CU_PASS("testRandomDelete() - Test Passed");
}

static void testCircularListToArray() {

    CircularList *list;
    Array *arr;
    Status stat;
    int i;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testCircularListToArray() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( circularlist_addLast(list, array[i]) == STAT_SUCCESS );
    CU_ASSERT_TRUE( circularlist_toArray(list, &arr) == STAT_SUCCESS);
    CU_ASSERT_TRUE( arr->len == LEN );

    for (i = 0; i < arr->len; i++)
        CU_ASSERT_TRUE( strcmp(arr->items[i], array[i]) == 0 );

    FREE_ARRAY(arr)
    circularlist_destroy(list, NULL);

    CU_PASS("testCircularListToArray() - Test Passed");
}

static void testCircularListIterator() {

    CircularList *list;
    Iterator *iter;
    Status stat;
    char *item;
    int i;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testCircularListIterator() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( circularlist_addLast(list, array[i]) == STAT_SUCCESS );
    CU_ASSERT_TRUE( circularlist_iterator(list, &iter) == STAT_SUCCESS );

    i = 0;
    while (iterator_hasNext(iter) == TRUE) {
        CU_ASSERT_TRUE( iterator_next(iter, (void **)&item) == STAT_SUCCESS );
        CU_ASSERT_TRUE( strcmp(item, array[i++]) == 0 );
    }

    iterator_destroy(iter);
    circularlist_destroy(list, NULL);

    CU_PASS("testCircularListIterator() - Test Passed");
}

static void testCircularListClear() {

    CircularList *list;
    Status stat;
    int i;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testCircularListClear() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( circularlist_addLast(list, array[i]) == STAT_SUCCESS );
    circularlist_clear(list, NULL);
    validateEmptyCircularList(list);
    circularlist_destroy(list, NULL);

    CU_PASS("testCircularListClear() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite("CircularList Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "CircularList - Empty", testEmptyCircularList);
    CU_add_test(suite, "CircularList - Single Item", testSingleItem);
    CU_add_test(suite, "CircularList - Invalid Index Access", testInvalidIndexAccess);
    CU_add_test(suite, "CircularList - Head Insertions", testAddFirst);
    CU_add_test(suite, "CircularList - Tail Insertions", testAddLast);
    CU_add_test(suite, "CircularList - Random Insertions", testInsertions);
    CU_add_test(suite, "CircularList - Set", testSetItem);
    CU_add_test(suite, "CircularList - Sequential Delete", testSequentialDelete);
    CU_add_test(suite, "CircularList - Random Delete", testRandomDelete);
    CU_add_test(suite, "CircularList - Array", testCircularListToArray);
    CU_add_test(suite, "CircularList - Iterator", testCircularListIterator);
    CU_add_test(suite, "CircularList - Clear", testCircularListClear);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
