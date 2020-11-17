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
#include "boundedstack.h"

/* Single item used for testing */
static char *singleItem = "Test";

/* Collection of items used for testing */
#define LEN 6
static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple"};

/* Capacity for bounded stack */
#define CAPACITY 4L

static void validateEmptyBoundedStack(BoundedStack *stack) {

    Array *arr;
    Iterator *iter;
    char *item;

    CU_ASSERT_TRUE(boundedstack_peek(stack, (void **)&item) == STAT_STRUCT_EMPTY );
    CU_ASSERT_TRUE(boundedstack_pop(stack, (void **)&item) == STAT_STRUCT_EMPTY );
    CU_ASSERT_TRUE( boundedstack_size(stack) == 0L );
    CU_ASSERT_TRUE( boundedstack_isEmpty(stack) == TRUE );
    CU_ASSERT_TRUE( boundedstack_isFull(stack) == FALSE );
    CU_ASSERT_TRUE( boundedstack_iterator(stack, &iter) == STAT_STRUCT_EMPTY );
    CU_ASSERT_TRUE( boundedstack_toArray(stack, &arr) == STAT_STRUCT_EMPTY );
}

static void testEmptyBoundedStack() {

    BoundedStack *stack;
    Status stat;

    stat = boundedstack_new(&stack, 0L);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyBoundedStack() - allocation failure");

    validateEmptyBoundedStack(stack);
    boundedstack_destroy(stack, NULL);

    CU_PASS("testEmptyBoundedStack() - Test Passed");
}

static void testSingleItem() {

    BoundedStack *stack;
    Status stat;
    char *item;

    stat = boundedstack_new(&stack, 1L);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    CU_ASSERT_TRUE( boundedstack_push(stack, singleItem) == STAT_SUCCESS );
    CU_ASSERT_TRUE( boundedstack_size(stack) == 1L );
    CU_ASSERT_TRUE( boundedstack_capacity(stack) == 1L );
    CU_ASSERT_TRUE( boundedstack_isEmpty(stack) == FALSE );
    CU_ASSERT_TRUE( boundedstack_isFull(stack) == TRUE );
    CU_ASSERT_TRUE( boundedstack_peek(stack, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( boundedstack_pop(stack, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( boundedstack_pop(stack, (void **)&item) == STAT_STRUCT_EMPTY );

    validateEmptyBoundedStack(stack);
    boundedstack_destroy(stack, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

static void testPushPop() {

    BoundedStack *stack;
    Status stat;
    int i;
    long size;
    char *item;

    stat = boundedstack_new(&stack, 0L);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testPushPop() - allocation failure");

    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE( boundedstack_push(stack, array[i]) == STAT_SUCCESS );
        CU_ASSERT_TRUE( boundedstack_size(stack) == (i + 1) );
        CU_ASSERT_TRUE( boundedstack_isEmpty(stack) == FALSE );
        CU_ASSERT_TRUE( boundedstack_isFull(stack) == FALSE );
        CU_ASSERT_TRUE( boundedstack_peek(stack, (void **)&item) == STAT_SUCCESS );
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
    }

    for (i = LEN - 1; i >= 0; i--) {
        CU_ASSERT_TRUE(boundedstack_pop(stack, (void **)&item) == STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
        size = boundedstack_size(stack);
        CU_ASSERT_TRUE( size == i);
        CU_ASSERT_TRUE( boundedstack_isEmpty(stack) == (size != 0L ? FALSE : TRUE) );
        CU_ASSERT_TRUE( boundedstack_isFull(stack) == FALSE );
    }

    boundedstack_destroy(stack, NULL);

    CU_PASS("testPushPop() - Test Passed");
}

static void testCapacity() {

    BoundedStack *stack;
    Status stat;
    char *item;
    int i;
    long size;

    stat = boundedstack_new(&stack, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testCapacity() - allocation failure");

    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE( boundedstack_push(stack, array[i]) == (i < CAPACITY ? STAT_SUCCESS : STAT_STRUCT_FULL) );
        size = boundedstack_size(stack);
        CU_ASSERT_TRUE( size == (i < CAPACITY ? (i + 1) : CAPACITY) );
        CU_ASSERT_TRUE( boundedstack_isEmpty(stack) == FALSE );
        CU_ASSERT_TRUE( boundedstack_isFull(stack) == (size < CAPACITY ? FALSE : TRUE) );
        CU_ASSERT_TRUE( boundedstack_peek(stack, (void **)&item) == STAT_SUCCESS );
        CU_ASSERT_TRUE( strcmp(item, (size < CAPACITY) ? array[i] : array[CAPACITY - 1]) == 0 );
    }

    boundedstack_destroy(stack, NULL);

    CU_PASS("testCapacity() - Test Passed");
}

static void testBoundedStackToArray() {

    BoundedStack *stack;
    Array *arr;
    Status stat;
    int i, j;

    stat = boundedstack_new(&stack, 0L);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testBoundedStackToArray() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( boundedstack_push(stack, array[i]) == STAT_SUCCESS );
    CU_ASSERT_TRUE( boundedstack_toArray(stack, &arr) == STAT_SUCCESS );
    CU_ASSERT_TRUE( arr->len == LEN );

    for (i = 0, j = LEN - 1; i < arr->len; i++, j--)
        CU_ASSERT_TRUE( strcmp(arr->items[i], array[j]) == 0 );

    FREE_ARRAY(arr)
    boundedstack_destroy(stack, NULL);

    CU_PASS("testBoundedStackToArray() - Test Passed");
}

static void testBoundedStackIterator() {

    Iterator *iter;
    BoundedStack *stack;
    Status stat;
    int i;
    char *item;

    stat = boundedstack_new(&stack, 0L);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testBoundedStackIterator() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( boundedstack_push(stack, array[i]) == STAT_SUCCESS );
    CU_ASSERT_TRUE( boundedstack_iterator(stack, &iter) == STAT_SUCCESS );

    i = LEN - 1;
    while (iterator_hasNext(iter) == TRUE) {
        CU_ASSERT_TRUE( iterator_next(iter, (void **)&item) == STAT_SUCCESS );
        CU_ASSERT_TRUE( strcmp(item, array[i--]) == 0 );
    }

    iterator_destroy(iter);
    boundedstack_destroy(stack, NULL);

    CU_PASS("testBoundedStackIterator() - Test Passed");
}

static void testBoundedStackClear() {

    BoundedStack *stack;
    Status stat;
    int i;

    stat = boundedstack_new(&stack, 0L);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testBoundedStackClear() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( boundedstack_push(stack, array[i]) == STAT_SUCCESS );

    boundedstack_clear(stack, NULL);
    validateEmptyBoundedStack(stack);
    boundedstack_destroy(stack, NULL);

    CU_PASS("testBoundedStackClear() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite("BoundedStack Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "BoundedStack - Empty BoundedStack", testEmptyBoundedStack);
    CU_add_test(suite, "BoundedStack - Single Item", testSingleItem);
    CU_add_test(suite, "BoundedStack - Push & Pop", testPushPop);
    CU_add_test(suite, "BoundedStack - Capacity Check", testCapacity);
    CU_add_test(suite, "BoundedStack - Array", testBoundedStackToArray);
    CU_add_test(suite, "BoundedStack - Iterator", testBoundedStackIterator);
    CU_add_test(suite, "BoundedStack - Clear", testBoundedStackClear);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
