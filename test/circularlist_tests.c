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

    Iterator *iter;
    Status stat;
    Boolean isEmpty;
    char *element, **array;
    long size, len;

    stat = circularlist_first(list, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = circularlist_last(list, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = circularlist_poll(list, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    size = circularlist_size(list);
    CU_ASSERT_EQUAL(size, 0L);
    isEmpty = circularlist_isEmpty(list);
    CU_ASSERT_EQUAL(isEmpty, TRUE);
    stat = circularlist_iterator(list, &iter);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = circularlist_toArray(list, (void ***)&array, &len);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
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

    Status stat;
    Boolean isEmpty;
    long size;
    char *element;

    size = circularlist_size(list);
    isEmpty = circularlist_isEmpty(list);
    CU_ASSERT_EQUAL(size, 1L);
    CU_ASSERT_EQUAL(isEmpty, FALSE);

    stat = circularlist_first(list, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
    stat = circularlist_last(list, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    stat = circularlist_poll(list, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
}

static void testSingleItemAtHead() {

    CircularList *list;
    Status stat;
    char *element;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItemAtHead() - allocation failure");

    stat = circularlist_addFirst(list, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    validateSingleItem(list);

    stat = circularlist_poll(list, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    validateEmptyCircularList(list);
    circularlist_destroy(list, NULL);

    CU_PASS("testSingleItemAtHead() - Test Passed");
}

static void testSingleItemAtTail() {

    CircularList *list;
    Status stat;
    char *element;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItemAtTail() - allocation failure");

    stat = circularlist_addFirst(list, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    validateSingleItem(list);

    stat = circularlist_poll(list, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    validateEmptyCircularList(list);
    circularlist_destroy(list, NULL);

    CU_PASS("testSingleItemAtTail() - Test Passed");
}

static void testHeadOperations() {

    CircularList *list;
    Status stat;
    int i;
    char *element;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testHeadOperations() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = circularlist_addFirst(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    for (i = LEN - 1; i >= 0; i--) {
        stat = circularlist_poll(list, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
    }

    circularlist_destroy(list, NULL);
    CU_PASS("testHeadOperations() - Test Passed");
}

static void testTailOperations() {

    CircularList *list;
    Status stat;
    int i;
    char *element;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testTailOperations() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = circularlist_addLast(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    for (i = 0; i < LEN; i++) {
        stat = circularlist_poll(list, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
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

    for (i = 0; i < LEN; i++) {
        stat = circularlist_addLast(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    x = 0;
    y = LEN - 1;
    for (i = 0; i < LEN; i++) {
        stat = circularlist_first(list, (void **)&first);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        stat = circularlist_last(list, (void **)&last);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
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

    CircularList *list;
    Status stat;
    int i;
    char **items;
    long len;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testCircularListToArray() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = circularlist_addLast(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    stat = circularlist_toArray(list, (void ***)&items, &len);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(len, LEN);

    for (i = 0; i < len; i++)
        CU_ASSERT_TRUE( strcmp(items[i], array[i]) == 0 );

    free(items);
    circularlist_destroy(list, NULL);

    CU_PASS("testCircularListToArray() - Test Passed");
}

static void testCircularListIterator() {

    CircularList *list;
    Iterator *iter;
    Status stat;
    int i;
    char *element;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testCircularListIterator() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = circularlist_addLast(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    stat = circularlist_iterator(list, &iter);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    i = 0;
    while (iterator_hasNext(iter) == TRUE) {
        stat = iterator_next(iter, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i++]) == 0 );
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

    for (i = 0; i < LEN; i++) {
        stat = circularlist_addFirst(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

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

    CU_pSuite suite = CU_add_suite("ArrayList Tests", NULL, NULL);
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
