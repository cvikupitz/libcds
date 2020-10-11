#include <stdio.h>
#include <stdlib.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "stack.h"

static void testEmptyStack() {

    Stack *stack;
    Status stat;

    stat = stack_new(&stack);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

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

    stack_destroy(stack, NULL);
    CU_PASS("Suite testEmptyStack() tests passed.");
}

#define ITEM 99
static void testSingleElement() {

    Stack *stack;
    Status stat;

    stat = stack_new(&stack);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    int *item = (int *)malloc(sizeof(int));
    if (item == NULL)
        CU_FAIL("testSingleElement - allocation failure");
    *item = ITEM;
    stat = stack_push(stack, item);

    int *element;
    stat = stack_peek(stack, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(*element, ITEM);

    long size = stack_size(stack);
    CU_ASSERT_EQUAL(size, 1L);

    Boolean isEmpty = stack_isEmpty(stack);
    CU_ASSERT_EQUAL(isEmpty, FALSE);
    
    Iterator *iter;
    stat = stack_iterator(stack, &iter);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    int **array;
    long len;
    stat = stack_toArray(stack, (void ***)&array, &len);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(*len, 1L);

    CU_PASS("Suite testSingleElement() tests passed.");
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
    CU_add_test(suite, "Single Element", testSingleElement);
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
