#include <stdio.h>
#include "treemap.h"

/*#define LEN 40
static int orderedSet[] = {
    1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50
};*/

#define LEN 12
static int orderedSet[] = {
    10,18,7,15,16,30,25,40,60,2,1,70
};

static int treeCmp(void *x, void *y) {
    return *((int *)x) - *((int *)y);
}

int main(int argc, char **argv) {

    TreeMap *ts;

    treemap_new(&ts, treeCmp, NULL);

    int i;
    int *prev;
    for (i = 0; i < LEN; i++) {
        treemap_put(ts, &(orderedSet[i]), NULL, (void **)&prev);
    }
    printTree(ts);

    /*int *x, *y;
    while (treemap_pollLast(ts, (void **)&x, (void **)&y) == STAT_SUCCESS)
        printf("polled: %d\n", *x);*/

    treemap_destroy(ts, NULL);

    return 0;
}
