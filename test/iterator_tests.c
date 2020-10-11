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
#include "iterator.h"

void testEmptyIterator() {

    Iterator *iter = NULL;
    Status stat;

    stat = iterator_new(&iter, NULL, 0L);
    if (stat != STAT_SUCCESS)
        CU_FAIL("ERROR: testEmptyIterator() - allocation failure");

    Boolean hasNext = iterator_hasNext(iter);
    CU_ASSERT_EQUAL(hasNext, FALSE);

    void *item;
    stat = iterator_next(iter, &item);
    CU_ASSERT_EQUAL(stat, STAT_ITERATION_END);

    iterator_destroy(iter);
    CU_PASS("testEmptyIterator() - Test Passed.");
}

static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple"};

void testIteration() {

    Iterator *iter = NULL;
    Status stat;

    char **items = (char **)malloc(sizeof(char *) * 6);
    if (items == NULL)
        CU_FAIL("ERROR: testIteration() - allocation failure");
    int i;
    for (i = 0; i < 6; i++)
        items[i] = array[i];

    stat = iterator_new(&iter, (void **)items, 6L);
    if (stat != STAT_SUCCESS) {
        free(items);
        CU_FAIL("ERROR: testIteration() - allocation failure");
    }

    char *item;
    Boolean hasNext;
    for (i = 0; i < 6; i++) {
        hasNext = iterator_hasNext(iter);
        CU_ASSERT_EQUAL(hasNext, TRUE);
        stat = iterator_next(iter, (void **)&item);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
    }

    hasNext = iterator_hasNext(iter);
    CU_ASSERT_EQUAL(hasNext, FALSE);
    stat = iterator_next(iter, (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_ITERATION_END);

    iterator_destroy(iter);
    CU_PASS("testIteration() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Iterator Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "testEmptyIterator", testEmptyIterator);
    CU_add_test(suite, "testIteration", testIteration);
    
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
