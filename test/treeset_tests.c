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
static char *singleItem = "Test";

/* Ordered set used for testing */
#define LEN 50
static char *orderedSet[] = {
    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09",
    "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
    "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
    "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
    "40", "41", "42", "43", "44", "45", "46", "47", "48", "49"
};

/* Comparator function used by the hashset */
static int strCmp(void *a, void *b) {
    return strcmp((char *)a, (char *)b);
}

static void validateEmptyTreeSet(TreeSet *tree) {

    Iterator *iter;
    Status stat;
    Boolean isEmpty;
    char *element, **array;
    long size, len;

    stat = treeset_first(tree, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_last(tree, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_floor(tree, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_floor(tree, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_lower(tree, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_higher(tree, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    stat = treeset_pollFirst(tree, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_pollLast(tree, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_remove(tree, singleItem, NULL);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    size = treeset_size(tree);
    CU_ASSERT_EQUAL(size, 0L);
    isEmpty = treeset_isEmpty(tree);
    CU_ASSERT_EQUAL(isEmpty, TRUE);
    stat = treeset_iterator(tree, &iter);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_toArray(tree, (void ***)&array, &len);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
}

static void testEmptyTreeSet() {

    TreeSet *tree;
    Status stat;

    stat = treeset_new(&tree, strCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyTreeSet() - allocation failure");

    validateEmptyTreeSet(tree);
    treeset_destroy(tree, NULL);

    CU_PASS("testEmptyTreeSet() - Test Passed");
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
    //CU_add_test(suite, "TreeSet - Array", testTreeSetToArray);
    //CU_add_test(suite, "TreeSet - Iterator", testTreeSetIterator);
    //CU_add_test(suite, "TreeSet - Clear", testTreeSetClear);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
