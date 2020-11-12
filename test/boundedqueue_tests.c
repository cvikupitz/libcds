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
#include "boundedqueue.h"

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
    Status stat;
    Boolean boolean;
    char *element;
    long size;

    stat = boundedqueue_peek(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = boundedqueue_poll(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    size = boundedqueue_size(queue);
    CU_ASSERT_EQUAL(size, 0L);
    boolean = boundedqueue_isEmpty(queue);
    CU_ASSERT_EQUAL(boolean, TRUE);
    boolean = boundedqueue_isFull(queue);
    CU_ASSERT_EQUAL(boolean, FALSE);
    stat = boundedqueue_iterator(queue, &iter);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = boundedqueue_toArray(queue, &arr);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
}

static void testEmptyBoundedQueue() {

    BoundedQueue *queue;
    Status stat;

    stat = boundedqueue_new(&queue, 0L);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyBoundedQueue() - allocation failure");

    validateEmptyBoundedQueue(queue);
    boundedqueue_destroy(queue, NULL);

    CU_PASS("testEmptyBoundedQueue() - Test Passed");
}

static void testSingleItem() {

    BoundedQueue *queue;
    Status stat;
    Boolean boolean;
    long size, capacity;
    char *element;

    stat = boundedqueue_new(&queue, 1L);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    stat = boundedqueue_add(queue, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    size = boundedqueue_size(queue);
    CU_ASSERT_EQUAL(size, 1L);
    capacity = boundedqueue_capacity(queue);
    CU_ASSERT_EQUAL(capacity, 1L);
    boolean = boundedqueue_isEmpty(queue);
    CU_ASSERT_EQUAL(boolean, FALSE);
    boolean = boundedqueue_isFull(queue);
    CU_ASSERT_EQUAL(boolean, TRUE);

    stat = boundedqueue_peek(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    stat = boundedqueue_poll(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
    stat = boundedqueue_poll(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    validateEmptyBoundedQueue(queue);
    boundedqueue_destroy(queue, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

static void testAddPoll() {

    BoundedQueue *queue;
    Status stat;
    Boolean boolean;
    int i;
    long size;
    char *element;

    stat = boundedqueue_new(&queue, 0L);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testAddPoll() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = boundedqueue_add(queue, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        size = boundedqueue_size(queue);
        boolean = boundedqueue_isEmpty(queue);
        CU_ASSERT_EQUAL(size, i + 1);
        CU_ASSERT_EQUAL(boolean, FALSE);
        boolean = boundedqueue_isFull(queue);
        CU_ASSERT_EQUAL(boolean, FALSE);
        stat = boundedqueue_peek(queue, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[0]) == 0 );
    }

    for (i = 0; i < LEN; i++) {
        stat = boundedqueue_poll(queue, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
        size = boundedqueue_size(queue);
        boolean = boundedqueue_isEmpty(queue);
        CU_ASSERT_EQUAL(boolean, (i == LEN - 1) ? TRUE : FALSE);
        CU_ASSERT_EQUAL(size, (LEN - (i + 1)));
        boolean = boundedqueue_isFull(queue);
        CU_ASSERT_EQUAL(boolean, FALSE);
    }

    boundedqueue_destroy(queue, NULL);

    CU_PASS("testPushPop() - Test Passed");
}

static void testCapacity() {

    BoundedQueue *queue;
    Boolean boolean;
    Status stat;
    char *element;
    int i;
    long size;

    stat = boundedqueue_new(&queue, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testCapacity() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = boundedqueue_add(queue, array[i]);
        CU_ASSERT_EQUAL(stat, i < CAPACITY ? STAT_SUCCESS : STAT_STRUCT_FULL);
        size = boundedqueue_size(queue);
        boolean = boundedqueue_isEmpty(queue);
        CU_ASSERT_EQUAL(size, (i < CAPACITY) ? (i + 1) : CAPACITY);
        CU_ASSERT_EQUAL(boolean, FALSE);
        boolean = boundedqueue_isFull(queue);
        CU_ASSERT_EQUAL(boolean, (size < CAPACITY) ? FALSE : TRUE);
        stat = boundedqueue_peek(queue, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[0]) == 0 );
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
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testBoundedQueueToArray() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = boundedqueue_add(queue, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    stat = boundedqueue_toArray(queue, &arr);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(arr->len, LEN);

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
    char *element;

    stat = boundedqueue_new(&queue, 0L);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testBoundedQueueIterator() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = boundedqueue_add(queue, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    stat = boundedqueue_iterator(queue, &iter);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    i = 0;
    while (iterator_hasNext(iter) == TRUE) {
        stat = iterator_next(iter, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i++]) == 0 );
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
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testBoundedQueueClear() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = boundedqueue_add(queue, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

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
