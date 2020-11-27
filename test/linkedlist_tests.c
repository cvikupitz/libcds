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

/* Single item used for testing */
static char *singleItem = "Test";

/* Collection of items used for testing */
#define LEN 9
static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple", "gray", "white", "black"};

static void validateEmptyLinkedList(LinkedList *list) {

    Array *arr;
    Iterator *iter;
    char *item;

    CU_ASSERT_TRUE( linkedlist_first(list, (void **)&item) == STRUCT_EMPTY );
    CU_ASSERT_TRUE( linkedlist_last(list, (void **)&item) == STRUCT_EMPTY );
    CU_ASSERT_TRUE( linkedlist_set(list, 0L, singleItem, (void **)&item) == STRUCT_EMPTY );
    CU_ASSERT_TRUE( linkedlist_remove(list, 0L, (void **)&item) == STRUCT_EMPTY );
    CU_ASSERT_TRUE( linkedlist_size(list) == 0L );
    CU_ASSERT_TRUE( linkedlist_isEmpty(list) == TRUE );
    CU_ASSERT_TRUE( linkedlist_iterator(list, &iter) == STRUCT_EMPTY );
    CU_ASSERT_TRUE( linkedlist_toArray(list, &arr) == STRUCT_EMPTY );
}

static void testEmptyLinkedList() {

    LinkedList *list;
    Status stat;

    stat = linkedlist_new(&list);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testEmptyLinkedList() - allocation failure");

    validateEmptyLinkedList(list);
    linkedlist_destroy(list, NULL);

    CU_PASS("testEmptyLinkedList() - Test Passed");
}

static void testSingleItem() {

    LinkedList *list;
    Status stat;
    char *item;

    stat = linkedlist_new(&list);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    CU_ASSERT_TRUE( linkedlist_addLast(list, singleItem) == OK );
    CU_ASSERT_TRUE( linkedlist_size(list) == 1L );
    CU_ASSERT_TRUE( linkedlist_isEmpty(list) == FALSE );
    CU_ASSERT_TRUE( linkedlist_get(list, 0L, (void **)&item) == OK );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( linkedlist_remove(list, 99L, (void **)&item) == INVALID_INDEX );
    CU_ASSERT_TRUE( linkedlist_remove(list, 0L, (void **)&item) == OK );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( linkedlist_remove(list, 0L, (void **)&item) == STRUCT_EMPTY );

    validateEmptyLinkedList(list);
    linkedlist_destroy(list, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

static void testAddFirst() {

    LinkedList *list;
    Status stat;
    int i, j;
    char *item;

    stat = linkedlist_new(&list);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testAddFirst() - allocation failure");

    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE( linkedlist_addFirst(list, array[i]) == OK );
        CU_ASSERT_TRUE( linkedlist_first(list, (void **)&item) == OK );
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
        CU_ASSERT_TRUE( linkedlist_last(list, (void **)&item) == OK );
        CU_ASSERT_TRUE( strcmp(item, array[0]) == 0 );
    }

    for (i = 0, j = LEN - 1; i < LEN; i++, j--) {
        CU_ASSERT_TRUE(linkedlist_get(list, i, (void **)&item) == OK);
        CU_ASSERT_TRUE( strcmp(item, array[j]) == 0 );
    }

    linkedlist_destroy(list, NULL);

    CU_PASS("testAddFirst() - Test Passed");
}

static void testAddLast() {

    LinkedList *list;
    Status stat;
    int i;
    char *item;

    stat = linkedlist_new(&list);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testAddLast() - allocation failure");

    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE( linkedlist_addLast(list, array[i]) == OK );
        CU_ASSERT_TRUE( linkedlist_first(list, (void **)&item) == OK );
        CU_ASSERT_TRUE( strcmp(item, array[0]) == 0 );
        CU_ASSERT_TRUE( linkedlist_last(list, (void **)&item) == OK );
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
    }

    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE( linkedlist_get(list, i, (void **)&item) == OK );
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
    }

    linkedlist_destroy(list, NULL);

    CU_PASS("testAddLast() - Test Passed");
}

static void testInsertions() {

    LinkedList *list;
    Status stat;
    int i;
    char *item;

    stat = linkedlist_new(&list);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testInsertions() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( linkedlist_addLast(list, array[i]) == OK );
    for (i = 0L; i < LEN; i++) {
        CU_ASSERT_TRUE( linkedlist_get(list, i, (void **)&item) == OK);
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
    }

    (void)linkedlist_insert(list, 0L, singleItem);
    (void)linkedlist_insert(list, 3L, singleItem);
    (void)linkedlist_insert(list, 6L, singleItem);
    CU_ASSERT_TRUE( linkedlist_get(list, 0L, (void **)&item) == OK );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( linkedlist_get(list, 3L, (void **)&item) == OK );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( linkedlist_get(list, 6L, (void **)&item) == OK );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );

    linkedlist_destroy(list, NULL);

    CU_PASS("testAdditions() - Test Passed");
}

static void testSetItem() {

    LinkedList *list;
    Status stat;
    int i;
    char *item;

    stat = linkedlist_new(&list);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testSetItem() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( linkedlist_addLast(list, array[i]) == OK );

    CU_ASSERT_TRUE( linkedlist_set(list, 0L, singleItem, (void **)&item) == OK );
    CU_ASSERT_TRUE( strcmp(item, array[0]) == 0 );
    CU_ASSERT_TRUE( linkedlist_set(list, 4L, singleItem, (void **)&item) == OK );
    CU_ASSERT_TRUE( strcmp(item, array[4]) == 0 );
    CU_ASSERT_TRUE( linkedlist_set(list, 7L, singleItem, (void **)&item) == OK );
    CU_ASSERT_TRUE( strcmp(item, array[7]) == 0 );

    CU_ASSERT_TRUE( linkedlist_get(list, 0L, (void **)&item) == OK );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( linkedlist_get(list, 4L, (void **)&item) == OK );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( linkedlist_get(list, 7L, (void **)&item) == OK );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );

    linkedlist_destroy(list, NULL);

    CU_PASS("testSetItem() - Test Passed");
}

static void testSequentialDelete() {

    LinkedList *list;
    Status stat;
    int i;
    char *item;

    stat = linkedlist_new(&list);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testSequentialDelete() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( linkedlist_addLast(list, array[i]) == OK );

    for (i = 0L; i < LEN; i++) {
        CU_ASSERT_TRUE( linkedlist_remove(list, 0L, (void **)&item) == OK );
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
    }

    linkedlist_destroy(list, NULL);

    CU_PASS("testSequentialDelete() - Test Passed");
}

static void testRandomDelete() {

    LinkedList *list;
    Status stat;
    int i;
    char *item;

    stat = linkedlist_new(&list);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testRandomDelete() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( linkedlist_addLast(list, array[i]) == OK );

    CU_ASSERT_TRUE( linkedlist_remove(list, 7L, (void **)&item) == OK );
    CU_ASSERT_TRUE( strcmp(item, array[7]) == 0 );
    CU_ASSERT_TRUE( linkedlist_remove(list, 5L, (void **)&item) == OK );
    CU_ASSERT_TRUE( strcmp(item, array[5]) == 0 );
    CU_ASSERT_TRUE( linkedlist_remove(list, 1L, (void **)&item) == OK );
    CU_ASSERT_TRUE( strcmp(item, array[1]) == 0 );

    linkedlist_destroy(list, NULL);

    CU_PASS("testRandomDelete() - Test Passed");
}

static void testInvalidIndexAccess() {

    LinkedList *list;
    Status stat;
    char *item;
    int i;

    stat = linkedlist_new(&list);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testInvalidIndexAccess() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( linkedlist_addLast(list, array[i]) == OK );

    CU_ASSERT_TRUE( linkedlist_insert(list, LEN + 1L, singleItem) == INVALID_INDEX );
    CU_ASSERT_TRUE( linkedlist_insert(list, LEN + 1L, singleItem) == INVALID_INDEX );
    CU_ASSERT_TRUE( linkedlist_insert(list, LEN + 10L, singleItem) == INVALID_INDEX );
    CU_ASSERT_TRUE( linkedlist_get(list, -1, (void **)&item) == INVALID_INDEX );
    CU_ASSERT_TRUE( linkedlist_get(list, LEN + 1L, (void **)&item) == INVALID_INDEX );
    CU_ASSERT_TRUE( linkedlist_get(list, LEN + 10L, (void **)&item) == INVALID_INDEX );
    CU_ASSERT_TRUE( linkedlist_set(list, -1, singleItem, (void **)&item) == INVALID_INDEX );
    CU_ASSERT_TRUE( linkedlist_set(list, LEN + 1L, singleItem, (void **)&item) == INVALID_INDEX );
    CU_ASSERT_TRUE( linkedlist_set(list, LEN + 10L, singleItem, (void **)&item) == INVALID_INDEX );
    CU_ASSERT_TRUE( linkedlist_remove(list, -1, (void **)&item) == INVALID_INDEX );
    CU_ASSERT_TRUE( linkedlist_remove(list, LEN + 1L, (void **)&item) == INVALID_INDEX );
    CU_ASSERT_TRUE( linkedlist_remove(list, LEN + 10L, (void **)&item) == INVALID_INDEX );

    linkedlist_destroy(list, NULL);

    CU_PASS("testInvalidIndexAccess() - Test Passed");
}

static void testLinkedListToArray() {

    LinkedList *list;
    Array *arr;
    Status stat;
    int i;

    stat = linkedlist_new(&list);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testLinkedListToArray() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( linkedlist_addLast(list, array[i]) == OK );
    CU_ASSERT_TRUE( linkedlist_toArray(list, &arr) == OK);
    CU_ASSERT_TRUE( arr->len == LEN );

    for (i = 0; i < arr->len; i++)
        CU_ASSERT_TRUE( strcmp(arr->items[i], array[i]) == 0 );

    FREE_ARRAY(arr)
    linkedlist_destroy(list, NULL);

    CU_PASS("testLinkedListToArray() - Test Passed");
}

static void testLinkedListIterator() {

    LinkedList *list;
    Iterator *iter;
    Status stat;
    char *item;
    int i;

    stat = linkedlist_new(&list);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testLinkedListIterator() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( linkedlist_addLast(list, array[i]) == OK );
    CU_ASSERT_TRUE( linkedlist_iterator(list, &iter) == OK );

    i = 0;
    while (iterator_hasNext(iter) == TRUE) {
        CU_ASSERT_TRUE( iterator_next(iter, (void **)&item) == OK );
        CU_ASSERT_TRUE( strcmp(item, array[i++]) == 0 );
    }

    iterator_destroy(iter);
    linkedlist_destroy(list, NULL);

    CU_PASS("testLinkedListIterator() - Test Passed");
}

static void testLinkedListClear() {

    LinkedList *list;
    Status stat;
    int i;

    stat = linkedlist_new(&list);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testLinkedListClear() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( linkedlist_addLast(list, array[i]) == OK );
    linkedlist_clear(list, NULL);
    validateEmptyLinkedList(list);
    linkedlist_destroy(list, NULL);

    CU_PASS("testLinkedListClear() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite("LinkedList Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "LinkedList - Empty", testEmptyLinkedList);
    CU_add_test(suite, "LinkedList - Single Item", testSingleItem);
    CU_add_test(suite, "LinkedList - Invalid Index Access", testInvalidIndexAccess);
    CU_add_test(suite, "LinkedList - Head Insertions", testAddFirst);
    CU_add_test(suite, "LinkedList - Tail Insertions", testAddLast);
    CU_add_test(suite, "LinkedList - Random Insertions", testInsertions);
    CU_add_test(suite, "LinkedList - Set", testSetItem);
    CU_add_test(suite, "LinkedList - Sequential Delete", testSequentialDelete);
    CU_add_test(suite, "LinkedList - Random Delete", testRandomDelete);
    CU_add_test(suite, "LinkedList - Array", testLinkedListToArray);
    CU_add_test(suite, "LinkedList - Iterator", testLinkedListIterator);
    CU_add_test(suite, "LinkedList - Clear", testLinkedListClear);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
