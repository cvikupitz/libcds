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

#define CAPACITY 2L

static char *singleItem = "Test";
#define LEN 9
static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple", "gray", "white", "black"};
static char *orderedArray[] = {"black", "blue", "gray", "green", "orange", "purple", "red", "white", "yellow"};

static int heapCmp(void *x, void *y) {
    return strcmp((char *)x, (char *)y);
}

static void validateEmptyHeap(Heap *heap) {

    Status stat;
    int *element;
    
    stat = heap_peek(heap, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    stat = heap_poll(heap, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    long size = heap_size(heap);
    CU_ASSERT_EQUAL(size, 0L);

    Boolean isEmpty = heap_isEmpty(heap);
    CU_ASSERT_EQUAL(isEmpty, TRUE);
   
    Iterator *iter;
    stat = heap_iterator(heap, &iter);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    int **array;
    long len;
    stat = heap_toArray(heap, (void ***)&array, &len);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
}

void testEmptyHeap() {

    Heap *heap = NULL;
    Status stat;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyHeap() - allocation failure");
    
    validateEmptyHeap(heap);
    heap_destroy(heap, NULL);
    CU_PASS("testEmptyHeap() - Test Passed");
}

void testSingleItem() {
    
    Heap *heap;
    Status stat;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    stat = heap_insert(heap, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    long size = heap_size(heap);
    Boolean isEmpty = heap_isEmpty(heap);
    CU_ASSERT_EQUAL(size, 1L);
    CU_ASSERT_EQUAL(isEmpty, FALSE);

    char *element;
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

void testUnorderedSet() {

    Heap *heap;
    Status stat;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testUnorderedSet() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = heap_insert(heap, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char *element;
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

void testOrderedSet() {

    Heap *heap;
    Status stat;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testOrderedSet() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = heap_insert(heap, orderedArray[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char *element;
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

void testHeapClear() {
    
    Heap *heap;
    Status stat;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testHeapClear() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = heap_insert(heap, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    heap_clear(heap, NULL);
    validateEmptyHeap(heap);
    heap_destroy(heap, NULL);

    CU_PASS("testHeapClear() - Test Passed");
}

void testHeapToArray() {

    Heap *heap;
    Status stat;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testHeapToArray() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = heap_insert(heap, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char **items;
    long len;
    stat = heap_toArray(heap, (void ***)&items, &len);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(len, LEN);

    free(items);
    heap_destroy(heap, NULL);
    CU_PASS("testHeapToArray() - Test Passed");
}

void testHeapIterator() {

    Heap *heap;
    Status stat;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testHeapIterator() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = heap_insert(heap, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    Iterator *iter;
    stat = heap_iterator(heap, &iter);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    iterator_destroy(iter);
    heap_destroy(heap, NULL);
    CU_PASS("testHeapIterator() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Heap Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "Empty heap", testEmptyHeap);
    CU_add_test(suite, "Single item", testSingleItem);
    CU_add_test(suite, "Unordered set", testUnorderedSet);
    CU_add_test(suite, "Ordered set", testOrderedSet);
    CU_add_test(suite, "Heap toArray", testHeapToArray);
    CU_add_test(suite, "Heap iterator", testHeapIterator);
    CU_add_test(suite, "Heap clear", testHeapClear);
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
