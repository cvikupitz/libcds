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
#include "treeset.h"

/* Single item used for testing */
static int singleItem = 10;

/* Collection of items used for testing */
#define LEN 50
static int orderedSet[] = {
    1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50
};

/* Comparator function used for comparing items in the treeset */
static int treeCmp(void *x, void *y) {
    return *((int *)x) - *((int *)y);
}

static void validateEmptyTreeSet(TreeSet *treeset) {

    Iterator *iter;
    Status stat;
    Boolean boolean;
    int *element, **array;
    long size, len;

    boolean = treeset_contains(treeset, &singleItem);
    CU_ASSERT_EQUAL(boolean, FALSE);
    stat = treeset_first(treeset, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_last(treeset, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    stat = treeset_floor(treeset, &singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_ceiling(treeset, &singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_lower(treeset, &singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_higher(treeset, &singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    stat = treeset_pollFirst(treeset, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_pollLast(treeset, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_remove(treeset, &singleItem, NULL);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    size = treeset_size(treeset);
    CU_ASSERT_EQUAL(size, 0L);
    boolean = treeset_isEmpty(treeset);
    CU_ASSERT_EQUAL(boolean, TRUE);
    stat = treeset_iterator(treeset, &iter);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_toArray(treeset, (void ***)&array, &len);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
}

static void testEmptyTreeSet() {

    TreeSet *treeset;
    Status stat;

    stat = treeset_new(&treeset, treeCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyTreeSet() - allocation failure");

    validateEmptyTreeSet(treeset);
    treeset_destroy(treeset, NULL);

    CU_PASS("testEmptyTreeSet() - Test Passed");
}

/*static void testSingleItem() {

    TreeSet *treeset;
    Status stat;
    Boolean isEmpty;
    long size;
    char *element;

    stat = treeset_new(&treeset);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    stat = treeset_push(treeset, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    size = treeset_size(treeset);
    isEmpty = treeset_isEmpty(treeset);
    CU_ASSERT_EQUAL(size, 1L);
    CU_ASSERT_EQUAL(isEmpty, FALSE);

    stat = treeset_peek(treeset, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    stat = treeset_pop(treeset, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
    stat = treeset_pop(treeset, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    validateEmptyTreeSet(treeset);
    treeset_destroy(treeset, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}*/

static void testTreeSetToArray() {

    TreeSet *treeset;
    Status stat;
    int **items;
    int i;
    long len;

    stat = treeset_new(&treeset, treeCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testTreeSetToArray() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = treeset_add(treeset, &(orderedSet[i]));
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    stat = treeset_toArray(treeset, (void ***)&items, &len);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(len, LEN);

    for (i = 0; i < len; i++)
        CU_ASSERT_TRUE( orderedSet[i] == *(items[i]) );

    free(items);
    treeset_destroy(treeset, NULL);

    CU_PASS("testTreeSetToArray() - Test Passed");
}

static void testTreeSetIterator() {

    Iterator *iter;
    TreeSet *treeset;
    Status stat;
    int i;
    int *element;

    stat = treeset_new(&treeset, treeCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testTreeSetIterator() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = treeset_add(treeset, &(orderedSet[i]));
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    stat = treeset_iterator(treeset, &iter);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    i = 0;
    while (iterator_hasNext(iter) == TRUE) {
        stat = iterator_next(iter, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( *element == orderedSet[i++] );
    }

    iterator_destroy(iter);
    treeset_destroy(treeset, NULL);

    CU_PASS("testTreeSetIterator() - Test Passed");
}

static void testTreeSetClear() {

    TreeSet *treeset;
    Status stat;
    int i;

    stat = treeset_new(&treeset, treeCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testTreeSetClear() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = treeset_add(treeset, &(orderedSet[i]));
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    treeset_clear(treeset, NULL);
    validateEmptyTreeSet(treeset);
    treeset_destroy(treeset, NULL);

    CU_PASS("testTreeSetClear() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite("TreeSet Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "TreeSet - Empty TreeSet", testEmptyTreeSet);
    //CU_add_test(suite, "TreeSet - Single Item", testSingleItem);
    CU_add_test(suite, "TreeSet - Array", testTreeSetToArray);
    CU_add_test(suite, "TreeSet - Iterator", testTreeSetIterator);
    CU_add_test(suite, "TreeSet - Clear", testTreeSetClear);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
