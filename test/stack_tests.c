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
#include "stack.h"

/* Single item used for testing */
static char *singleItem = "Test";

/* Collection of items used for testing */
#define LEN 6
static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple"};

static void validateEmptyStack(Stack *stack) {

    Array *arr;
    Iterator *iter;
    char *item;

    CU_ASSERT_TRUE( stack_pop(stack, (void **)&item) == STAT_STRUCT_EMPTY );
    CU_ASSERT_TRUE( stack_pop(stack, (void **)&item) == STAT_STRUCT_EMPTY );
    CU_ASSERT_TRUE( stack_size(stack) == 0L);
    CU_ASSERT_TRUE( stack_isEmpty(stack) == TRUE );
    CU_ASSERT_TRUE( stack_toArray(stack, &arr) == STAT_STRUCT_EMPTY);
    CU_ASSERT_TRUE( stack_iterator(stack, &iter) == STAT_STRUCT_EMPTY );
}

static void testEmptyStack() {

    Stack *stack;
    Status stat;

    stat = stack_new(&stack);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyStack() - allocation failure");

    validateEmptyStack(stack);
    stack_destroy(stack, NULL);

    CU_PASS("testEmptyStack() - Test Passed");
}

static void testSingleItem() {

    Stack *stack;
    Status stat;
    char *item;

    stat = stack_new(&stack);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    CU_ASSERT_TRUE( stack_push(stack, singleItem) == STAT_SUCCESS );
    CU_ASSERT_TRUE( stack_size(stack) == 1L );
    CU_ASSERT_TRUE( stack_isEmpty(stack) == FALSE );
    CU_ASSERT_TRUE( stack_peek(stack, (void **)&item) == STAT_SUCCESS );
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( stack_pop(stack, (void **)&item) == STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, singleItem) == 0 );
    CU_ASSERT_TRUE( stack_pop(stack, (void **)&item) == STAT_STRUCT_EMPTY);

    validateEmptyStack(stack);
    stack_destroy(stack, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

static void testPushPop() {

    Stack *stack;
    Status stat;
    int i;
    long size;
    char *item;

    stat = stack_new(&stack);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testPushPop() - allocation failure");

    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE( stack_push(stack, array[i]) == STAT_SUCCESS );
        CU_ASSERT_TRUE( stack_size(stack) == (i + 1) );
        CU_ASSERT_TRUE( stack_isEmpty(stack) == FALSE );
        CU_ASSERT_TRUE( stack_peek(stack, (void **)&item) == STAT_SUCCESS );
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
    }

    for (i = LEN - 1; i >= 0; i--) {
        CU_ASSERT_TRUE( stack_pop(stack, (void **)&item) == STAT_SUCCESS );
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
        size = stack_size(stack);
        CU_ASSERT_TRUE( size == i );
        CU_ASSERT_TRUE( stack_isEmpty(stack) == (size != 0L ? FALSE : TRUE) );
    }

    stack_destroy(stack, NULL);

    CU_PASS("testPushPop() - Test Passed");
}

static void testStackToArray() {

    Stack *stack;
    Array *arr;
    Status stat;
    int i, j;

    stat = stack_new(&stack);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testStackToArray() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( stack_push(stack, array[i]) == STAT_SUCCESS );
    CU_ASSERT_TRUE( stack_toArray(stack, &arr) == STAT_SUCCESS );
    CU_ASSERT_TRUE( arr->len == LEN );

    for (i = 0, j = LEN - 1; i < arr->len; i++, j--)
        CU_ASSERT_TRUE( strcmp(arr->items[i], array[j]) == 0 );

    FREE_ARRAY(arr)
    stack_destroy(stack, NULL);

    CU_PASS("testStackToArray() - Test Passed");
}

static void testStackIterator() {

    Iterator *iter;
    Stack *stack;
    Status stat;
    int i;
    char *item;

    stat = stack_new(&stack);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testStackIterator() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( stack_push(stack, array[i]) == STAT_SUCCESS );
    CU_ASSERT_TRUE( stack_iterator(stack, &iter) == STAT_SUCCESS );

    i = LEN - 1;
    while (iterator_hasNext(iter) == TRUE) {
        CU_ASSERT_TRUE( iterator_next(iter, (void **)&item) == STAT_SUCCESS );
        CU_ASSERT_TRUE( strcmp(item, array[i--]) == 0 );
    }

    iterator_destroy(iter);
    stack_destroy(stack, NULL);

    CU_PASS("testStackIterator() - Test Passed");
}

static void testStackClear() {

    Stack *stack;
    Status stat;
    int i;

    stat = stack_new(&stack);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testStackClear() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( stack_push(stack, array[i]) == STAT_SUCCESS );

    stack_clear(stack, NULL);
    validateEmptyStack(stack);
    stack_destroy(stack, NULL);

    CU_PASS("testStackClear() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite("Stack Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "Stack - Empty Stack", testEmptyStack);
    CU_add_test(suite, "Stack - Single Item", testSingleItem);
    CU_add_test(suite, "Stack - Push & Pop", testPushPop);
    CU_add_test(suite, "Stack - Array", testStackToArray);
    CU_add_test(suite, "Stack - Iterator", testStackIterator);
    CU_add_test(suite, "Stack - Clear", testStackClear);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
