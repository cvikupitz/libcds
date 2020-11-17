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

/* Collection of items used for testing */
#define LEN 6
static char *array[] = {"red", "orange", "yellow", "green", "blue", "purple"};

void testEmptyIterator() {

    Iterator *iter;
    Status stat;
    char *item;

    stat = iterator_new(&iter, NULL, 0L);
    if (stat != STAT_SUCCESS)
        CU_FAIL_FATAL("ERROR: testEmptyIterator() - allocation failure");

    CU_ASSERT_TRUE( iterator_hasNext(iter) == FALSE );
    stat = iterator_next(iter, (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_ITERATION_END);

    iterator_destroy(iter);

    CU_PASS("testEmptyIterator() - Test Passed.");
}

void testIteration() {

    Iterator *iter;
    Status stat;
    char *item, **items;
    int i;

    items = (char **)malloc(sizeof(char *) * 6);
    if (items == NULL)
        CU_FAIL_FATAL("ERROR: testIteration() - allocation failure");
    for (i = 0; i < LEN; i++)
        items[i] = array[i];

    stat = iterator_new(&iter, (void **)items, 6L);
    if (stat != STAT_SUCCESS) {
        free(items);
        CU_FAIL_FATAL("ERROR: testIteration() - allocation failure");
    }

    for (i = 0; i < LEN; i++) {
        CU_ASSERT_TRUE( iterator_hasNext(iter) == TRUE);
        stat = iterator_next(iter, (void **)&item);
        CU_ASSERT_EQUAL(stat, STAT_SUCCESS);
        CU_ASSERT_TRUE( strcmp(item, array[i]) == 0 );
    }

    CU_ASSERT_TRUE( iterator_hasNext(iter) == FALSE );
    stat = iterator_next(iter, (void **)&item);
    CU_ASSERT_EQUAL(stat, STAT_ITERATION_END);

    iterator_destroy(iter);

    CU_PASS("testIteration() - Test Passed");
}

#define UNUSED __attribute__((unused))
int main(UNUSED int argc, UNUSED char **argv) {

    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite("Iterator Tests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "Iterator - Empty", testEmptyIterator);
    CU_add_test(suite, "Iterator - Full Set", testIteration);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
