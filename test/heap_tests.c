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

    Array *arr;
    Iterator *iter;
    char *item;

    CU_ASSERT_TRUE( heap_peek(heap, (void **)&item) == STRUCT_EMPTY );
    CU_ASSERT_TRUE( heap_poll(heap, (void **)&item) == STRUCT_EMPTY );
    CU_ASSERT_TRUE( heap_size(heap) == 0L );
    CU_ASSERT_TRUE( heap_isEmpty(heap) == TRUE );
    CU_ASSERT_TRUE( heap_iterator(heap, &iter) == STRUCT_EMPTY );
    CU_ASSERT_TRUE( heap_toArray(heap, &arr) == STRUCT_EMPTY );
}

static void testEmptyHeap() {

    Heap *heap;
    Status stat;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testEmptyHeap() - allocation failure");

    validateEmptyHeap(heap);
    heap_destroy(heap, NULL);

    CU_PASS("testEmptyHeap() - Test Passed");
}

static void testSingleItem() {

    Heap *heap;
    Status stat;
    char *item;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    CU_ASSERT_TRUE( heap_insert(heap, singleItem) == OK );
    CU_ASSERT_TRUE( heap_size(heap) == 1L );
    CU_ASSERT_TRUE( heap_isEmpty(heap) == FALSE );
    CU_ASSERT_TRUE( heap_peek(heap, (void **)&item) == OK );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( heap_poll(heap, (void **)&item) == OK );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( heap_poll(heap, (void **)&item) == STRUCT_EMPTY );

    validateEmptyHeap(heap);
    heap_destroy(heap, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

static void testUnorderedSet() {

    Heap *heap;
    Status stat;
    char *item;
    int i;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testUnorderedSet() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE(heap_insert(heap, array[i]) == OK);

    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE( heap_peek(heap, (void **)&item) == OK );
        CU_ASSERT_TRUE( strcmp(item, orderedArray[i]) == 0 );
        CU_ASSERT_TRUE( heap_poll(heap, (void **)&item) == OK );
        CU_ASSERT_TRUE( strcmp(item, orderedArray[i]) == 0 );
    }

    heap_destroy(heap, NULL);

    CU_PASS("testUnorderedSet() - Test Passed");
}

static void testOrderedSet() {

    Heap *heap;
    Status stat;
    char *item;
    int i;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testOrderedSet() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( heap_insert(heap, orderedArray[i]) == OK );

    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE( heap_peek(heap, (void **)&item) == OK );
        CU_ASSERT_TRUE( strcmp(item, orderedArray[i]) == 0 );
        CU_ASSERT_TRUE( heap_poll(heap, (void **)&item) == OK );
        CU_ASSERT_TRUE( strcmp(item, orderedArray[i]) == 0 );
    }

    heap_destroy(heap, NULL);

    CU_PASS("testOrderedSet() - Test Passed");
}

static void testHeapClear() {

    Heap *heap;
    Status stat;
    int i;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testHeapClear() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( heap_insert(heap, array[i]) == OK );

    heap_clear(heap, NULL);
    validateEmptyHeap(heap);
    heap_destroy(heap, NULL);

    CU_PASS("testHeapClear() - Test Passed");
}

static void testHeapToArray() {

    Heap *heap;
    Array *arr;
    Status stat;
    int i;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testHeapToArray() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( heap_insert(heap, array[i]) == OK );
    CU_ASSERT_TRUE( heap_toArray(heap, &arr) == OK );
    CU_ASSERT_TRUE( arr->len == LEN );

    FREE_ARRAY(arr)
    heap_destroy(heap, NULL);

    CU_PASS("testHeapToArray() - Test Passed");
}

static void testHeapIterator() {

    Heap *heap;
    Iterator *iter;
    Status stat;
    int i;

    stat = heap_new(&heap, CAPACITY, heapCmp);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testHeapIterator() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( heap_insert(heap, array[i]) == OK );
    CU_ASSERT_TRUE( heap_iterator(heap, &iter) == OK );

    iterator_destroy(iter);
    heap_destroy(heap, NULL);

    CU_PASS("testHeapIterator() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite("Heap Tests", NULL, NULL);
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
