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
#include "linkedlist.h"

static char *singleItem = "Test";

#define LEN 9
static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple", "gray", "white", "black"};

static void validateEmptyLinkedList(LinkedList *list) {

    Status stat;
    char *element;
    
    stat = linkedlist_first(list, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = linkedlist_last(list, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = linkedlist_set(list, 0L, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = linkedlist_remove(list, 0L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    long size = linkedlist_size(list);
    CU_ASSERT_EQUAL(size, 0L);

    Boolean isEmpty = linkedlist_isEmpty(list);
    CU_ASSERT_EQUAL(isEmpty, TRUE);
    
    Iterator *iter;
    stat = linkedlist_iterator(list, &iter);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    int **array;
    long len;
    stat = linkedlist_toArray(list, (void ***)&array, &len);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
}

void testEmptyLinkedList() {

    LinkedList *list = NULL;
    Status stat;

    stat = linkedlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyLinkedList() - allocation failure");
    
    validateEmptyLinkedList(list);
    linkedlist_destroy(list, NULL);
    CU_PASS("testEmptyLinkedList() - Test Passed");
}

void testSingleItem() {
    
    LinkedList *list;
    Status stat;

    stat = linkedlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    stat = linkedlist_add(list, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    long size = linkedlist_size(list);
    Boolean isEmpty = linkedlist_isEmpty(list);
    CU_ASSERT_EQUAL(size, 1L);
    CU_ASSERT_EQUAL(isEmpty, FALSE);

    char *element;
    stat = linkedlist_get(list, 0L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    stat = linkedlist_remove(list, 99L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);

    stat = linkedlist_remove(list, 0L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    stat = linkedlist_remove(list, 0L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    validateEmptyLinkedList(list);
    linkedlist_destroy(list, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

void testAddFirst() {

    LinkedList *list;
    Status stat;

    stat = linkedlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testAddFirst() - allocation failure");

    int i, j;
    char *element;
    for (i = 0; i < LEN; i++) {
        stat = linkedlist_addFirst(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        stat = linkedlist_first(list, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
        stat = linkedlist_last(list, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[0]) == 0 );
    }

    for (i = 0, j = LEN - 1; i < LEN; i++, j--) {
        stat = linkedlist_get(list, i, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[j]) == 0 );
    }

    linkedlist_destroy(list, NULL);

    CU_PASS("testAddFirst() - Test Passed");
}

void testAddLast() {

    LinkedList *list;
    Status stat;

    stat = linkedlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testAddLast() - allocation failure");

    int i;
    char *element;
    for (i = 0; i < LEN; i++) {
        stat = linkedlist_addLast(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        stat = linkedlist_first(list, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[0]) == 0 );
        stat = linkedlist_last(list, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
    }

    for (i = 0; i < LEN; i++) {
        stat = linkedlist_get(list, i, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
    }

    linkedlist_destroy(list, NULL);

    CU_PASS("testAddLast() - Test Passed");
}

void testInsertions() {

    LinkedList *list;
    Status stat;

    stat = linkedlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testInsertions() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = linkedlist_add(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char *element;
    for (i = 0L; i < LEN; i++) {
        stat = linkedlist_get(list, i, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
    }

    stat = linkedlist_insert(list, 0L, singleItem);
    stat = linkedlist_insert(list, 3L, singleItem);
    stat = linkedlist_insert(list, 6L, singleItem);
    stat = linkedlist_get(list, 0L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
    stat = linkedlist_get(list, 3L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
    stat = linkedlist_get(list, 6L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    linkedlist_destroy(list, NULL);

    CU_PASS("testAdditions() - Test Passed");
}

void testSetItem() {

    LinkedList *list;
    Status stat;

    stat = linkedlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSetItem() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = linkedlist_add(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char *element;
    stat = linkedlist_set(list, 0L, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, array[0]) == 0 );
    stat = linkedlist_set(list, 4L, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, array[4]) == 0 );
    stat = linkedlist_set(list, 7L, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, array[7]) == 0 );

    stat = linkedlist_get(list, 0L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
    stat = linkedlist_get(list, 4L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
    stat = linkedlist_get(list, 7L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    linkedlist_destroy(list, NULL);

    CU_PASS("testSetItem() - Test Passed");
}

void testSequentialDelete() {

    LinkedList *list;
    Status stat;

    stat = linkedlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSequentialDelete() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = linkedlist_add(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char *element;
    for (i = 0L; i < LEN; i++) {
        stat = linkedlist_remove(list, 0L, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
    }

    linkedlist_destroy(list, NULL);

    CU_PASS("testSequentialDelete() - Test Passed");
}

void testRandomDelete() {

    LinkedList *list;
    Status stat;

    stat = linkedlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testRandomDelete() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = linkedlist_add(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char *element;
    stat = linkedlist_remove(list, 7L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, array[7]) == 0 );
    stat = linkedlist_remove(list, 5L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, array[5]) == 0 );
    stat = linkedlist_remove(list, 1L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, array[1]) == 0 );

    linkedlist_destroy(list, NULL);

    CU_PASS("testRandomDelete() - Test Passed");
}

void testInvalidIndexAccess() {

    LinkedList *list;
    Status stat;

    stat = linkedlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testInvalidIndexAccess() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = linkedlist_add(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    stat = linkedlist_insert(list, LEN + 1L, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);
    stat = linkedlist_insert(list, LEN + 1L, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);
    stat = linkedlist_insert(list, LEN + 10L, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);
    
    char *element;
    stat = linkedlist_get(list, -1, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);
    stat = linkedlist_get(list, LEN + 1L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);
    stat = linkedlist_get(list, LEN + 10L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);

    stat = linkedlist_set(list, -1, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);
    stat = linkedlist_set(list, LEN + 1L, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);
    stat = linkedlist_set(list, LEN + 10L, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);

    stat = linkedlist_remove(list, -1, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);
    stat = linkedlist_remove(list, LEN + 1L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);
    stat = linkedlist_remove(list, LEN + 10L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);

    linkedlist_destroy(list, NULL);
    CU_PASS("testInvalidIndexAccess() - Test Passed");
}

void testLinkedListToArray() {

    LinkedList *list;
    Status stat;

    stat = linkedlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testLinkedListToArray() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = linkedlist_add(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char **items;
    long len;
    stat = linkedlist_toArray(list, (void ***)&items, &len);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(len, LEN);

    for (i = 0; i < len; i++)
        CU_ASSERT_TRUE( strcmp(items[i], array[i]) == 0 );

    free(items);
    linkedlist_destroy(list, NULL);
    CU_PASS("testLinkedListToArray() - Test Passed");
}

void testLinkedListIterator() {

    LinkedList *list;
    Status stat;

    stat = linkedlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testLinkedListIterator() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = linkedlist_add(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    Iterator *iter;
    stat = linkedlist_iterator(list, &iter);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    char *element;
    i = 0;
    while (iterator_hasNext(iter) == TRUE) {
        stat = iterator_next(iter, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i++]) == 0 );
    }

    iterator_destroy(iter);
    linkedlist_destroy(list, NULL);
    CU_PASS("testLinkedListIterator() - Test Passed");
}

void testLinkedListClear() {
    
    LinkedList *list;
    Status stat;

    stat = linkedlist_new(&list);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testLinkedListClear() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = linkedlist_add(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    linkedlist_clear(list, NULL);
    validateEmptyLinkedList(list);
    linkedlist_destroy(list, NULL);

    CU_PASS("testLinkedListClear() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("LinkedList Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "Empty arraylist", testEmptyLinkedList);
    CU_add_test(suite, "Single item", testSingleItem);
    CU_add_test(suite, "Invalid Index Access", testInvalidIndexAccess);
    CU_add_test(suite, "LinkedList addFirst", testAddFirst);
    CU_add_test(suite, "LinkedList addLast", testAddLast);
    CU_add_test(suite, "LinkedList additions", testInsertions);
    CU_add_test(suite, "LinkedList set", testSetItem);
    CU_add_test(suite, "LinkedList sequential delete", testSequentialDelete);
    CU_add_test(suite, "LinkedList random delete", testRandomDelete);
    CU_add_test(suite, "LinkedList toArray", testLinkedListToArray);
    CU_add_test(suite, "LinkedList iterator", testLinkedListIterator);
    CU_add_test(suite, "LinkedList clear", testLinkedListClear);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    
    return 0;
}
