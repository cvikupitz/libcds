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
#define LEN 6
static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple"};

static void validateEmptyCircularList(CircularList *list) {

    Array *arr;
    Iterator *iter;
    char *item;

    CU_ASSERT_TRUE( circularlist_first(list, (void **)&item) == STAT_STRUCT_EMPTY );
    CU_ASSERT_TRUE( circularlist_last(list, (void **)&item) == STAT_STRUCT_EMPTY );
    CU_ASSERT_TRUE( circularlist_poll(list, (void **)&item) == STAT_STRUCT_EMPTY );
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

static void validateSingleItem(CircularList *list) {

    char *item;

    CU_ASSERT_TRUE( circularlist_size(list) == 1L );
    CU_ASSERT_TRUE( circularlist_isEmpty(list) == FALSE );
    CU_ASSERT_TRUE( circularlist_first(list, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( circularlist_last(list, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( circularlist_poll(list, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
}

static void testSingleItemAtHead() {

    CircularList *list;
    Status stat;
    char *item;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItemAtHead() - allocation failure");

    CU_ASSERT_TRUE( circularlist_addFirst(list, singleItem) == STAT_SUCCESS );
    validateSingleItem(list);
    CU_ASSERT_TRUE( circularlist_poll(list, (void **)&item) == STAT_STRUCT_EMPTY );
    validateEmptyCircularList(list);
    circularlist_destroy(list, NULL);

    CU_PASS("testSingleItemAtHead() - Test Passed");
}

static void testSingleItemAtTail() {

    CircularList *list;
    Status stat;
    char *item;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItemAtTail() - allocation failure");

    CU_ASSERT_TRUE( circularlist_addFirst(list, singleItem) == STAT_SUCCESS );
    validateSingleItem(list);
    CU_ASSERT_TRUE( circularlist_poll(list, (void **)&item) == STAT_STRUCT_EMPTY );
    validateEmptyCircularList(list);
    circularlist_destroy(list, NULL);

    CU_PASS("testSingleItemAtTail() - Test Passed");
}

static void testHeadOperations() {

    CircularList *list;
    Status stat;
    int i;
    char *item;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testHeadOperations() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( circularlist_addFirst(list, array[i]) == STAT_SUCCESS );

    for (i = LEN - 1; i >= 0; i--) {
        CU_ASSERT_TRUE( circularlist_poll(list, (void **)&item) == STAT_SUCCESS );
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
    }

    circularlist_destroy(list, NULL);
    CU_PASS("testHeadOperations() - Test Passed");
}

static void testTailOperations() {

    CircularList *list;
    Status stat;
    int i;
    char *item;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testTailOperations() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( circularlist_addLast(list, array[i]) == STAT_SUCCESS );

    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE( circularlist_poll(list, (void **)&item) == STAT_SUCCESS );
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
    }

    circularlist_destroy(list, NULL);
    CU_PASS("testTailOperations() - Test Passed");
}

static void testRotations() {

    CircularList *list;
    Status stat;
    int i;
    char *first, *last;
    int x, y;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testRotations() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( circularlist_addLast(list, array[i]) == STAT_SUCCESS );

    x = 0;
    y = LEN - 1;
    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE(circularlist_first(list, (void **)&first) == STAT_SUCCESS);
        CU_ASSERT_TRUE(circularlist_last(list, (void **)&last) == STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(first, array[x]) == 0 );
        CU_ASSERT_TRUE( strcmp(last, array[y]) == 0 );
        circularlist_rotate(list);
        x = (x + 1) % LEN;
        y = (y + 1) % LEN;
    }
    circularlist_destroy(list, NULL);

    CU_PASS("testRotations() - Test Passed");
}

static void testCircularListToArray() {

    Array *arr;
    CircularList *list;
    Status stat;
    int i;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testCircularListToArray() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( circularlist_addLast(list, array[i]) == STAT_SUCCESS );
    CU_ASSERT_TRUE( circularlist_toArray(list, &arr) == STAT_SUCCESS );
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
    int i;
    char *item;

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
        CU_ASSERT_TRUE( circularlist_addFirst(list, array[i]) == STAT_SUCCESS );

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

    CU_add_test(suite, "CircularList - Empty List", testEmptyCircularList);
    CU_add_test(suite, "CircularList - Single Head Item", testSingleItemAtHead);
    CU_add_test(suite, "CircularList - Single Tail Item", testSingleItemAtTail);
    CU_add_test(suite, "CircularList - Head Insert & Delete", testHeadOperations);
    CU_add_test(suite, "CircularList - Tail insert & Delete", testTailOperations);
    CU_add_test(suite, "CircularList - Rotations", testRotations);
    CU_add_test(suite, "CircularList - Array", testCircularListToArray);
    CU_add_test(suite, "CircularList - Iterator", testCircularListIterator);
    CU_add_test(suite, "CircularList - Clear", testCircularListClear);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
