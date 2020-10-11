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
    int *element;
    
    stat = deque_peek(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = queue_dequeue(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    long size = queue_size(queue);
    CU_ASSERT_EQUAL(size, 0L);

    Boolean isEmpty = queue_isEmpty(queue);
    CU_ASSERT_EQUAL(isEmpty, TRUE);
    
    Iterator *iter;
    stat = queue_iterator(queue, &iter);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    int **array;
    long len;
    stat = queue_toArray(queue, (void ***)&array, &len);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
}

void testEmptyDeque() {

    Deque *queue = NULL;
    Status stat;

    stat = queue_new(&queue);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyDeque() - allocation failure");
    
    validateEmptyDeque(queue);
    queue_destroy(queue, NULL);
    CU_PASS("testEmptyDeque() - Test Passed");
}

static char *singleItem = "Test";
void testSingleItem() {
    
    Deque *queue;
    Status stat;

    stat = queue_new(&queue);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    stat = queue_enqueue(queue, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    long size = queue_size(queue);
    Boolean isEmpty = queue_isEmpty(queue);
    CU_ASSERT_EQUAL(size, 1L);
    CU_ASSERT_EQUAL(isEmpty, FALSE);

    char *element;
    stat = queue_peek(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    stat = queue_dequeue(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    stat = queue_dequeue(queue, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    validateEmptyDeque(queue);
    queue_destroy(queue, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

#define LEN 6
static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple"};

void testEnqueueDequeue() {
    
    Deque *queue;
    Status stat;

    stat = queue_new(&queue);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEnqueueDequeue() - allocation failure");

    int i;
    long size;
    Boolean isEmpty;
    char *element;
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

void testDequeToArray() {

    Deque *queue;
    Status stat;

    stat = queue_new(&queue);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testDequeToArray() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = queue_enqueue(queue, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char **items;
    long len;
    stat = queue_toArray(queue, (void ***)&items, &len);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(len, LEN);

    for (i = 0; i < len; i++)
        CU_ASSERT_TRUE( strcmp(items[i], array[i]) == 0 );

    free(items);
    queue_destroy(queue, NULL);
    CU_PASS("testDequeToArray() - Test Passed");
}

void testDequeIterator() {

    Deque *queue;
    Status stat;

    stat = queue_new(&queue);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testDequeIterator() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = queue_enqueue(queue, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    Iterator *iter;
    stat = queue_iterator(queue, &iter);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    char *element;
    i = 0;
    while (iterator_hasNext(iter) == TRUE) {
        stat = iterator_next(iter, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i++]) == 0 );
    }

    iterator_destroy(iter);
    queue_destroy(queue, NULL);
    CU_PASS("testDequeIterator() - Test Passed");
}

void testDequeClear() {
    
    Deque *queue;
    Status stat;

    stat = queue_new(&queue);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testDequeClear() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = queue_enqueue(queue, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    queue_clear(queue, NULL);
    validateEmptyDeque(queue);
    queue_destroy(queue, NULL);

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

    CU_add_test(suite, "Empty queue", testEmptyDeque);
    CU_add_test(suite, "Single item", testSingleItem);
    CU_add_test(suite, "Enqueue & dequeue", testEnqueueDequeue);
    CU_add_test(suite, "Deque toArray", testDequeToArray);
    CU_add_test(suite, "Deque iterator", testDequeIterator);
    CU_add_test(suite, "Deque clear", testDequeClear);
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
