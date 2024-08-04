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
#include "bounded_queue.h"

/* Single item used for testing */
static char *singleItem = "Test";

/* Collection of items used for testing */
#define LEN 6
static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple"};

/* Capacity for bounded queue */
#define CAPACITY 4L

static void validateEmptyBoundedQueue(BoundedQueue *queue) {

    Array *arr;
    Iterator *iter;
    char *item;

    CU_ASSERT_TRUE( boundedqueue_peek(queue, (void **)&item) == STRUCT_EMPTY );
    CU_ASSERT_TRUE( boundedqueue_poll(queue, (void **)&item) == STRUCT_EMPTY );
    CU_ASSERT_TRUE( boundedqueue_size(queue) == 0L );
    CU_ASSERT_TRUE( boundedqueue_isEmpty(queue) == TRUE );
    CU_ASSERT_TRUE( boundedqueue_isFull(queue) == FALSE );
    CU_ASSERT_TRUE( boundedqueue_iterator(queue, &iter) == STRUCT_EMPTY );
    CU_ASSERT_TRUE( boundedqueue_toArray(queue, &arr) == STRUCT_EMPTY );
}

static void testEmptyBoundedQueue() {

    BoundedQueue *queue;
    Status stat;

    stat = boundedqueue_new(&queue, 0L);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testEmptyBoundedQueue() - allocation failure");

    validateEmptyBoundedQueue(queue);
    boundedqueue_destroy(queue, NULL);

    CU_PASS("testEmptyBoundedQueue() - Test Passed");
}

static void testSingleItem() {

    BoundedQueue *queue;
    Status stat;
    char *item;

    stat = boundedqueue_new(&queue, 1L);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    CU_ASSERT_TRUE( boundedqueue_add(queue, singleItem) == OK );
    CU_ASSERT_TRUE( boundedqueue_size(queue) == 1L );
    CU_ASSERT_TRUE( boundedqueue_capacity(queue) == 1L );
    CU_ASSERT_TRUE( boundedqueue_isEmpty(queue) == FALSE );
    CU_ASSERT_TRUE( boundedqueue_isFull(queue) == TRUE );
    CU_ASSERT_TRUE( boundedqueue_peek(queue, (void **)&item) == OK );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( boundedqueue_poll(queue, (void **)&item) == OK );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( boundedqueue_poll(queue, (void **)&item) == STRUCT_EMPTY );

    validateEmptyBoundedQueue(queue);
    boundedqueue_destroy(queue, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

static void testAddPoll() {

    BoundedQueue *queue;
    Status stat;
    int i;
    char *item;

    stat = boundedqueue_new(&queue, 0L);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testAddPoll() - allocation failure");

    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE( boundedqueue_add(queue, array[i]) == OK );
        CU_ASSERT_TRUE( boundedqueue_size(queue) == (i + 1) );
        CU_ASSERT_TRUE( boundedqueue_isEmpty(queue) == FALSE );
        CU_ASSERT_TRUE( boundedqueue_isFull(queue) == FALSE );
        CU_ASSERT_TRUE( boundedqueue_peek(queue, (void **)&item) == OK );
        CU_ASSERT_TRUE( strcmp(item, array[0]) == 0 );
    }

    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE( boundedqueue_poll(queue, (void **)&item) == OK );
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
        CU_ASSERT_TRUE( boundedqueue_isEmpty(queue) == ((i == LEN - 1) ? TRUE : FALSE) );
        CU_ASSERT_TRUE( boundedqueue_size(queue) == (LEN - (i + 1)) );
        CU_ASSERT_TRUE( boundedqueue_isFull(queue) == FALSE );
    }

    boundedqueue_destroy(queue, NULL);

    CU_PASS("testPushPop() - Test Passed");
}

static void testCapacity() {

    BoundedQueue *queue;
    Status stat;
    char *item;
    int i;
    long size;

    stat = boundedqueue_new(&queue, CAPACITY);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testCapacity() - allocation failure");

    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE( boundedqueue_add(queue, array[i]) == (i < CAPACITY ? OK : STRUCT_FULL) );
        size = boundedqueue_size(queue);
        CU_ASSERT_TRUE( size == (i < CAPACITY) ? (i + 1) : CAPACITY );
        CU_ASSERT_TRUE( boundedqueue_isEmpty(queue) == FALSE );
        CU_ASSERT_TRUE( boundedqueue_isFull(queue) == (size < CAPACITY ? FALSE : TRUE) );
        CU_ASSERT_TRUE( boundedqueue_peek(queue, (void **)&item) == OK );
        CU_ASSERT_TRUE( strcmp(item, array[0]) == 0 );
    }

    boundedqueue_destroy(queue, NULL);

    CU_PASS("testCapacity() - Test Passed");
}

static void testBoundedQueueToArray() {

    BoundedQueue *queue;
    Array *arr;
    Status stat;
    int i;

    stat = boundedqueue_new(&queue, 0L);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testBoundedQueueToArray() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( boundedqueue_add(queue, array[i]) == OK );
    CU_ASSERT_TRUE( boundedqueue_toArray(queue, &arr) == OK );
    CU_ASSERT_TRUE( arr->len == LEN );

    for (i = 0; i < arr->len; i++)
        CU_ASSERT_TRUE( strcmp(arr->items[i], array[i]) == 0 );

    FREE_ARRAY(arr)
    boundedqueue_destroy(queue, NULL);

    CU_PASS("testBoundedQueueToArray() - Test Passed");
}

static void testBoundedQueueIterator() {

    Iterator *iter;
    BoundedQueue *queue;
    Status stat;
    int i;
    char *item;

    stat = boundedqueue_new(&queue, 0L);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testBoundedQueueIterator() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( boundedqueue_add(queue, array[i]) == OK );
    CU_ASSERT_TRUE( boundedqueue_iterator(queue, &iter) == OK );

    i = 0;
    while (iterator_hasNext(iter) == TRUE) {
        CU_ASSERT_TRUE( iterator_next(iter, (void **)&item) == OK );
        CU_ASSERT_TRUE( strcmp(item, array[i++]) == 0 );
    }

    iterator_destroy(iter);
    boundedqueue_destroy(queue, NULL);

    CU_PASS("testBoundedQueueIterator() - Test Passed");
}

static void testBoundedQueueClear() {

    BoundedQueue *queue;
    Status stat;
    int i;

    stat = boundedqueue_new(&queue, 0L);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testBoundedQueueClear() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( boundedqueue_add(queue, array[i]) == OK );

    boundedqueue_clear(queue, NULL);
    validateEmptyBoundedQueue(queue);
    boundedqueue_destroy(queue, NULL);

    CU_PASS("testBoundedQueueClear() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite("BoundedQueue Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "BoundedQueue - Empty BoundedQueue", testEmptyBoundedQueue);
    CU_add_test(suite, "BoundedQueue - Single Item", testSingleItem);
    CU_add_test(suite, "BoundedQueue - Add & Poll", testAddPoll);
    CU_add_test(suite, "BoundedQueue - Capacity Check", testCapacity);
    CU_add_test(suite, "BoundedQueue - Array", testBoundedQueueToArray);
    CU_add_test(suite, "BoundedQueue - Iterator", testBoundedQueueIterator);
    CU_add_test(suite, "BoundedQueue - Clear", testBoundedQueueClear);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
