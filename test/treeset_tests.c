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
static char *singleItem = "10";

/* Collection of items used for testing */
#define LEN 30
static char *orderedSet[] = {
    "01","02","03","04","05","06","07","08","09","10",
    "11","12","13","14","15","16","17","18","19","20",
    "21","22","23","24","25","26","27","28","29","30"
};

/* Comparator function used for comparing items in the tree */
static int treeCmp(void *x, void *y) {
    return strcmp((char *)x, (char *)y);
}

static void validateEmptyTreeSet(TreeSet *tree) {

    Array *arr;
    Iterator *iter;
    Status stat;
    Boolean boolean;
    char *element;
    long size;

    boolean = treeset_contains(tree, &singleItem);
    CU_ASSERT_EQUAL(boolean, FALSE);
    stat = treeset_first(tree, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_last(tree, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    stat = treeset_floor(tree, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_ceiling(tree, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_lower(tree, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_higher(tree, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    stat = treeset_pollFirst(tree, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_pollLast(tree, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_remove(tree, &singleItem, NULL);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);

    size = treeset_size(tree);
    CU_ASSERT_EQUAL(size, 0L);
    boolean = treeset_isEmpty(tree);
    CU_ASSERT_EQUAL(boolean, TRUE);
    stat = treeset_iterator(tree, &iter);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
    stat = treeset_toArray(tree, &arr);
    CU_ASSERT_EQUAL(stat, STAT_STRUCT_EMPTY);
}

static void testEmptyTreeSet() {

    TreeSet *tree;
    Status stat;

    stat = treeset_new(&tree, treeCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyTreeSet() - allocation failure");

    validateEmptyTreeSet(tree);
    treeset_destroy(tree, NULL);

    CU_PASS("testEmptyTreeSet() - Test Passed");
}

static void testSingleItem() {

    TreeSet *tree;
    Status stat;
    Boolean boolean;
    long size;
    char *element;

    stat = treeset_new(&tree, treeCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testSingleItem() - allocation failure");

    stat = treeset_add(tree, singleItem);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    boolean = treeset_contains(tree, singleItem);
    CU_ASSERT_EQUAL(boolean, TRUE);

    size = treeset_size(tree);
    boolean = treeset_isEmpty(tree);
    CU_ASSERT_EQUAL(size, 1L);
    CU_ASSERT_EQUAL(boolean, FALSE);

    stat = treeset_first(tree, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );
    stat = treeset_last(tree, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(element, singleItem) == 0 );

    stat = treeset_lower(tree, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_NOT_FOUND);
    stat = treeset_higher(tree, singleItem, (void **)&element);
    CU_ASSERT_EQUAL(stat, STAT_NOT_FOUND);

    stat = treeset_remove(tree, singleItem, NULL);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    validateEmptyTreeSet(tree);
    treeset_destroy(tree, NULL);

    CU_PASS("testSingleItem() - Test Passed");
}

static void testItemSet() {

    TreeSet *tree;
    Status stat;
    FILE *fd;
    char line[4], *item;

    stat = treeset_new(&tree, treeCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testItemSet() - allocation failure");

    if ((fd = fopen("treeset_items.txt", "r")) == NULL) {
        treeset_destroy(tree, NULL);
        CU_FAIL_FATAL("ERROR: testItemSet() - failed to open 'treeset_items.txt");
    }

    while (fgets(line, sizeof(line), fd)) {
        line[2] = '\0';
        stat = treeset_add(tree, strdup(line));
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    CU_ASSERT_TRUE( treeset_size(tree) == 20L );
    CU_ASSERT_TRUE( treeset_contains(tree, "00") == FALSE );
    CU_ASSERT_TRUE( treeset_contains(tree, "01") == FALSE );
    CU_ASSERT_TRUE( treeset_contains(tree, "02") == TRUE );
    CU_ASSERT_TRUE( treeset_contains(tree, "03") == FALSE );
    CU_ASSERT_TRUE( treeset_contains(tree, "04") == TRUE );
    CU_ASSERT_TRUE( treeset_contains(tree, "05") == FALSE );
    CU_ASSERT_TRUE( treeset_contains(tree, "06") == TRUE );
    CU_ASSERT_TRUE( treeset_contains(tree, "07") == FALSE );

    treeset_first(tree, (void **)&item);
    CU_ASSERT_TRUE( strcmp(item, "02") == 0 );
    treeset_last(tree, (void **)&item);
    CU_ASSERT_TRUE( strcmp(item, "40") == 0 );

    stat = treeset_floor(tree, "99", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "40") == 0 );
    stat = treeset_floor(tree, "00", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_NOT_FOUND);
    stat = treeset_floor(tree, "07", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "06") == 0 );
    stat = treeset_floor(tree, "16", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "16") == 0 );

    stat = treeset_ceiling(tree, "00", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "02") == 0 );
    stat = treeset_ceiling(tree, "99", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_NOT_FOUND);
    stat = treeset_ceiling(tree, "03", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "04") == 0 );
    stat = treeset_ceiling(tree, "18", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "18") == 0 );

    stat = treeset_lower(tree, "99", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "40") == 0 );
    stat = treeset_lower(tree, "00", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_NOT_FOUND);
    stat = treeset_lower(tree, "25", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "24") == 0 );
    stat = treeset_lower(tree, "32", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "30") == 0 );

    stat = treeset_higher(tree, "00", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "02") == 0 );
    stat = treeset_higher(tree, "99", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_NOT_FOUND);
    stat = treeset_higher(tree, "23", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "24") == 0 );
    stat = treeset_higher(tree, "36", (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_TRUE( strcmp(item, "38") == 0 );

    rewind(fd);
    while (fgets(line, sizeof(line), fd)) {
        line[2] = '\0';
        stat = treeset_remove(tree, line, free);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    fclose(fd);
    treeset_destroy(tree, NULL);

    CU_PASS("testItemSet() - Test Passed");
}

static void testPollFirst() {

    TreeSet *tree;
    Status stat;
    char *first;
    int i;

    stat = treeset_new(&tree, treeCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testPollFirst() - allocation failure");

    for (i = 0L; i < LEN; i++) {
        stat = treeset_add(tree, orderedSet[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    for (i = 0L; i < LEN; i++) {
        stat = treeset_pollFirst(tree, (void **)&first);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(first, orderedSet[i]) == 0 );
    }

    treeset_destroy(tree, NULL);

    CU_PASS("testPollFirst() - Test Passed");
}

static void testPollLast() {

    TreeSet *tree;
    Status stat;
    char *last;
    int i;

    stat = treeset_new(&tree, treeCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testPollLast() - allocation failure");

    for (i = 0L; i < LEN; i++) {
        stat = treeset_add(tree, orderedSet[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    for (i = LEN - 1; i >= 0; i--) {
        stat = treeset_pollLast(tree, (void **)&last);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(last, orderedSet[i]) == 0 );
    }

    treeset_destroy(tree, NULL);

    CU_PASS("testPollLast() - Test Passed");
}

static void testTreeSetToArray() {

    TreeSet *tree;
    Array *arr;
    Status stat;
    int i;

    stat = treeset_new(&tree, treeCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testTreeSetToArray() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = treeset_add(tree, orderedSet[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    stat = treeset_toArray(tree, &arr);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    CU_ASSERT_EQUAL(arr->len, LEN);

    for (i = 0; i < arr->len; i++)
        CU_ASSERT_TRUE( orderedSet[i] == arr->items[i] );

    FREE_ARRAY(arr)
    treeset_destroy(tree, NULL);

    CU_PASS("testTreeSetToArray() - Test Passed");
}

static void testTreeSetIterator() {

    Iterator *iter;
    TreeSet *tree;
    Status stat;
    int i;
    char *element;

    stat = treeset_new(&tree, treeCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testTreeSetIterator() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = treeset_add(tree, orderedSet[i]);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    stat = treeset_iterator(tree, &iter);
    CU_ASSERT_EQUAL(stat, STAT_SUCCESS);

    i = 0;
    while (iterator_hasNext(iter) == TRUE) {
        stat = iterator_next(iter, (void **)&element);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( element == orderedSet[i++] );
    }

    iterator_destroy(iter);
    treeset_destroy(tree, NULL);

    CU_PASS("testTreeSetIterator() - Test Passed");
}

static void testTreeSetClear() {

    TreeSet *tree;
    Status stat;
    int i;

    stat = treeset_new(&tree, treeCmp);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testTreeSetClear() - allocation failure");

    for (i = 0; i < LEN; i++) {
        stat = treeset_add(tree, &(orderedSet[i]));
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
    }

    treeset_clear(tree, NULL);
    validateEmptyTreeSet(tree);
    treeset_destroy(tree, NULL);

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
    CU_add_test(suite, "TreeSet - Single Item", testSingleItem);
    CU_add_test(suite, "TreeSet - Item Set", testItemSet);
    CU_add_test(suite, "TreeSet - Poll First", testPollFirst);
    CU_add_test(suite, "TreeSet - Poll Last", testPollLast);
    CU_add_test(suite, "TreeSet - Array", testTreeSetToArray);
    CU_add_test(suite, "TreeSet - Iterator", testTreeSetIterator);
    CU_add_test(suite, "TreeSet - Clear", testTreeSetClear);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
