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

// test push
// test peek
// test pop
// test size
// test isEmpty
// test size
// test clear
// test iterator
  // test empty vs full
// test array
  // test empty vs full


static void validateEmptyStack(Stack *stack) {

    Status stat;
    int *element;
    
    stat = stack_peek(stack, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = stack_pop(stack, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    long size = stack_size(stack);
    CU_ASSERT_EQUAL(size, 0L);

    Boolean isEmpty = stack_isEmpty(stack);
    CU_ASSERT_EQUAL(isEmpty, TRUE);
    
    Iterator *iter;
    stat = stack_iterator(stack, &iter);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    int **array;
    long len;
    stat = stack_toArray(stack, (void ***)&array, &len);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
}

void testEmptyStack() {

    Stack *stack = NULL;
    Status stat;

    stat = stack_new(&stack);
    if (stat != STAT_SUCCESS)
        CU_FAIL("ERROR: testEmptyStack() - allocation failure");
    
    validateEmptyStack(stack);
    stack_destroy(stack, NULL);
    CU_PASS("testEmptyStack() - Test Passed");
}

static char *singleItem = "Test";
void testSingleItem() {
    
    Stack *stack;
    Status stat;

    stat = stack_new(&stack);
    if (stat != STAT_SUCCESS)
        CU_FAIL("ERROR: testSingleItem() - allocation failure");

    stat = stack_push(stack, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    long size = stack_size(stack);
    Boolean isEmpty = stack_isEmpty(stack);
    CU_ASSERT_EQUAL(size, 1L);
    CU_ASSERT_EQUAL(isEmpty, FALSE);

    char *element;
    stat = stack_peek(stack, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    stat = stack_pop(stack, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    stat = stack_pop(stack, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    validateEmptyStack(stack);
    stack_destroy(stack, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple"};
void testPushPop() {
    
    Stack *stack;
    Status stat;

    stat = stack_new(&stack);
    if (stat != STAT_SUCCESS)
        CU_FAIL("ERROR: testPushPop() - allocation failure");

    int i;
    long size;
    Boolean isEmpty;
    char *element;
    for (i = 0; i < 6; i++) {
        stat = stack_push(stack, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        size = stack_size(stack);
        isEmpty = stack_isEmpty(stack);
        CU_ASSERT_EQUAL(size, i + 1);
        CU_ASSERT_EQUAL(isEmpty, FALSE);
        stat = stack_peek(stack, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
    }

    for (i = 5; i >= 0; i++) {
        stat = stack_pop(stack, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
        size = stack_size(stack);
        isEmpty = stack_isEmpty(stack);
        CU_ASSERT_EQUAL(size, i + 1);
        CU_ASSERT_EQUAL(isEmpty, FALSE);
    }

    stack_destroy(stack, NULL);
    CU_PASS("testPushPop() - Test Passed");
}

void testStackToArray() {

    Stack *stack;
    Status stat;

    stat = stack_new(&stack);
    if (stat != STAT_SUCCESS)
        CU_FAIL("ERROR: testStackToArray() - allocation failure");

    stack_destroy(stack, NULL);
    CU_PASS("testStackToArray() - Test Passed");
}

void testStackIterator() {

    Stack *stack;
    Status stat;

    stat = stack_new(&stack);
    if (stat != STAT_SUCCESS)
        CU_FAIL("ERROR: testStackIterator() - allocation failure");

    stack_destroy(stack, NULL);
    CU_PASS("testStackIterator() - Test Passed");
}

void testStackClear() {
    
    Stack *stack;
    Status stat;

    stat = stack_new(&stack);
    if (stat != STAT_SUCCESS)
        CU_FAIL("ERROR: testStackClear() - allocation failure");

    int i;
    for (i = 0; i < 6; i++) {
        stat = stack_push(stack, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    stack_clear(stack, NULL);
    validateEmptyStack(stack);
    stack_destroy(stack, NULL);

    CU_PASS("testStackClear() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Stack Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "Empty stack", testEmptyStack);
    CU_add_test(suite, "Single item", testSingleItem);
    CU_add_test(suite, "Push & pop", testPushPop);
    CU_add_test(suite, "Stack toArray", testStackToArray);
    CU_add_test(suite, "Stack iterator", testStackIterator);
    CU_add_test(suite, "Stack clear", testStackClear);
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
