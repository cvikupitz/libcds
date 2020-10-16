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
#include "heap.h"

/* DEfault capacity for the heap */
#define CAPACITY 2L

/* Single item used for testing */
static char *singleItem = "Test";

/* Collection of unordered items used for testing */
#define LEN 9
static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple", "gray", "white", "black"};

/* Collection of ordered items used for testing */
static char *orderedArray[] = {"black", "blue", "gray", "green", "orange", "purple", "red", "white", "yellow"};

/* Comparator function used for comparing items on the heap */
static int heapCmp(void *x, void *y) {
    return strcmp((char *)x, (char *)y);
}

static void validateEmptyHeap(Heap *heap) {

    Iterator *iter;
    Status stat;
    Boolean isEmpty;
    char *element, **array;
    long size, len;

    stat = heap_peek(heap, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = heap_poll(heap, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    size = heap_size(heap);
    CU_ASSERT_EQUAL(size, 0L);
    isEmpty = heap_isEmpty(heap);
    CU_ASSERT_EQUAL(isEmpty, TRUE);
    stat = heap_iterator(heap, &iter);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = heap_toArray(heap, (void ***)&array, &len);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
}

static void testEmptyHeap() {

    Heap *heap;
    Status stat;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyHeap() - allocation failure");

    validateEmptyHeap(heap);
    heap_destroy(heap, NULL);

    CU_PASS("testEmptyHeap() - Test Passed");
}

static void testSingleItem() {

    Heap *heap;
    Status stat;
    Boolean isEmpty;
    char *element;
    long size;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    stat = heap_insert(heap, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    size = heap_size(heap);
    isEmpty = heap_isEmpty(heap);
    CU_ASSERT_EQUAL(size, 1L);
    CU_ASSERT_EQUAL(isEmpty, FALSE);

    stat = heap_peek(heap, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    stat = heap_poll(heap, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
    stat = heap_poll(heap, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    validateEmptyHeap(heap);
    heap_destroy(heap, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

static void testUnorderedSet() {

    Heap *heap;
    Status stat;
    char *element;
    int i;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testUnorderedSet() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = heap_insert(heap, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    for (i = 0; i < LEN; i++) {
        stat = heap_peek(heap, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, orderedArray[i]) == 0);
        stat = heap_poll(heap, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, orderedArray[i]) == 0);
    }

    heap_destroy(heap, NULL);

    CU_PASS("testUnorderedSet() - Test Passed");
}

static void testOrderedSet() {

    Heap *heap;
    Status stat;
    char *element;
    int i;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testOrderedSet() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = heap_insert(heap, orderedArray[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    for (i = 0; i < LEN; i++) {
        stat = heap_peek(heap, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, orderedArray[i]) == 0);
        stat = heap_poll(heap, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, orderedArray[i]) == 0);
    }

    heap_destroy(heap, NULL);

    CU_PASS("testOrderedSet() - Test Passed");
}

static void testHeapClear() {

    Heap *heap;
    Status stat;
    int i;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testHeapClear() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = heap_insert(heap, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    heap_clear(heap, NULL);
    validateEmptyHeap(heap);
    heap_destroy(heap, NULL);

    CU_PASS("testHeapClear() - Test Passed");
}

static void testHeapToArray() {

    Heap *heap;
    Status stat;
    char **items;
    int i;
    long len;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testHeapToArray() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = heap_insert(heap, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    stat = heap_toArray(heap, (void ***)&items, &len);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(len, LEN);

    free(items);
    heap_destroy(heap, NULL);
    CU_PASS("testHeapToArray() - Test Passed");
}

static void testHeapIterator() {

    Heap *heap;
    Iterator *iter;
    Status stat;
    int i;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testHeapIterator() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = heap_insert(heap, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    stat = heap_iterator(heap, &iter);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    iterator_destroy(iter);
    heap_destroy(heap, NULL);

    CU_PASS("testHeapIterator() - Test Passed");
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

    CU_add_test(suite, "Heap - Empty", testEmptyHeap);
    CU_add_test(suite, "Heap - Single Item", testSingleItem);
    CU_add_test(suite, "Heap - Unordered Set", testUnorderedSet);
    CU_add_test(suite, "Heap - Ordered Set", testOrderedSet);
    CU_add_test(suite, "Heap - Array", testHeapToArray);
    CU_add_test(suite, "Heap - Iterator", testHeapIterator);
    CU_add_test(suite, "Heap - Clear", testHeapClear);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
