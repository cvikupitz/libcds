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
#include "treemap.h"

/* Single item used for testing */
static char *singleKey = "10";
static char *singleValue = "TEN";

/* Collection of items used for testing */
#define LEN 30
static char *orderedKeys[] = {
    "01","02","03","04","05","06","07","08","09","10",
    "11","12","13","14","15","16","17","18","19","20",
    "21","22","23","24","25","26","27","28","29","30"
};

static char *orderedValues[] = {
    "ONE","TWO","THREE","FOUR","FIVE","SIX","SEVEN","EIGHT","NINE","TEN",
    "ELEVEN","TWELVE","THIRTEEN","FOURTEEN","FIFTEEN","SIXTEEN","SEVENTEEN",
    "EIGHTEEN","NINETEEN","TWENTY","TWENTY-ONE","TWENTY-TWO","TWENTY-THREE",
    "TWENTY-FOUR","TWENTY-FIVE","TWENTY-SIX","TWENTY-SEVEN","TWENTY-EIGHT",
    "TWENTY-NINE","THIRTY"
};

/* Comparator function used for comparing items in the tree */
static int treeCmp(void *x, void *y) {
    return strcmp((char *)x, (char *)y);
}

static void validateEmptyTreeMap(TreeMap *tree) {

    Array *arr;
    Iterator *iter;
    TmEntry *entry;
    Status stat;
    Boolean boolean;
    char *element, *temp;
    long size;

    boolean = treemap_containsKey(tree, singleKey);
    CU_ASSERT_EQUAL(boolean, FALSE);
    stat = treemap_firstKey(tree, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treemap_first(tree, &entry);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treemap_lastKey(tree, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treemap_last(tree, &entry);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    stat = treemap_floorKey(tree, singleKey, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treemap_floor(tree, singleKey, &entry);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treemap_ceilingKey(tree, singleKey, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treemap_ceiling(tree, singleKey, &entry);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treemap_lowerKey(tree, singleKey, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treemap_lower(tree, singleKey, &entry);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treemap_higherKey(tree, singleKey, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treemap_higher(tree, singleKey, &entry);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    stat = treemap_pollFirst(tree, (void **)&element, (void **)&temp);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treemap_pollLast(tree, (void **)&element, (void **)&temp);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treemap_remove(tree, singleKey, (void **)&temp);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    size = treemap_size(tree);
    CU_ASSERT_EQUAL(size, 0L);
    boolean = treemap_isEmpty(tree);
    CU_ASSERT_EQUAL(boolean, TRUE);
    stat = treemap_iterator(tree, &iter);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treemap_keyArray(tree, &arr);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treemap_entryArray(tree, &arr);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
}

static void testEmptyTreeMap() {

    TreeMap *tree;
    Status stat;

    stat = treemap_new(&tree, treeCmp, NULL);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyTreeMap() - allocation failure");

    validateEmptyTreeMap(tree);
    treemap_destroy(tree, NULL);

    CU_PASS("testEmptyTreeMap() - Test Passed");
}

static void testSingleKey() {

    TreeMap *tree;
    TmEntry *entry;
    Status stat;
    Boolean boolean;
    long size;
    char *element, *prev;

    stat = treemap_new(&tree, treeCmp, NULL);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleKey() - allocation failure");

    stat = treemap_put(tree, singleKey, singleValue, (void **)&prev);
    CU_ASSERT_EQUAL(stat, STAT_ENTRY_INSERTED);
    boolean = treemap_containsKey(tree, singleKey);
    CU_ASSERT_EQUAL(boolean, TRUE);

    size = treemap_size(tree);
    boolean = treemap_isEmpty(tree);
    CU_ASSERT_EQUAL(size, 1L);
    CU_ASSERT_EQUAL(boolean, FALSE);

    stat = treemap_firstKey(tree, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleKey) == 0 );
    stat = treemap_first(tree, &entry);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(tmentry_getKey(entry), singleKey) == 0 );
    CU_ASSERT_TRUE( strcmp(tmentry_getValue(entry), singleValue) == 0 );

    stat = treemap_lastKey(tree, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleKey) == 0 );
    stat = treemap_last(tree, &entry);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(tmentry_getKey(entry), singleKey) == 0 );
    CU_ASSERT_TRUE( strcmp(tmentry_getValue(entry), singleValue) == 0 );

    stat = treemap_lowerKey(tree, singleKey, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_NOT_FOUND);
    stat = treemap_lower(tree, singleKey, &entry);
    CU_ASSERT_EQUAL(stat, STAT_NOT_FOUND);
    stat = treemap_higherKey(tree, singleKey, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_NOT_FOUND);
    stat = treemap_higher(tree, singleKey, &entry);
    CU_ASSERT_EQUAL(stat, STAT_NOT_FOUND);

    stat = treemap_remove(tree, singleKey, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    validateEmptyTreeMap(tree);
    treemap_destroy(tree, NULL);

    CU_PASS("testSingleKey() - Test Passed");
}

static void testItemSet() {

    TreeMap *tree;
    Status stat;
    FILE *fd;
    char line[2], key[], *item;

    stat = treemap_new(&tree, treeCmp, NULL);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testItemSet() - allocation failure");

    if ((fd = fopen("treemap_items.txt", "r")) == NULL) {
        treemap_destroy(tree, NULL);
        CU_FAIL_FATAL("ERROR: testItemSet() - allocation failure");
    }

    while (fgets(line, sizeof(line), fd)) {
        line[2] = '\0';
        stat = treemap_add(tree, line);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    CU_ASSERT_TRUE( treemap_size(tree) == 20L );
    CU_ASSERT_TRUE( treemap_contains(tree, "00") == FALSE );
    CU_ASSERT_TRUE( treemap_contains(tree, "01") == FALSE );
    CU_ASSERT_TRUE( treemap_contains(tree, "02") == TRUE );
    CU_ASSERT_TRUE( treemap_contains(tree, "03") == FALSE );
    CU_ASSERT_TRUE( treemap_contains(tree, "04") == TRUE );
    CU_ASSERT_TRUE( treemap_contains(tree, "05") == FALSE );
    CU_ASSERT_TRUE( treemap_contains(tree, "06") == TRUE );
    CU_ASSERT_TRUE( treemap_contains(tree, "07") == FALSE );

    treemap_first(tree, (void **)&item);
    CU_ASSERT_TRUE( strcmp(item, "02") == 0 );
    treemap_last(tree, (void **)&item);
    CU_ASSERT_TRUE( strcmp(item, "40") == 0 );

    stat = treemap_floor(tree, "99", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "40") == 0 );
    stat = treemap_floor(tree, "00", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_NOT_FOUND);
    stat = treemap_floor(tree, "07", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "06") == 0 );
    stat = treemap_floor(tree, "16", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "16") == 0 );

    stat = treemap_ceiling(tree, "00", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "02") == 0 );
    stat = treemap_ceiling(tree, "99", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_NOT_FOUND);
    stat = treemap_ceiling(tree, "03", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "04") == 0 );
    stat = treemap_ceiling(tree, "18", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "18") == 0 );

    stat = treemap_lower(tree, "99", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "40") == 0 );
    stat = treemap_lower(tree, "00", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_NOT_FOUND);
    stat = treemap_lower(tree, "25", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "24") == 0 );
    stat = treemap_lower(tree, "32", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "30") == 0 );

    stat = treemap_higher(tree, "00", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "02") == 0 );
    stat = treemap_higher(tree, "99", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_NOT_FOUND);
    stat = treemap_higher(tree, "23", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "24") == 0 );
    stat = treemap_higher(tree, "36", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "38") == 0 );

    rewind(fd);
    while (fgets(line, sizeof(line), fd)) {
        line[2] = '\0';
        stat = treemap_remove(tree, line, free);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    fclose(fd);
    treemap_destroy(tree, free);

    CU_PASS("testItemSet() - Test Passed");
}*/

static void testTreeMapToArray() {

    TreeMap *tree;
    TmEntry *entry;
    Array *keyArray, *entryArray;
    Status stat;
    int i;
    char *prev;

    stat = treemap_new(&tree, treeCmp, NULL);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testTreeMapToArray() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = treemap_put(tree, orderedKeys[i], orderedValues[i], (void **)&prev);
        CU_ASSERT_EQUAL(stat, STAT_ENTRY_INSERTED);
    }

    stat = treemap_keyArray(tree, &keyArray);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(keyArray->len, LEN);

    for (i = 0; i < keyArray->len; i++)
        CU_ASSERT_TRUE( strcmp(orderedKeys[i], keyArray->items[i]) == 0 );
    FREE_ARRAY(keyArray);

    stat = treemap_entryArray(tree, &entryArray);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(entryArray->len, LEN);

    for (i = 0; i < entryArray->len; i++) {
        entry = entryArray->items[i];
        char *key = tmentry_getKey(entry);
        char *value = tmentry_getValue(entry);
        CU_ASSERT_TRUE( strcmp(key, orderedKeys[i]) == 0 );
        CU_ASSERT_TRUE( strcmp(value, orderedValues[i]) == 0 );
    }
    FREE_ARRAY(entryArray);
    treemap_destroy(tree, NULL);

    CU_PASS("testTreeMapToArray() - Test Passed");
}

static void testTreeMapIterator() {

    Iterator *iter;
    TreeMap *tree;
    TmEntry *entry;
    Status stat;
    int i;
    char *prev;

    stat = treemap_new(&tree, treeCmp, NULL);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testTreeMapIterator() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = treemap_put(tree, orderedKeys[i], orderedValues[i], (void **)&prev);
        CU_ASSERT_EQUAL(stat, STAT_ENTRY_INSERTED);
    }

    stat = treemap_iterator(tree, &iter);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    i = 0;
    while (iterator_hasNext(iter) == TRUE) {
        stat = iterator_next(iter, (void **)&entry);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        char *key = tmentry_getKey(entry);
        char *value = tmentry_getValue(entry);
        CU_ASSERT_TRUE( strcmp(key, orderedKeys[i]) == 0 );
        CU_ASSERT_TRUE( strcmp(value, orderedValues[i]) == 0 );
        i++;
    }

    iterator_destroy(iter);
    treemap_destroy(tree, NULL);

    CU_PASS("testTreeMapIterator() - Test Passed");
}

static void testTreeMapClear() {

    TreeMap *tree;
    Status stat;
    int i;
    char *prev;

    stat = treemap_new(&tree, treeCmp, NULL);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testTreeMapClear() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = treemap_put(tree, orderedKeys[i], orderedValues[i], (void **)&prev);
        CU_ASSERT_EQUAL(stat, STAT_ENTRY_INSERTED);
    }

    treemap_clear(tree, NULL);
    validateEmptyTreeMap(tree);
    treemap_destroy(tree, NULL);

    CU_PASS("testTreeMapClear() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite("TreeMap Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "TreeMap - Empty TreeMap", testEmptyTreeMap);
    CU_add_test(suite, "TreeMap - Single Item", testSingleKey);
    //CU_add_test(suite, "TreeMap - Item Set", testItemSet);
    CU_add_test(suite, "TreeMap - Array", testTreeMapToArray);
    CU_add_test(suite, "TreeMap - Iterator", testTreeMapIterator);
    CU_add_test(suite, "TreeMap - Clear", testTreeMapClear);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}