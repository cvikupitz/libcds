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

/* Default capacity for the bounded queues */
#define CAPACITY 5L

/* Single item used for testing */
static char *singleItem = "Test";

/* Collection of items used for testing */
#define LEN 9
static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple", "gray", "white", "black"};

static void validateEmptyBoundedQueue(BoundedQueue *queue) {

    Status stat;
    Boolean isEmpty;
    Iterator *iter;
    char *element, **array;
    long size, len;

    stat = boundedqueue_peek(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = boundedqueue_dequeue(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    size = boundedqueue_size(queue);
    CU_ASSERT_EQUAL(size, 0L);
    isEmpty = boundedqueue_isEmpty(queue);
    CU_ASSERT_EQUAL(isEmpty, TRUE);
    stat = boundedqueue_iterator(queue, &iter);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = boundedqueue_toArray(queue, (void ***)&array, &len);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
}

static void testEmptyBoundedQueue() {

    BoundedQueue *queue;
    Status stat;

    stat = boundedqueue_new(&queue, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyBoundedQueue() - allocation failure");

    validateEmptyBoundedQueue(queue);
    boundedqueue_destroy(queue, NULL);

    CU_PASS("testEmptyBoundedQueue() - Test Passed");
}

static void testSingleItem() {

    BoundedQueue *queue;
    Status stat;
    Boolean isEmpty;
    long size;
    char *element;

    stat = boundedqueue_new(&queue, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    stat = boundedqueue_enqueue(queue, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    size = boundedqueue_size(queue);
    isEmpty = boundedqueue_isEmpty(queue);
    CU_ASSERT_EQUAL(size, 1L);
    CU_ASSERT_EQUAL(isEmpty, FALSE);

    stat = boundedqueue_peek(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    stat = boundedqueue_dequeue(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
    stat = boundedqueue_dequeue(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    validateEmptyBoundedQueue(queue);
    boundedqueue_destroy(queue, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

static void testEnqueueDequeue() {

    BoundedQueue *queue;
    Status stat;
    Boolean isEmpty, isFull;
    int i;
    long size;
    char *element;

    stat = boundedqueue_new(&queue, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEnqueueDequeue() - allocation failure");

    for (i = 0; i < CAPACITY; i++) {
        stat = boundedqueue_enqueue(queue, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        size = boundedqueue_size(queue);
        isEmpty = boundedqueue_isEmpty(queue);
        isFull = boundedqueue_isFull(queue);
        CU_ASSERT_EQUAL(size, i + 1);
        CU_ASSERT_EQUAL(isEmpty, FALSE);
        CU_ASSERT_EQUAL(isFull, (size == CAPACITY) ? TRUE : FALSE);
        stat = boundedqueue_peek(queue, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[0]) == 0 );
    }

    for (i = CAPACITY; i < LEN; i++) {
        stat = boundedqueue_enqueue(queue, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_STRUCT_FULL);
        size = boundedqueue_size(queue);
        isEmpty = boundedqueue_isEmpty(queue);
        isFull = boundedqueue_isFull(queue);
        CU_ASSERT_EQUAL(size, CAPACITY);
        CU_ASSERT_EQUAL(isEmpty, FALSE);
        CU_ASSERT_EQUAL(isFull, TRUE);
    }

    for (i = 0; i < 0; i++) {
        stat = boundedqueue_dequeue(queue, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
        size = boundedqueue_size(queue);
        isEmpty = boundedqueue_isEmpty(queue);
        CU_ASSERT_EQUAL(size, i);
        CU_ASSERT_EQUAL( isEmpty, (size != 0L) ? FALSE : TRUE );
    }

    boundedqueue_destroy(queue, NULL);

    CU_PASS("testEnqueueDequeue() - Test Passed");
}

static void testBoundedQueueToArray() {

    BoundedQueue *queue;
    Status stat;
    int i;
    long len;
    char **items;

    stat = boundedqueue_new(&queue, LEN);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testBoundedQueueToArray() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = boundedqueue_enqueue(queue, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    stat = boundedqueue_toArray(queue, (void ***)&items, &len);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(len, LEN);

    for (i = 0; i < len; i++)
        CU_ASSERT_TRUE( strcmp(items[i], array[i]) == 0 );

    free(items);
    boundedqueue_destroy(queue, NULL);

    CU_PASS("testBoundedQueueToArray() - Test Passed");
}

static void testBoundedQueueIterator() {

    BoundedQueue *queue;
    Iterator *iter;
    Status stat;
    int i;
    char *element;

    stat = boundedqueue_new(&queue, LEN);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testBoundedQueueIterator() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = boundedqueue_enqueue(queue, array[i]);
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

    stat = boundedqueue_new(&queue, LEN);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testBoundedQueueClear() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = boundedqueue_enqueue(queue, array[i]);
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

    CU_pSuite suite = CU_add_suite("ArrayList Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "BoundedQueue - Empty Queue", testEmptyBoundedQueue);
    CU_add_test(suite, "BoundedQueue - Single Item", testSingleItem);
    CU_add_test(suite, "BoundedQueue - Enqueue & Dequeue", testEnqueueDequeue);
    CU_add_test(suite, "BoundedQueue - Array", testBoundedQueueToArray);
    CU_add_test(suite, "BoundedQueue - Iterator", testBoundedQueueIterator);
    CU_add_test(suite, "BoundedQueue - Clear", testBoundedQueueClear);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
