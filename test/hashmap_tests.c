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
#include "hashmap.h"

/* Assigned default capacity for hashmap */
#define CAPACITY 4L
/* Assigned default load factor for hashmap */
#define LOAD_FACTOR 0.75f

/* Single key/value itemms used for testing */
static char *singleKey = "Single-Key";
static char *singleValue = "Single-Value";
static char *otherValue = "Other-Value";

/* Collection of keys used for testing */
#define LEN 24
static char *keys[] = {
    "red", "orange", "yellow", "green", "blue", "purple",
    "gray", "brown", "black", "white", "pink", "lime",
    "aqua", "navy", "coral", "teal", "khaki", "sea green",
    "lavender", "indigo", "olive", "tomato", "peach", "maroon"
};

/* Collection of values used for testing */
/* The elements are meant to be matched by index with the list of key test items */
static char *entries[] = {
    "#FF0000", "#FFA500", "#FFFF00", "#008000", "#0000FF", "#800080",
    "#808080", "#A52A2A", "#000000", "#FFFFFF", "#FFC0CB", "#00FF00",
    "#00FFFF", "#000080", "#FF7F50", "#008080", "#F0E68C", "#2E8B57",
    "#E6E6FA", "#4B0082", "#808000", "#FF6347", "#FFDAB9", "#800000"
};

static void validateEmptyHashMap(HashMap *map) {

    Iterator *iter;
    Status stat;
    Boolean isEmpty;
    HmEntry **entryArray;
    char *value, **keyArray;
    long size, len;
    
    stat = hashmap_containsKey(map, singleKey);
    CU_ASSERT_EQUAL(stat, FALSE);
    stat = hashmap_remove(map, singleKey, (void **)&value);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    size = hashmap_size(map);
    CU_ASSERT_EQUAL(size, 0L);

    isEmpty = hashmap_isEmpty(map);
    CU_ASSERT_EQUAL(isEmpty, TRUE);
    
    stat = hashmap_iterator(map, &iter);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    stat = hashmap_keyArray(map, &keyArray, &len);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = hashmap_entryArray(map, &entryArray, &len);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
}

void testEmptyHashMap() {

    HashMap *map;
    Status stat;

    stat = hashmap_new(&map, CAPACITY, LOAD_FACTOR);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyHashMap() - allocation failure");
    
    validateEmptyHashMap(map);
    hashmap_destroy(map, NULL);
    CU_PASS("testEmptyHashMap() - Test Passed");
}

void testSingleItem() {
    
    HashMap *map;
    Status stat;
    Boolean isEmpty, exists;
    long size;
    char *prev;

    stat = hashmap_new(&map, CAPACITY, LOAD_FACTOR);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    stat = hashmap_put(map, singleKey, singleValue, (void **)&prev);
    CU_ASSERT_EQUAL(stat, STAT_ENTRY_INSERTED);
    stat = hashmap_put(map, singleKey, otherValue, (void **)&prev);
    CU_ASSERT_EQUAL(stat, STAT_ENTRY_REPLACED);
    CU_ASSERT_TRUE( strcmp(singleValue, prev) == 0 );

    size = hashmap_size(map);
    isEmpty = hashmap_isEmpty(map);
    CU_ASSERT_EQUAL(size, 1L);
    CU_ASSERT_EQUAL(isEmpty, FALSE);

    exists = hashmap_containsKey(map, singleKey);
    CU_ASSERT_EQUAL(exists, TRUE);
    stat = hashmap_get(map, singleKey, (void **)&prev);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(otherValue, prev) == 0 );
    stat = hashmap_get(map, "???????", (void **)&prev);
    CU_ASSERT_EQUAL(stat, STAT_NOT_FOUND);

    stat = hashmap_remove(map, "Non-present key", (void **)&prev);
    CU_ASSERT_EQUAL(stat, STAT_NOT_FOUND);
    stat = hashmap_remove(map, singleKey, (void **)&prev);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(otherValue, prev) == 0 );
    stat = hashmap_remove(map, singleKey, (void **)&prev);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    validateEmptyHashMap(map);
    hashmap_destroy(map, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

void testHashMapInsertions() {
    
    HashMap *map;
    Status stat;
    Boolean exists;
    int i;
    char *prev;

    stat = hashmap_new(&map, CAPACITY, LOAD_FACTOR);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testHashMapInsertions() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = hashmap_put(map, keys[i], entries[i], (void **)&prev);
        CU_ASSERT_EQUAL(stat, STAT_ENTRY_INSERTED);
    }

    for (i = 0; i < LEN; i++) {
        exists = hashmap_containsKey(map, keys[i]);
        CU_ASSERT_EQUAL(exists, TRUE);
        stat = hashmap_get(map, keys[i], (void **)&prev);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(prev, entries[i]) == 0 );
    }

    exists = hashmap_containsKey(map, singleKey);
    CU_ASSERT_EQUAL(exists, FALSE);
    stat = hashmap_get(map, singleKey, (void **)&prev);
    CU_ASSERT_EQUAL(stat, STAT_NOT_FOUND);

    hashmap_destroy(map, NULL);

    CU_PASS("testHashMapInsertions() - Test Passed");
}

void testHashMapReplacements() {
    
    HashMap *map;
    Status stat;
    int i, j;
    char *prev;

    stat = hashmap_new(&map, CAPACITY, LOAD_FACTOR);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testHashMapReplacements() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = hashmap_put(map, keys[i], entries[i], (void **)&prev);
        CU_ASSERT_EQUAL(stat, STAT_ENTRY_INSERTED);
    }

    for (i = 0, j = LEN - 1; i < LEN; i++, j--) {
        stat = hashmap_put(map, keys[i], entries[j], (void **)&prev);
        CU_ASSERT_EQUAL(stat, STAT_ENTRY_REPLACED);
        CU_ASSERT_TRUE( strcmp(prev, entries[i]) == 0 );
    }

    hashmap_destroy(map, NULL);

    CU_PASS("testHashMapReplacements() - Test Passed");
}

void testHashMapClear() {
    
    HashMap *map;
    Status stat;
    int i;
    char *prev;

    stat = hashmap_new(&map, CAPACITY, LOAD_FACTOR);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testHashMapClear() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = hashmap_put(map, keys[i], entries[i], (void **)&prev);
        CU_ASSERT_EQUAL(stat, STAT_ENTRY_INSERTED);
    }

    hashmap_clear(map, NULL);
    validateEmptyHashMap(map);
    hashmap_destroy(map, NULL);

    CU_PASS("testHashMapClear() - Test Passed");
}

void testHashMapToArray() {

    HashMap *map;
    HmEntry **entryArray;
    Status stat;
    int i;
    long len;
    char *prev, **keyArray;

    stat = hashmap_new(&map, CAPACITY, LOAD_FACTOR);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testHashMapToArray() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = hashmap_put(map, keys[i], entries[i], (void **)&prev);
        CU_ASSERT_EQUAL(stat, STAT_ENTRY_INSERTED);
    }

    stat = hashmap_keyArray(map, &keyArray, &len);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(len, LEN);
    stat = hashmap_entryArray(map, &entryArray, &len);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(len, LEN);

    free(keyArray);
    free(entryArray);
    hashmap_destroy(map, NULL);
    CU_PASS("testHashMapToArray() - Test Passed");
}

void testHashMapIterator() {

    HashMap *map;
    Iterator *iter;
    Status stat;
    int i;
    char *prev;

    stat = hashmap_new(&map, CAPACITY, LOAD_FACTOR);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testHashMapIterator() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = hashmap_put(map, keys[i], entries[i], (void **)&prev);
        CU_ASSERT_EQUAL(stat, STAT_ENTRY_INSERTED);
    }

    stat = hashmap_iterator(map, &iter);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    iterator_destroy(iter);
    hashmap_destroy(map, NULL);
    CU_PASS("testHashMapIterator() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("HashMap Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "Empty hashMap", testEmptyHashMap);
    CU_add_test(suite, "Single item", testSingleItem);
    CU_add_test(suite, "Insertions", testHashMapInsertions);
    CU_add_test(suite, "Replacements", testHashMapReplacements);
    CU_add_test(suite, "HashMap clear", testHashMapClear);
    CU_add_test(suite, "HashMap toArray", testHashMapToArray);
    CU_add_test(suite, "HashMap iterator", testHashMapIterator);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
