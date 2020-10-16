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
#include "queue.h"

/* Single item used for testing */
static char *singleItem = "Test";

/* Collection of items used for testing */
#define LEN 6
static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple"};

static void validateEmptyQueue(Queue *queue) {

    Iterator *iter;
    Status stat;
    Boolean isEmpty;
    char *element, **array;
    long size, len;

    stat = queue_peek(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = queue_dequeue(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    size = queue_size(queue);
    CU_ASSERT_EQUAL(size, 0L);
    isEmpty = queue_isEmpty(queue);
    CU_ASSERT_EQUAL(isEmpty, TRUE);
    stat = queue_iterator(queue, &iter);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = queue_toArray(queue, (void ***)&array, &len);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
}

static void testEmptyQueue() {

    Queue *queue;
    Status stat;

    stat = queue_new(&queue);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyQueue() - allocation failure");

    validateEmptyQueue(queue);
    queue_destroy(queue, NULL);

    CU_PASS("testEmptyQueue() - Test Passed");
}

static void testSingleItem() {

    Queue *queue;
    Status stat;
    Boolean isEmpty;
    char *element;
    long size;

    stat = queue_new(&queue);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    stat = queue_enqueue(queue, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    size = queue_size(queue);
    isEmpty = queue_isEmpty(queue);
    CU_ASSERT_EQUAL(size, 1L);
    CU_ASSERT_EQUAL(isEmpty, FALSE);

    stat = queue_peek(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    stat = queue_dequeue(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
    stat = queue_dequeue(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    validateEmptyQueue(queue);
    queue_destroy(queue, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

static void testEnqueueDequeue() {

    Queue *queue;
    Status stat;
    Boolean isEmpty;
    char *element;
    int i;
    long size;

    stat = queue_new(&queue);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEnqueueDequeue() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = queue_enqueue(queue, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        size = queue_size(queue);
        isEmpty = queue_isEmpty(queue);
        CU_ASSERT_EQUAL(size, i + 1);
        CU_ASSERT_EQUAL(isEmpty, FALSE);
        stat = queue_peek(queue, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[0]) == 0 );
    }

    for (i = 0; i < 0; i++) {
        stat = queue_dequeue(queue, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
        size = queue_size(queue);
        isEmpty = queue_isEmpty(queue);
        CU_ASSERT_EQUAL(size, i);
        CU_ASSERT_EQUAL( isEmpty, (size != 0L) ? FALSE : TRUE );
    }

    queue_destroy(queue, NULL);

    CU_PASS("testEnqueueDequeue() - Test Passed");
}

static void testQueueToArray() {

    Queue *queue;
    Status stat;
    int i;
    long len;
    char **items;

    stat = queue_new(&queue);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testQueueToArray() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = queue_enqueue(queue, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    stat = queue_toArray(queue, (void ***)&items, &len);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(len, LEN);

    for (i = 0; i < len; i++)
        CU_ASSERT_TRUE( strcmp(items[i], array[i]) == 0 );

    free(items);
    queue_destroy(queue, NULL);

    CU_PASS("testQueueToArray() - Test Passed");
}

static void testQueueIterator() {

    Queue *queue;
    Iterator *iter;
    Status stat;
    int i;
    char *element;

    stat = queue_new(&queue);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testQueueIterator() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = queue_enqueue(queue, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    stat = queue_iterator(queue, &iter);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    i = 0;
    while (iterator_hasNext(iter) == TRUE) {
        stat = iterator_next(iter, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i++]) == 0 );
    }

    iterator_destroy(iter);
    queue_destroy(queue, NULL);

    CU_PASS("testQueueIterator() - Test Passed");
}

static void testQueueClear() {

    Queue *queue;
    Status stat;
    int i;

    stat = queue_new(&queue);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testQueueClear() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = queue_enqueue(queue, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    queue_clear(queue, NULL);
    validateEmptyQueue(queue);
    queue_destroy(queue, NULL);

    CU_PASS("testQueueClear() - Test Passed");
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

    CU_add_test(suite, "Queue - Empty", testEmptyQueue);
    CU_add_test(suite, "Queue - Single Item", testSingleItem);
    CU_add_test(suite, "Queue - Enqueue & Dequeue", testEnqueueDequeue);
    CU_add_test(suite, "Queue - Array", testQueueToArray);
    CU_add_test(suite, "Queue - Iterator", testQueueIterator);
    CU_add_test(suite, "Queue - Clear", testQueueClear);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
