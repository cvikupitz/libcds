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

#define CAPACITY 5L

static void validateEmptyBoundedStack(BoundedStack *stack) {

    Status stat;
    int *element;
    
    stat = boundedstack_peek(stack, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = boundedstack_pop(stack, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    long size = boundedstack_size(stack);
    CU_ASSERT_EQUAL(size, 0L);

    Boolean isEmpty = boundedstack_isEmpty(stack);
    CU_ASSERT_EQUAL(isEmpty, TRUE);
    Boolean isFull = boundedstack_isFull(stack);
    CU_ASSERT_EQUAL(isFull, FALSE);
    
    Iterator *iter;
    stat = boundedstack_iterator(stack, &iter);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    int **array;
    long len;
    stat = boundedstack_toArray(stack, (void ***)&array, &len);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
}

void testEmptyBoundedStack() {

    BoundedStack *stack = NULL;
    Status stat;

    stat = boundedstack_new(&stack, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyBoundedStack() - allocation failure");
    
    validateEmptyBoundedStack(stack);
    boundedstack_destroy(stack, NULL);
    CU_PASS("testEmptyBoundedStack() - Test Passed");
}

static char *singleItem = "Test";
void testSingleItem() {
    
    BoundedStack *stack;
    Status stat;

    stat = boundedstack_new(&stack, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    stat = boundedstack_push(stack, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    long size = boundedstack_size(stack);
    long capacity = boundedstack_capacity(stack);
    Boolean isEmpty = boundedstack_isEmpty(stack);
    Boolean isFull = boundedstack_isFull(stack);
    CU_ASSERT_EQUAL(size, 1L);
    CU_ASSERT_EQUAL(capacity, CAPACITY);
    CU_ASSERT_EQUAL(isEmpty, FALSE);
    CU_ASSERT_EQUAL(isFull, FALSE);

    char *element;
    stat = boundedstack_peek(stack, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    stat = boundedstack_pop(stack, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    stat = boundedstack_pop(stack, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    validateEmptyBoundedStack(stack);
    boundedstack_destroy(stack, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

#define LEN 9
static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple", "gray", "white", "black"};

void testPushPop() {
    
    BoundedStack *stack;
    Status stat;

    stat = boundedstack_new(&stack, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testPushPop() - allocation failure");

    int i;
    long size;
    Boolean isEmpty, isFull;
    char *element;
    for (i = 0; i < CAPACITY; i++) {
        stat = boundedstack_push(stack, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        size = boundedstack_size(stack);
        isEmpty = boundedstack_isEmpty(stack);
        isFull = boundedstack_isFull(stack);
        CU_ASSERT_EQUAL(size, i + 1);
        CU_ASSERT_EQUAL(isEmpty, FALSE);
        CU_ASSERT_EQUAL(isFull, (size == CAPACITY) ? TRUE : FALSE);
        stat = boundedstack_peek(stack, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
    }

    for (i = CAPACITY; i < LEN; i++) {
        stat = boundedstack_push(stack, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_STRUCT_FULL);
        size = boundedstack_size(stack);
        isEmpty = boundedstack_isEmpty(stack);
        isFull = boundedstack_isFull(stack);
        CU_ASSERT_EQUAL(size, CAPACITY);
        CU_ASSERT_EQUAL(isEmpty, FALSE);
        CU_ASSERT_EQUAL(isFull, TRUE);
    }

    for (i = CAPACITY - 1; i >= 0; i--) {
        stat = boundedstack_pop(stack, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
        size = boundedstack_size(stack);
        isEmpty = boundedstack_isEmpty(stack);
        isFull = boundedstack_isFull(stack);
        CU_ASSERT_EQUAL(size, i);
        CU_ASSERT_EQUAL( isEmpty, (size != 0L) ? FALSE : TRUE );
        CU_ASSERT_EQUAL(isFull, FALSE);
    }

    boundedstack_destroy(stack, NULL);
    CU_PASS("testPushPop() - Test Passed");
}

void testBoundedStackToArray() {

    BoundedStack *stack;
    Status stat;

    stat = boundedstack_new(&stack, LEN);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testBoundedStackToArray() - allocation failure");

    int i, j;
    for (i = 0; i < LEN; i++) {
        stat = boundedstack_push(stack, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char **items;
    long len;
    stat = boundedstack_toArray(stack, (void ***)&items, &len);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(len, LEN);

    for (i = 0, j = LEN - 1; i < len; i++, j--)
        CU_ASSERT_TRUE( strcmp(items[i], array[j]) == 0 );

    free(items);
    boundedstack_destroy(stack, NULL);
    CU_PASS("testBoundedStackToArray() - Test Passed");
}

void testBoundedStackIterator() {

    BoundedStack *stack;
    Status stat;

    stat = boundedstack_new(&stack, LEN);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testBoundedStackIterator() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = boundedstack_push(stack, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    Iterator *iter;
    stat = boundedstack_iterator(stack, &iter);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    char *element;
    i = LEN - 1;
    while (iterator_hasNext(iter) == TRUE) {
        stat = iterator_next(iter, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i--]) == 0 );
    }

    iterator_destroy(iter);
    boundedstack_destroy(stack, NULL);
    CU_PASS("testBoundedStackIterator() - Test Passed");
}

void testBoundedStackClear() {
    
    BoundedStack *stack;
    Status stat;

    stat = boundedstack_new(&stack, LEN);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testBoundedStackClear() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = boundedstack_push(stack, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    boundedstack_clear(stack, NULL);
    validateEmptyBoundedStack(stack);
    boundedstack_destroy(stack, NULL);

    CU_PASS("testBoundedStackClear() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("BoundedStack Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "Empty stack", testEmptyBoundedStack);
    CU_add_test(suite, "Single item", testSingleItem);
    CU_add_test(suite, "Push & pop", testPushPop);
    CU_add_test(suite, "BoundedStack toArray", testBoundedStackToArray);
    CU_add_test(suite, "BoundedStack iterator", testBoundedStackIterator);
    CU_add_test(suite, "BoundedStack clear", testBoundedStackClear);
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
