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
#include "hash_map.h"

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

/*
 * Hash function used by the hashmap.
 */
#define PRIME 7L
static long hash(void *key, long N) {

    long val = 0L;
    char *ch;

    for (ch = key; *ch != '\0'; ch++)
        val = (*ch + (val * PRIME)) % N;
    return val;
}

/*
 * Comparator function for the hashmap keys.
 */
static int keyCmp(void *this, void *other) {

    return strcmp((char *)this, (char *)other);
}

static void validateEmptyHashMap(HashMap *map) {

    Array *keyArray, *entryArray;
    Iterator *iter;
    char *item;

    CU_ASSERT_TRUE( hashmap_containsKey(map, singleKey) == FALSE );
    CU_ASSERT_TRUE( hashmap_remove(map, singleKey, (void **)&item) == STRUCT_EMPTY );
    CU_ASSERT_TRUE( hashmap_size(map) == 0L );
    CU_ASSERT_TRUE( hashmap_isEmpty(map) == TRUE );
    CU_ASSERT_TRUE( hashmap_iterator(map, &iter) == STRUCT_EMPTY );
    CU_ASSERT_TRUE( hashmap_keyArray(map, &keyArray) == STRUCT_EMPTY );
    CU_ASSERT_TRUE( hashmap_entryArray(map, &entryArray) == STRUCT_EMPTY );
}

static void testEmptyHashMap() {

    HashMap *map;
    Status stat;

    stat = hashmap_new(&map, hash, keyCmp, CAPACITY, LOAD_FACTOR, NULL);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testEmptyHashMap() - allocation failure");

    validateEmptyHashMap(map);
    hashmap_destroy(map, NULL);

    CU_PASS("testEmptyHashMap() - Test Passed");
}

static void testSingleItem() {

    HashMap *map;
    Status stat;
    char *prev;

    stat = hashmap_new(&map, hash, keyCmp, CAPACITY, LOAD_FACTOR, NULL);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    CU_ASSERT_TRUE( hashmap_put(map, singleKey, singleValue, (void **)&prev) == INSERTED );
    CU_ASSERT_TRUE( hashmap_put(map, singleKey, otherValue, (void **)&prev) == REPLACED );
    CU_ASSERT_TRUE( strcmp(singleValue, prev) == 0 );
    CU_ASSERT_TRUE( hashmap_size(map) == 1L );
    CU_ASSERT_TRUE( hashmap_isEmpty(map) == FALSE );
    CU_ASSERT_TRUE( hashmap_containsKey(map, singleKey) == TRUE );
    CU_ASSERT_TRUE( hashmap_get(map, singleKey, (void **)&prev) == OK );
    CU_ASSERT_TRUE( strcmp(otherValue, prev) == 0 );
    CU_ASSERT_TRUE( hashmap_get(map, "???????", (void **)&prev) == NOT_FOUND );
    CU_ASSERT_TRUE( hashmap_remove(map, "Non-present key", (void **)&prev) == NOT_FOUND );
    CU_ASSERT_TRUE( hashmap_remove(map, singleKey, (void **)&prev) == OK );
    CU_ASSERT_TRUE( strcmp(otherValue, prev) == 0 );
    CU_ASSERT_TRUE( hashmap_remove(map, singleKey, (void **)&prev) == STRUCT_EMPTY );

    validateEmptyHashMap(map);
    hashmap_destroy(map, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

static void testHashMapInsertions() {

    HashMap *map;
    Status stat;
    int i;
    char *prev;

    stat = hashmap_new(&map, hash, keyCmp, CAPACITY, LOAD_FACTOR, NULL);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testHashMapInsertions() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE(hashmap_put(map, keys[i], entries[i], (void **)&prev) == INSERTED);

    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE( hashmap_containsKey(map, keys[i]) == TRUE );
        CU_ASSERT_TRUE( hashmap_get(map, keys[i], (void **)&prev) == OK );
        CU_ASSERT_TRUE( strcmp(prev, entries[i]) == 0 );
    }

    CU_ASSERT_TRUE( hashmap_containsKey(map, singleKey) == FALSE );
    CU_ASSERT_TRUE( hashmap_get(map, singleKey, (void **)&prev) == NOT_FOUND );
    hashmap_destroy(map, NULL);

    CU_PASS("testHashMapInsertions() - Test Passed");
}

static void testHashMapReplacements() {

    HashMap *map;
    Status stat;
    int i, j;
    char *prev;

    stat = hashmap_new(&map, hash, keyCmp, CAPACITY, LOAD_FACTOR, NULL);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testHashMapReplacements() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( hashmap_put(map, keys[i], entries[i], (void **)&prev) == INSERTED );

    for (i = 0, j = LEN - 1; i < LEN; i++, j--) {
        CU_ASSERT_TRUE( hashmap_put(map, keys[i], entries[j], (void **)&prev) == REPLACED );
        CU_ASSERT_TRUE( strcmp(prev, entries[i]) == 0 );
    }

    hashmap_destroy(map, NULL);

    CU_PASS("testHashMapReplacements() - Test Passed");
}

static void testHashMapClear() {

    HashMap *map;
    Status stat;
    int i;
    char *prev;

    stat = hashmap_new(&map, hash, keyCmp, CAPACITY, LOAD_FACTOR, NULL);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testHashMapClear() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( hashmap_put(map, keys[i], entries[i], (void **)&prev) == INSERTED );
    hashmap_clear(map, NULL);
    validateEmptyHashMap(map);
    hashmap_destroy(map, NULL);

    CU_PASS("testHashMapClear() - Test Passed");
}

static void testHashMapToArray() {

    HashMap *map;
    Array *keyArray, *entryArray;
    Status stat;
    int i;
    char *prev;

    stat = hashmap_new(&map, hash, keyCmp, CAPACITY, LOAD_FACTOR, NULL);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testHashMapToArray() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( hashmap_put(map, keys[i], entries[i], (void **)&prev) == INSERTED );
    CU_ASSERT_TRUE( hashmap_keyArray(map, &keyArray) == OK );
    CU_ASSERT_TRUE( keyArray->len == LEN );
    CU_ASSERT_TRUE( hashmap_entryArray(map, &entryArray) == OK );
    CU_ASSERT_TRUE( entryArray->len == LEN );

    FREE_ARRAY(keyArray)
    FREE_ARRAY(entryArray)
    hashmap_destroy(map, NULL);

    CU_PASS("testHashMapToArray() - Test Passed");
}

static void testHashMapIterator() {

    HashMap *map;
    HmEntry *entry;
    Iterator *iter;
    Status stat;
    int i;
    char *prev;

    stat = hashmap_new(&map, hash, keyCmp, CAPACITY, LOAD_FACTOR, NULL);
    if (stat != OK)
        CU_FAIL_FATAL("ERROR: testHashMapIterator() - allocation failure");

    for (i = 0; i < LEN; i++)
        CU_ASSERT_TRUE( hashmap_put(map, keys[i], entries[i], (void **)&prev) == INSERTED );
    CU_ASSERT_TRUE( hashmap_iterator(map, &iter) == OK );
    while (iterator_hasNext(iter) == TRUE)
        CU_ASSERT_TRUE( iterator_next(iter, (void **)&entry) == OK );

    iterator_destroy(iter);
    hashmap_destroy(map, NULL);

    CU_PASS("testHashMapIterator() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite("HashMap Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "HashMap - Empty HashMap", testEmptyHashMap);
    CU_add_test(suite, "HashMap - Single Item", testSingleItem);
    CU_add_test(suite, "HashMap - Insertions", testHashMapInsertions);
    CU_add_test(suite, "HashMap - Replacements", testHashMapReplacements);
    CU_add_test(suite, "HashMap - Clear", testHashMapClear);
    CU_add_test(suite, "HashMap - Array", testHashMapToArray);
    CU_add_test(suite, "HashMap - Iterator", testHashMapIterator);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
