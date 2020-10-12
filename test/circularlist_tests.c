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

static void validateEmptyCircularList(CircularList *list) {

    Status stat;
    int *element;
    
    stat = circularlist_first(list, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = circularlist_last(list, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = circularlist_poll(list, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    long size = circularlist_size(list);
    CU_ASSERT_EQUAL(size, 0L);

    Boolean isEmpty = circularlist_isEmpty(list);
    CU_ASSERT_EQUAL(isEmpty, TRUE);
    
    Iterator *iter;
    stat = circularlist_iterator(list, &iter);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    int **array;
    long len;
    stat = circularlist_toArray(list, (void ***)&array, &len);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
}

void testEmptyCircularList() {

    CircularList *list = NULL;
    Status stat;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyCircularList() - allocation failure");
    
    validateEmptyCircularList(list);
    circularlist_destroy(list, NULL);
    CU_PASS("testEmptyCircularList() - Test Passed");
}

static char *singleItem = "Test";

static void validateSingleItem(CircularList *list) {

    Status stat;
    long size = circularlist_size(list);
    Boolean isEmpty = circularlist_isEmpty(list);
    CU_ASSERT_EQUAL(size, 1L);
    CU_ASSERT_EQUAL(isEmpty, FALSE);

    char *element;
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

void testSingleItemAtHead() {

    CircularList *list;
    Status stat;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItemAtHead() - allocation failure");

    stat = circularlist_addFirst(list, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    validateSingleItem(list);

    char **element;
    stat = circularlist_poll(list, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    validateEmptyCircularList(list);
    circularlist_destroy(list, NULL);

    CU_PASS("testSingleItemAtHead() - Test Passed");
}

void testSingleItemAtTail() {
    
    CircularList *list;
    Status stat;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItemAtTail() - allocation failure");

    stat = circularlist_addFirst(list, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    validateSingleItem(list);

    char **element;
    stat = circularlist_poll(list, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    validateEmptyCircularList(list);
    circularlist_destroy(list, NULL);

    CU_PASS("testSingleItemAtTail() - Test Passed");
}

#define LEN 6
static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple"};

void testHeadOperations() {

    CircularList *list;
    Status stat;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testHeadOperations() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = circularlist_addFirst(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char *element;
    for (i = LEN - 1; i >= 0; i--) {
        stat = circularlist_poll(list, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
    }

    circularlist_destroy(list, NULL);
    CU_PASS("testHeadOperations() - Test Passed");
}

void testTailOperations() {

    CircularList *list;
    Status stat;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testTailOperations() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = circularlist_addLast(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char *element;
    for (i = 0; i < LEN; i++) {
        stat = circularlist_poll(list, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
    }

    circularlist_destroy(list, NULL);
    CU_PASS("testTailOperations() - Test Passed");
}

void testRotations() {
    
    CircularList *list;
    Status stat;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testRotations() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = circularlist_addLast(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char *first, *last;
    int x = 0, y = LEN - 1;
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

void testCircularListToArray() {

    CircularList *list;
    Status stat;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testCircularListToArray() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = circularlist_addLast(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char **items;
    long len;
    stat = circularlist_toArray(list, (void ***)&items, &len);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(len, LEN);

    for (i = 0; i < len; i++)
        CU_ASSERT_TRUE( strcmp(items[i], array[i]) == 0 );

    free(items);
    circularlist_destroy(list, NULL);
    CU_PASS("testCircularListToArray() - Test Passed");
}

void testCircularListIterator() {

    CircularList *list;
    Status stat;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testCircularListIterator() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = circularlist_addLast(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    Iterator *iter;
    stat = circularlist_iterator(list, &iter);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    char *element;
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

void testCircularListClear() {
    
    CircularList *list;
    Status stat;

    stat = circularlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testCircularListClear() - allocation failure");

    int i;
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

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("CircularList Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "Empty circular list", testEmptyCircularList);
    CU_add_test(suite, "Single item at head", testSingleItemAtHead);
    CU_add_test(suite, "Single item at head", testSingleItemAtTail);
    CU_add_test(suite, "Head insert then delete", testHeadOperations);
    CU_add_test(suite, "Tail insert then delete", testTailOperations);
    CU_add_test(suite, "List Rotations", testRotations);
    CU_add_test(suite, "CircularList toArray", testCircularListToArray);
    CU_add_test(suite, "CircularList iterator", testCircularListIterator);
    CU_add_test(suite, "CircularList clear", testCircularListClear);
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
