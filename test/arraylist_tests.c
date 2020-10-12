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
#include "arraylist.h"

#define CAPACITY 2L

static char *singleItem = "Test";

static void validateEmptyArrayList(ArrayList *list) {

    Status stat;
    char *element;
    
    stat = arraylist_get(list, 0L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = arraylist_set(list, 0L, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = arraylist_remove(list, 0L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    long size = arraylist_size(list);
    CU_ASSERT_EQUAL(size, 0L);

    Boolean isEmpty = arraylist_isEmpty(list);
    CU_ASSERT_EQUAL(isEmpty, TRUE);
    
    Iterator *iter;
    stat = arraylist_iterator(list, &iter);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    int **array;
    long len;
    stat = arraylist_toArray(list, (void ***)&array, &len);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
}

void testEmptyArrayList() {

    ArrayList *list = NULL;
    Status stat;

    stat = arraylist_new(&list, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyArrayList() - allocation failure");
    
    validateEmptyArrayList(list);
    arraylist_destroy(list, NULL);
    CU_PASS("testEmptyArrayList() - Test Passed");
}

void testSingleItem() {
    
    ArrayList *list;
    Status stat;

    stat = arraylist_new(&list, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    stat = arraylist_add(list, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    long size = arraylist_size(list);
    long capacity = arraylist_capacity(list);
    Boolean isEmpty = arraylist_isEmpty(list);
    CU_ASSERT_EQUAL(size, 1L);
    CU_ASSERT_EQUAL(capacity, CAPACITY);
    CU_ASSERT_EQUAL(isEmpty, FALSE);

    char *element;
    /*stat = arraylist_get(list, 0L, (void **)&element);
    printf("=================%d\n", stat);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );*/

    stat = arraylist_remove(list, 99L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);
    printf("*************** After invalid remove: %d\n", stat);

    stat = arraylist_remove(list, 0L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
    printf("*************** After valid remove: %d\n", stat);

    stat = arraylist_remove(list, 0L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    printf("*************** After empty remove: %d\n", stat);

    validateEmptyArrayList(list);
    arraylist_destroy(list, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

#define LEN 9
static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple", "gray", "white", "black"};

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("ArrayList Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "Empty arraylist", testEmptyArrayList);
    CU_add_test(suite, "Single item", testSingleItem);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
