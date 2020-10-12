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
#include "deque.h"

static void validateEmptyDeque(Deque *deque) {

    Status stat;
    char *element;
    
    stat = deque_first(deque, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = deque_last(deque, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = deque_removeFirst(deque, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = deque_removeLast(deque, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    long size = deque_size(deque);
    CU_ASSERT_EQUAL(size, 0L);

    Boolean isEmpty = deque_isEmpty(deque);
    CU_ASSERT_EQUAL(isEmpty, TRUE);
    
    Iterator *iter;
    stat = deque_iterator(deque, &iter);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    int **array;
    long len;
    stat = deque_toArray(deque, (void ***)&array, &len);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
}

void testEmptyDeque() {

    Deque *deque = NULL;
    Status stat;

    stat = deque_new(&deque);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyDeque() - allocation failure");
    
    validateEmptyDeque(deque);
    deque_destroy(deque, NULL);
    CU_PASS("testEmptyDeque() - Test Passed");
}

static char *singleItem = "Test";
#define LEN 6
static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple"};

static void validateSingleItem(Deque *deque) {

    Status stat;

    long size = deque_size(deque);
    Boolean isEmpty = deque_isEmpty(deque);
    CU_ASSERT_EQUAL(size, 1L);
    CU_ASSERT_EQUAL(isEmpty, FALSE);

    char *element;
    stat = deque_first(deque, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
    stat = deque_last(deque, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
}

void testSingleItemFromFirst() {
    
    Deque *deque;
    Status stat;
    char *element;

    stat = deque_new(&deque);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItemFromFirst() - allocation failure");

    stat = deque_addFirst(deque, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    validateSingleItem(deque);
    stat = deque_removeFirst(deque, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
    stat = deque_removeFirst(deque, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    validateEmptyDeque(deque);
    deque_destroy(deque, NULL);

    CU_PASS("testSingleItemFromFirst() - Test Passed");
}

void testSingleItemFromLast() {
    
    Deque *deque;
    Status stat;
    char *element;

    stat = deque_new(&deque);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItemFromLast() - allocation failure");

    stat = deque_addLast(deque, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    validateSingleItem(deque);
    stat = deque_removeLast(deque, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
    stat = deque_removeLast(deque, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    validateEmptyDeque(deque);
    deque_destroy(deque, NULL);

    CU_PASS("testSingleItemFromLast() - Test Passed");
}

void testEndequeDedequeFromFirst() {
    
    Deque *deque;
    Status stat;

    stat = deque_new(&deque);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEndequeDedequeFromFirst() - allocation failure");

    int i;
    long size;
    Boolean isEmpty;
    char *element;
    for (i = 0; i < LEN; i++) {
        stat = deque_addFirst(deque, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        size = deque_size(deque);
        isEmpty = deque_isEmpty(deque);
        CU_ASSERT_EQUAL(size, i + 1);
        CU_ASSERT_EQUAL(isEmpty, FALSE);
        stat = deque_first(deque, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
        stat = deque_last(deque, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[0]) == 0 );
    }

    for (i = LEN - 1; i >= 0; i--) {
        stat = deque_removeFirst(deque, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
        size = deque_size(deque);
        isEmpty = deque_isEmpty(deque);
        CU_ASSERT_EQUAL(size, i);
        CU_ASSERT_EQUAL( isEmpty, (size != 0L) ? FALSE : TRUE );
    }

    stat = deque_removeFirst(deque, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    deque_destroy(deque, NULL);

    CU_PASS("testEndequeDedequeFromFirst() - Test Passed");
}

void testEndequeDedequeFromLast() {
    
    Deque *deque;
    Status stat;

    stat = deque_new(&deque);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEndequeDedequeFromLast() - allocation failure");

    int i;
    long size;
    Boolean isEmpty;
    char *element;
    for (i = 0; i < LEN; i++) {
        stat = deque_addLast(deque, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        size = deque_size(deque);
        isEmpty = deque_isEmpty(deque);
        CU_ASSERT_EQUAL(size, i + 1);
        CU_ASSERT_EQUAL(isEmpty, FALSE);
        stat = deque_first(deque, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[0]) == 0 );
        stat = deque_last(deque, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
    }

    for (i = LEN - 1; i >= 0; i--) {
        stat = deque_removeLast(deque, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
        size = deque_size(deque);
        isEmpty = deque_isEmpty(deque);
        CU_ASSERT_EQUAL(size, i);
        CU_ASSERT_EQUAL( isEmpty, (size != 0L) ? FALSE : TRUE );
    }

    stat = deque_removeLast(deque, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    deque_destroy(deque, NULL);

    CU_PASS("testEndequeDedequeFromLast() - Test Passed");
}

void testDequeToArray() {

    Deque *deque;
    Status stat;

    stat = deque_new(&deque);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testDequeToArray() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = deque_addLast(deque, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char **items;
    long len;
    stat = deque_toArray(deque, (void ***)&items, &len);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(len, LEN);

    for (i = 0; i < len; i++)
        CU_ASSERT_TRUE( strcmp(items[i], array[i]) == 0 );

    free(items);
    deque_destroy(deque, NULL);
    CU_PASS("testDequeToArray() - Test Passed");
}

void testDequeIterator() {

    Deque *deque;
    Status stat;

    stat = deque_new(&deque);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testDequeIterator() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = deque_addLast(deque, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    Iterator *iter;
    stat = deque_iterator(deque, &iter);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    char *element;
    i = 0;
    while (iterator_hasNext(iter) == TRUE) {
        stat = iterator_next(iter, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i++]) == 0 );
    }

    iterator_destroy(iter);
    deque_destroy(deque, NULL);
    CU_PASS("testDequeIterator() - Test Passed");
}

void testDequeClear() {
    
    Deque *deque;
    Status stat;

    stat = deque_new(&deque);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testDequeClear() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = deque_addFirst(deque, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    deque_clear(deque, NULL);
    validateEmptyDeque(deque);
    deque_destroy(deque, NULL);

    CU_PASS("testDequeClear() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Deque Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "Empty deque", testEmptyDeque);
    CU_add_test(suite, "Single item from first", testSingleItemFromFirst);
    CU_add_test(suite, "Single item from last", testSingleItemFromLast);
    CU_add_test(suite, "Deque insert & delete from head", testEndequeDedequeFromFirst);
    CU_add_test(suite, "Deque insert & delete from tail", testEndequeDedequeFromLast);
    CU_add_test(suite, "Deque toArray", testDequeToArray);
    CU_add_test(suite, "Deque iterator", testDequeIterator);
    CU_add_test(suite, "Deque clear", testDequeClear);
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}