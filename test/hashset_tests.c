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
#include "hashset.h"

#define CAPACITY 4L
#define LOAD_FACTOR 0.75f

static int strCmp(void *a, void *b) {
    return strcmp((char *)a, (char *)b);
}

#define PRIME 7L    /* Prime used in algorithm */
static long hash(void *item, long N) {

    char *key = (char *)item;
    long val = 0L;
    char *ch;

    for (ch = key; *ch != '\0'; ch++)
        val = (*ch + (val * PRIME)) % N;
    return val;
}

static char *singleItem = "Test";

#define LEN 24
static char *array[] = {
    "red", "orange", "yellow", "green", "blue", "purple",
    "gray", "brown", "black", "white", "pink", "lime",
    "aqua", "navy", "coral", "teal", "mustard", "dark green",
    "blue gray", "indigo", "pea green", "amber", "peach", "maroon"
};

static void validateEmptyHashSet(HashSet *set) {

    Status stat;
    
    stat = hashset_contains(set, singleItem);
    CU_ASSERT_EQUAL(stat, FALSE);
    stat = hashset_remove(set, singleItem, NULL);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    long size = hashset_size(set);
    CU_ASSERT_EQUAL(size, 0L);

    Boolean isEmpty = hashset_isEmpty(set);
    CU_ASSERT_EQUAL(isEmpty, TRUE);
    
    Iterator *iter;
    stat = hashset_iterator(set, &iter);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    int **array;
    long len;
    stat = hashset_toArray(set, (void ***)&array, &len);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
}

void testEmptyHashSet() {

    HashSet *set = NULL;
    Status stat;

    stat = hashset_new(&set, hash, strCmp, CAPACITY, LOAD_FACTOR);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyHashSet() - allocation failure");
    
    validateEmptyHashSet(set);
    hashset_destroy(set, NULL);
    CU_PASS("testEmptyHashSet() - Test Passed");
}

void testSingleItem() {
    
    HashSet *set;
    Status stat;

    stat = hashset_new(&set, hash, strCmp, CAPACITY, LOAD_FACTOR);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    stat = hashset_add(set, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    stat = hashset_add(set, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_KEY_ALREADY_EXISTS);

    long size = hashset_size(set);
    Boolean isEmpty = hashset_isEmpty(set);
    CU_ASSERT_EQUAL(size, 1L);
    CU_ASSERT_EQUAL(isEmpty, FALSE);

    Boolean exists = hashset_contains(set, singleItem);
    CU_ASSERT_EQUAL(exists, TRUE);

    stat = hashset_remove(set, "Non-present key", NULL);
    CU_ASSERT_EQUAL(stat, STAT_NOT_FOUND);
    stat = hashset_remove(set, singleItem, NULL);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    stat = hashset_remove(set, singleItem, NULL);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    validateEmptyHashSet(set);
    hashset_destroy(set, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

void testCompleteSet() {

    HashSet *set;
    Status stat;
    Boolean exists;

    stat = hashset_new(&set, hash, strCmp, CAPACITY, LOAD_FACTOR);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testCompleteSet() - allocation failure");

    stat = hashset_add(set, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    int i;
    for (i = 0; i < LEN; i++) {
        stat = hashset_add(set, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        exists = hashset_contains(set, array[i]);
        CU_ASSERT_EQUAL(exists, TRUE);
    }

    for (i = 0; i < LEN; i++) {
        stat = hashset_remove(set, array[i], NULL);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        exists = hashset_contains(set, array[i]);
        CU_ASSERT_EQUAL(exists, FALSE);
    }

    stat = hashset_remove(set, array[0], NULL);
    CU_ASSERT_EQUAL(stat, STAT_NOT_FOUND);

    hashset_destroy(set, NULL);
    CU_PASS("testCompleteSet() - Test Passed");
}

void testHashSetClear() {
    
    HashSet *set;
    Status stat;

    stat = hashset_new(&set, hash, strCmp, CAPACITY, LOAD_FACTOR);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testHashSetClear() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = hashset_add(set, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    hashset_clear(set, NULL);
    validateEmptyHashSet(set);
    hashset_destroy(set, NULL);

    CU_PASS("testHashSetClear() - Test Passed");
}

void testHashSetToArray() {

    HashSet *set;
    Status stat;

    stat = hashset_new(&set, hash, strCmp, CAPACITY, LOAD_FACTOR);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testHashSetToArray() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = hashset_add(set, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    char **items;
    long len;
    stat = hashset_toArray(set, (void ***)&items, &len);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(len, LEN);

    free(items);
    hashset_destroy(set, NULL);
    CU_PASS("testHashSetToArray() - Test Passed");
}

void testHashSetIterator() {

    HashSet *set;
    Status stat;

    stat = hashset_new(&set, hash, strCmp, CAPACITY, LOAD_FACTOR);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testHashSetIterator() - allocation failure");

    int i;
    for (i = 0; i < LEN; i++) {
        stat = hashset_add(set, array[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    Iterator *iter;
    stat = hashset_iterator(set, &iter);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    iterator_destroy(iter);
    hashset_destroy(set, NULL);
    CU_PASS("testHashSetIterator() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("HashSet Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "Empty hashset", testEmptyHashSet);
    CU_add_test(suite, "Single item", testSingleItem);
    CU_add_test(suite, "Full set", testCompleteSet);
    CU_add_test(suite, "Hashset clear", testHashSetClear);
    CU_add_test(suite, "HashSet toArray", testHashSetToArray);
    CU_add_test(suite, "HashSet iterator", testHashSetIterator);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
