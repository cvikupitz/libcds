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

#define LEN 9
static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple", "gray", "white", "black"};

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
    stat = arraylist_get(list, 0L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    stat = arraylist_remove(list, 99L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);

    stat = arraylist_remove(list, 0L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    stat = arraylist_remove(list, 0L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    validateEmptyArrayList(list);
    arraylist_destroy(list, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

void testInsertions() {

    ArrayList *list;
    Status stat;

    stat = arraylist_new(&list, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testInsertions() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = arraylist_add(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char *element;
    for (i = 0L; i < LEN; i++) {
        stat = arraylist_get(list, i, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
    }

    stat = arraylist_insert(list, 0L, singleItem);
    stat = arraylist_insert(list, 3L, singleItem);
    stat = arraylist_insert(list, 6L, singleItem);
    stat = arraylist_get(list, 0L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
    stat = arraylist_get(list, 3L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
    stat = arraylist_get(list, 6L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    arraylist_destroy(list, NULL);

    CU_PASS("testAdditions() - Test Passed");
}

void testSetItem() {

    ArrayList *list;
    Status stat;

    stat = arraylist_new(&list, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSetItem() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = arraylist_add(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char *element;
    stat = arraylist_set(list, 0L, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, array[0]) == 0 );
    stat = arraylist_set(list, 4L, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, array[4]) == 0 );
    stat = arraylist_set(list, 7L, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, array[7]) == 0 );

    stat = arraylist_get(list, 0L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
    stat = arraylist_get(list, 4L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
    stat = arraylist_get(list, 7L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    arraylist_destroy(list, NULL);

    CU_PASS("testSetItem() - Test Passed");
}

void testSequentialDelete() {

    ArrayList *list;
    Status stat;

    stat = arraylist_new(&list, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSequentialDelete() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = arraylist_add(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char *element;
    for (i = 0L; i < LEN; i++) {
        stat = arraylist_remove(list, 0L, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i]) == 0 );
    }

    arraylist_destroy(list, NULL);

    CU_PASS("testSequentialDelete() - Test Passed");
}

void testRandomDelete() {

    ArrayList *list;
    Status stat;

    stat = arraylist_new(&list, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testRandomDelete() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = arraylist_add(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char *element;
    stat = arraylist_remove(list, 7L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, array[7]) == 0 );
    stat = arraylist_remove(list, 5L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, array[5]) == 0 );
    stat = arraylist_remove(list, 1L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, array[1]) == 0 );

    arraylist_destroy(list, NULL);

    CU_PASS("testRandomDelete() - Test Passed");
}

void testEnsureCapacity() {

    ArrayList *list;
    Status stat;

    stat = arraylist_new(&list, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEnsureCapacity() - allocation failure");

    long capacity = arraylist_capacity(list);
    CU_ASSERT_EQUAL(capacity, CAPACITY);

    stat = arraylist_ensureCapacity(list, CAPACITY - 1);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    capacity = arraylist_capacity(list);
    CU_ASSERT_EQUAL(capacity, CAPACITY);

    stat = arraylist_ensureCapacity(list, CAPACITY + 20L);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    capacity = arraylist_capacity(list);
    CU_ASSERT_EQUAL(capacity, CAPACITY + 20L);

    arraylist_destroy(list, NULL);
    CU_PASS("testEnsureCapacity() - Test Passed");
}

void testTrimToSize() {

    ArrayList *list;
    Status stat;

    stat = arraylist_new(&list, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testTrimToSize() - allocation failure");

    stat = arraylist_trimToSize(list);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = arraylist_add(list, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    stat = arraylist_trimToSize(list);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);


    arraylist_destroy(list, NULL);
    CU_PASS("testEnsureCapacity() - Test Passed");
}

void testInvalidIndexAccess() {

    ArrayList *list;
    Status stat;

    stat = arraylist_new(&list, CAPACITY);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testInvalidIndexAccess() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = arraylist_add(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    stat = arraylist_insert(list, LEN + 1L, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);
    stat = arraylist_insert(list, LEN + 1L, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);
    stat = arraylist_insert(list, LEN + 10L, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);
    
    char *element;
    stat = arraylist_get(list, -1, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);
    stat = arraylist_get(list, LEN + 1L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);
    stat = arraylist_get(list, LEN + 10L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);

    stat = arraylist_set(list, -1, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);
    stat = arraylist_set(list, LEN + 1L, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);
    stat = arraylist_set(list, LEN + 10L, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);

    stat = arraylist_remove(list, -1, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);
    stat = arraylist_remove(list, LEN + 1L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);
    stat = arraylist_remove(list, LEN + 10L, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_INVALID_INDEX);

    arraylist_destroy(list, NULL);
    CU_PASS("testInvalidIndexAccess() - Test Passed");
}

void testArrayListToArray() {

    ArrayList *list;
    Status stat;

    stat = arraylist_new(&list, LEN);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testArrayListToArray() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = arraylist_add(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char **items;
    long len;
    stat = arraylist_toArray(list, (void ***)&items, &len);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(len, LEN);

    for (i = 0; i < len; i++)
        CU_ASSERT_TRUE( strcmp(items[i], array[i]) == 0 );

    free(items);
    arraylist_destroy(list, NULL);
    CU_PASS("testArrayListToArray() - Test Passed");
}

void testArrayListIterator() {

    ArrayList *list;
    Status stat;

    stat = arraylist_new(&list, LEN);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testArrayListIterator() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = arraylist_add(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    Iterator *iter;
    stat = arraylist_iterator(list, &iter);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    char *element;
    i = 0;
    while (iterator_hasNext(iter) == TRUE) {
        stat = iterator_next(iter, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(element, array[i++]) == 0 );
    }

    iterator_destroy(iter);
    arraylist_destroy(list, NULL);
    CU_PASS("testArrayListIterator() - Test Passed");
}

void testArrayListClear() {
    
    ArrayList *list;
    Status stat;

    stat = arraylist_new(&list, LEN);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testArrayListClear() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = arraylist_add(list, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    arraylist_clear(list, NULL);
    validateEmptyArrayList(list);
    arraylist_destroy(list, NULL);

    CU_PASS("testArrayListClear() - Test Passed");
}

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
    CU_add_test(suite, "Invalid Index Access", testInvalidIndexAccess);
    CU_add_test(suite, "ArrayList additions", testInsertions);
    CU_add_test(suite, "ArrayList set", testSetItem);
    CU_add_test(suite, "ArrayList sequential delete", testSequentialDelete);
    CU_add_test(suite, "ArrayList random delete", testRandomDelete);
    CU_add_test(suite, "ArrayList ensure", testEnsureCapacity);
    CU_add_test(suite, "ArrayList trimToSize", testTrimToSize);
    CU_add_test(suite, "ArrayList toArray", testArrayListToArray);
    CU_add_test(suite, "ArrayList iterator", testArrayListIterator);
    CU_add_test(suite, "ArrayList clear", testArrayListClear);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
