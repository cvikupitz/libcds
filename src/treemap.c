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
#include "treemap.h"

#define RED 0
#define BLACK 1

struct tm_entry {
    void *key;
    void *value;
};

typedef struct node {
    struct node *parent;
    struct node *left;
    struct node *right;
    char color;
    TmEntry *entry;
} Node;

struct treemap {
    int (*keyCmp)(void *, void *);
    void (*keyDxn)(void *);
    Node *root;
    long size;
};

/*
 * Makeshift iterator for populating tree elements via tree traversal.
 */
typedef struct {
    void **items;       /* Array of treeset items */
    long next;          /* Next index in iteration */
} TreeIter;

Status treemap_new(TreeMap **tree, int (*keyComparator)(void *, void *), void (*keyDestructor)(void *)) {

    /* Allocate the struct, check for allocation failures */
    TreeMap *temp = (TreeMap *)malloc(sizeof(TreeMap));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Initialize remaining struct members */
    temp->keyCmp = keyComparator;
    temp->keyDxn = keyDestructor;
    temp->root = NULL;
    temp->size = 0L;
    *tree = temp;

    return STAT_SUCCESS;
}

static Node *findNode(TreeMap *tree, void *item) {

    Node *temp = tree->root;
    while (temp != NULL) {
        int cmp = (*tree->keyCmp)(item, temp->entry->key);
        if (cmp == 0)
            break;
        temp = (cmp < 0) ? temp->left : temp->right;
    }

    return temp;
}

#define UNUSED __attribute__((unused))

Status treemap_put(UNUSED TreeMap **tree, UNUSED void *key, UNUSED void *value, UNUSED void **previous) {
    return STAT_SUCCESS;
}

/*
 * Fetches and returns the node with the minimum value in the given subtree.
 */
static Node *getMin(Node *node) {
    while (node->left != NULL)
        node = node->left;
    return node;
}

/*
 * Fetches and returns the node with the maximum value in the given subtree.
 */
static Node *getMax(Node *node) {
    while (node->right != NULL)
        node = node->right;
    return node;
}

Status treemap_firstKey(TreeMap *tree, void **firstKey) {

    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    Node *node = getMin(tree->root);
    *firstKey = node->entry->key;

    return STAT_SUCCESS;
}

Status treemap_first(TreeMap *tree, TmEntry **first) {

    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    Node *node = getMin(tree->root);
    *first = node->entry;

    return STAT_SUCCESS;
}

Status treemap_lastKey(TreeMap *tree, void **lastKey) {

    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    Node *node = getMax(tree->root);
    *lastKey = node->entry->key;

    return STAT_SUCCESS;
}

Status treemap_last(TreeMap *tree, TmEntry **last) {

    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    Node *node = getMax(tree->root);
    *last = node->entry;

    return STAT_SUCCESS;
}

static Node *getFloorNode(TreeMap *tree, void *item) {

    Node *temp = NULL;
    Node *current = tree->root;

    while (current != NULL) {
        int cmp = (*tree->keyCmp)(item, current->entry->key);
        if (cmp == 0) {  /* Value found, return as floor */
            temp = current;
            break;
        }
        if (cmp < 0) {
            temp = current;
            current = current->right;
        } else {
            current = current->left;
        }
    }

    return temp;
}

Status treemap_floorKey(TreeMap *tree, void *key, void **floorKey) {

    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    Node *node = getFloorNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    *floorKey = node->entry->key;

    return STAT_SUCCESS;
}

Status treemap_floor(TreeMap *tree, void *key, TmEntry **floor) {

    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    Node *node = getFloorNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    *floor = node->entry;

    return STAT_SUCCESS;
}

static Node *getCeilingNode(TreeMap *tree, void *item) {

    Node *temp = NULL;
    Node *current = tree->root;

    while (current != NULL) {
        int cmp = (*tree->keyCmp)(item, current->entry->key);
        if (cmp == 0) {  /* Value found, return as ceiling */
            temp = current;
            break;
        }
        if (cmp < 0) {
            current = current->right;
        } else {
            temp = current;
            current = current->left;
        }
    }

    return temp;
}

Status treemap_ceilingKey(TreeMap *tree, void *key, void **ceilingKey) {

    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    Node *node = getCeilingNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    *ceilingKey = node->entry->key;

    return STAT_SUCCESS;
}

Status treemap_ceiling(TreeMap *tree, void *key, TmEntry **ceiling) {

    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    Node *node = getCeilingNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    *ceiling = node->entry;

    return STAT_SUCCESS;
}

static Node *getLowerNode(TreeMap *tree, void *item) {

    Node *temp = NULL;
    Node *current = tree->root;

    while (current != NULL) {
        int cmp = (*tree->keyCmp)(item, current->entry->key);
        if (cmp >= 0) {
            current = current->left;
        } else {
            temp = current;
            current = current->right;
        }
    }

    return temp;
}

Status treemap_lowerKey(TreeMap *tree, void *key, void **lowerKey) {

    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    Node *node = getLowerNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    *lowerKey = node->entry->key;

    return STAT_SUCCESS;
}

Status treemap_lower(TreeMap *tree, void *key, TmEntry **lower) {

    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    Node *node = getLowerNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    *lower = node->entry;

    return STAT_SUCCESS;
}

static Node *getHigherNode(TreeMap *tree, void *item) {

    Node *temp = NULL;
    Node *current = tree->root;

    while (current != NULL) {
        int cmp = (*tree->keyCmp)(item, current->entry->key);
        if (cmp >= 0) {
            temp = current;
            current = current->right;
        } else {
            current = current->left;
        }
    }

    return temp;
}

Status treemap_higherKey(TreeMap *tree, void *key, void **higherKey) {

    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    Node *node = getHigherNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    *higherKey = node->entry->key;

    return STAT_SUCCESS;
}

Status treemap_higher(TreeMap *tree, void *key, TmEntry **higher) {

    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    Node *node = getHigherNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    *higher = node->entry;

    return STAT_SUCCESS;
}

Boolean treemap_containsKey(TreeMap *tree, void *key) {
    return ( findNode(tree, key) != NULL ) ? TRUE : FALSE;
}

Status treemap_get(TreeMap *tree, void *key, void **value) {

    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    Node *node = findNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    *value = node->entry->value;

    return STAT_SUCCESS;
}

Status treemap_pollFirst(UNUSED TreeMap *tree, UNUSED void **firstKey, UNUSED void **firstValue) {
    return STAT_SUCCESS;
}

Status treemap_pollLast(UNUSED TreeMap *tree,UNUSED void **lastKey, UNUSED void **lastValue) {
    return STAT_SUCCESS;
}

Status treemap_remove(UNUSED TreeMap *tree,UNUSED void *key,UNUSED void **value) {
    return STAT_SUCCESS;
}

static void clearTree(Node *node, void (*keyDxn)(void *), void (*valueDxn)(void *)) {

    if (node == NULL)
        return;
    clearTree(node->left, keyDxn, valueDxn);
    clearTree(node->right, keyDxn, valueDxn);

    if (keyDxn != NULL)
        (*keyDxn)(node->entry->key);
    if (valueDxn != NULL)
        (*valueDxn)(node->entry->value);
    free(node->entry);
    free(node);
}

void treemap_clear(TreeMap *tree, void (*valueDestructor)(void *)) {
    clearTree(tree->root, tree->keyDxn, valueDestructor);
    tree->root = NULL;
    tree->size = 0L;
}

long treemap_size(TreeMap *tree) {
    return tree->size;
}

Boolean treemap_isEmpty(TreeMap *tree) {
    return ( tree->size == 0L ) ? TRUE : FALSE;
}

Status treemap_keyArray(TreeMap *tree, void ***keys, long *len);//FIXME

Status treemap_entryArray(TreeMap *tree, TmEntry ***entries, long *len); //FIXME

Status treemap_iterator(TreeMap *tree, Iterator **iter); //FIXME

void treemap_destroy(TreeMap *tree, void (*valueDestructor)(void *)) {
    clearTree(tree->root, tree->keyDxn, valueDestructor);
    free(tree);
}

void *tmentry_getKey(TmEntry *entry) {
    return entry->key;
}

void *tmentry_getValue(TmEntry *entry) {
    return entry->value;
}
