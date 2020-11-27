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

/* Default capacity for the hashset */
#define CAPACITY 4L
/* Default load factor for the hashset */
#define LOAD_FACTOR 0.75f

/* Comparator function used by the hashset */
static int strCmp(void *a, void *b) {
    return strcmp((char *)a, (char *)b);
}

/* Hashing function used by the hashset */
#define PRIME 7L    /* Prime used in algorithm */
static long hash(void *item, long N) {

    char *key = (char *)item;
    long val = 0L;
    char *ch;

    for (ch = key; *ch != '\0'; ch++)
        val = (*ch + (val * PRIME)) % N;
    return val;
}

/* Single item used for testing */
static char *singleItem = "Test";

/* Collection of items used for testing */
#define LEN 24
static char *array[] = {
    "red", "orange", "yellow", "green", "blue", "purple",
    "gray", "brown", "black", "white", "pink", "lime",
    "aqua", "navy", "coral", "teal", "mustard", "dark green",
    "blue gray", "indigo", "pea green", "amber", "peach", "maroon"
};

static void validateEmptyHashSet(HashSet *set) {

    Array *arr;
    Iterator *iter;

    CU_ASSERT_TRUE( hashset_contains(set, singleItem) == FALSE );
    CU_ASSERT_TRUE( hashset_remove(set, singleItem, NULL) == STRUCT_EMPTY );
    CU_ASSERT_TRUE( hashset_size(set) == 0L );
    CU_ASSERT_TRUE( hashset_isEmpty(set) == TRUE );
    CU_ASSERT_TRUE( hashset_iterator(set, &iter) == STRUCT_EMPTY );
    CU_ASSERT_TRUE( hashset_toArray(set, &arr) == STRUCT_EMPTY );
}

static void testEmptyHashSet() {

    HashSet *set;
    Status stat;

    stat = hashset_new(&set, hash, strCmp, CAPACITY, LOAD_FACTOR);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testEmptyHashSet() - allocation failure");

    validateEmptyHashSet(set);
    hashset_destroy(set, NULL);

    CU_PASS("testEmptyHashSet() - Test Passed");
}

static void testSingleItem() {

    HashSet *set;
    Status stat;

    stat = hashset_new(&set, hash, strCmp, CAPACITY, LOAD_FACTOR);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    CU_ASSERT_TRUE( hashset_add(set, singleItem) == OK );
    CU_ASSERT_TRUE( hashset_add(set, singleItem) == ALREADY_EXISTS );
    CU_ASSERT_TRUE( hashset_size(set) == 1L );
    CU_ASSERT_TRUE( hashset_isEmpty(set) == FALSE );
    CU_ASSERT_TRUE( hashset_contains(set, singleItem) == TRUE );
    CU_ASSERT_TRUE( hashset_remove(set, "Non-present key", NULL) == NOT_FOUND );
    CU_ASSERT_TRUE( hashset_remove(set, singleItem, NULL) == OK );
    CU_ASSERT_TRUE( hashset_remove(set, singleItem, NULL) == STRUCT_EMPTY );

    validateEmptyHashSet(set);
    hashset_destroy(set, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

static void testCompleteSet() {

    HashSet *set;
    Status stat;
    int i;

    stat = hashset_new(&set, hash, strCmp, CAPACITY, LOAD_FACTOR);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testCompleteSet() - allocation failure");

    CU_ASSERT_TRUE( hashset_add(set, singleItem) == OK );

    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE( hashset_add(set, array[i]) == OK );
        CU_ASSERT_TRUE( hashset_contains(set, array[i]) == TRUE );
    }

    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE( hashset_remove(set, array[i], NULL) == OK );
        CU_ASSERT_TRUE( hashset_contains(set, array[i]) == FALSE );
    }

    CU_ASSERT_TRUE( hashset_remove(set, array[0], NULL) == NOT_FOUND );
    hashset_destroy(set, NULL);

    CU_PASS("testCompleteSet() - Test Passed");
}

static void testHashSetClear() {

    HashSet *set;
    Status stat;
    int i;

    stat = hashset_new(&set, hash, strCmp, CAPACITY, LOAD_FACTOR);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testHashSetClear() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( hashset_add(set, array[i]) == OK );

    hashset_clear(set, NULL);
    validateEmptyHashSet(set);
    hashset_destroy(set, NULL);

    CU_PASS("testHashSetClear() - Test Passed");
}

static void testHashSetToArray() {

    HashSet *set;
    Array *arr;
    Status stat;
    int i;

    stat = hashset_new(&set, hash, strCmp, CAPACITY, LOAD_FACTOR);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testHashSetToArray() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( hashset_add(set, array[i]) == OK );
    CU_ASSERT_TRUE( hashset_toArray(set, &arr) == OK );
    CU_ASSERT_TRUE( arr->len == LEN );

    FREE_ARRAY(arr)
    hashset_destroy(set, NULL);

    CU_PASS("testHashSetToArray() - Test Passed");
}

static void testHashSetIterator() {

    HashSet *set;
    Iterator *iter;
    Status stat;
    int i;

    stat = hashset_new(&set, hash, strCmp, CAPACITY, LOAD_FACTOR);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testHashSetIterator() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( hashset_add(set, array[i]) == OK );
    CU_ASSERT_TRUE( hashset_iterator(set, &iter) == OK );
    iterator_destroy(iter);
    hashset_destroy(set, NULL);

    CU_PASS("testHashSetIterator() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite("HashSet Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "HashSet - Empty", testEmptyHashSet);
    CU_add_test(suite, "HashSet - Single Item", testSingleItem);
    CU_add_test(suite, "HashSet - Full Set", testCompleteSet);
    CU_add_test(suite, "Hashset - Clear", testHashSetClear);
    CU_add_test(suite, "HashSet - Array", testHashSetToArray);
    CU_add_test(suite, "HashSet - Iterator", testHashSetIterator);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
