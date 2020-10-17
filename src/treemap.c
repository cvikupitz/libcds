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

struct tm_entry {
    void *key;
    void *value;
};

typedef struct node {
    struct node *left;
    struct node *right;
    TmEntry *entry;
    unsigned int height;
} Node;

struct treemap {
    int (*comparator)(void *, void *);
    void (*keyDestructor)(void *);
    Node *root;
    long size;
};

typedef struct {
    void **items;
    long next;
} TreeIter;

Status treemap_new(TreeMap **tree, int (*comparator)(void *, void *), void (*keyDestructor)(void *)) {

    /* Allocate the struct, check for allocation failures */
    TreeMap *temp = (TreeMap *)malloc(sizeof(TreeMap));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Initialize remaining struct members */
    temp->comparator = comparator;
    temp->keyDestructor = keyDestructor;
    temp->root = NULL;
    temp->size = 0L;
    *tree = temp;

    return STAT_SUCCESS;
}

/*
 * Fetches and returns the node with the specified key, or NULL if the item is
 * not present.
 */
static Node *fetchNode(TreeMap *tree, void *key) {

    Node *temp = tree->root;
    while (temp != NULL) {
        int cmp = (*tree->comparator)(key, temp->entry->key);
        if (cmp == 0)
            break;
        temp = (cmp < 0) ? temp->left : temp->right;
    }

    return temp;
}

#define UNUSED __attribute__((unused))
Status treemap_put(UNUSED TreeMap **tree,UNUSED void *key,UNUSED void *entry,UNUSED void **previous) {return STAT_SUCCESS;}

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

    /* Checks if the tree is currently empty */
    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *minNode = getMin(tree->root);
    *firstKey = minNode->entry->key;

    return STAT_SUCCESS;
}

Status treemap_first(TreeMap *tree, TmEntry **first) {

    /* Checks if the tree is currently empty */
    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *minNode = getMin(tree->root);
    *first = minNode->entry;

    return STAT_SUCCESS;
}

Status treemap_lastKey(TreeMap *tree, void **lastKey) {

    /* Checks if the tree is currently empty */
    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *maxNode = getMax(tree->root);
    *lastKey = maxNode->entry->key;

    return STAT_SUCCESS;
}

Status treemap_last(TreeMap *tree, TmEntry **last) {

    /* Checks if the tree is currently empty */
    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *maxNode = getMax(tree->root);
    *last = maxNode->entry;

    return STAT_SUCCESS;
}

static Node *findFloor(TreeMap *tree, void *key) {

    Node *temp = NULL;
    Node *current = tree->root;

    while (current != NULL) {
        int cmp = (*tree->comparator)(key, current->entry->key);
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

    /* Checks if the tree is currently empty */
    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *floorNode = findFloor(tree, key);
    if (floorNode == NULL)
        return STAT_NOT_FOUND;
    *floorKey = floorNode->entry->key;

    return STAT_SUCCESS;
}

Status treemap_floor(TreeMap *tree, void *key, TmEntry **floor) {

    /* Checks if the tree is currently empty */
    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *floorNode = findFloor(tree, key);
    if (floorNode == NULL)
        return STAT_NOT_FOUND;
    *floor = floorNode->entry;

    return STAT_SUCCESS;
}

static Node *findCeiling(TreeMap *tree, void *key) {

    Node *temp = NULL;
    Node *current = tree->root;

    while (current != NULL) {
        int cmp = (*tree->comparator)(key, current->entry->key);
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

    /* Checks if the tree is currently empty */
    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *ceilingNode = findCeiling(tree, key);
    if (ceilingNode == NULL)
        return STAT_NOT_FOUND;
    *ceilingKey = ceilingNode->entry->key;

    return STAT_SUCCESS;
}

Status treemap_ceiling(TreeMap *tree, void *key, TmEntry **ceiling) {

    /* Checks if the tree is currently empty */
    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *ceilingNode = findCeiling(tree, key);
    if (ceilingNode == NULL)
        return STAT_NOT_FOUND;
    *ceiling = ceilingNode->entry;

    return STAT_SUCCESS;
}

static Node *findLower(TreeMap *tree, void *key) {

    Node *temp = NULL;
    Node *current = tree->root;

    while (current != NULL) {
        int cmp = (*tree->comparator)(key, current->entry->key);
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

    /* Checks if the tree is currently empty */
    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *lowerNode = findLower(tree, key);
    if (lowerNode == NULL)
        return STAT_NOT_FOUND;
    *lowerKey = lowerNode->entry->key;

    return STAT_SUCCESS;
}

Status treemap_lower(TreeMap *tree, void *key, TmEntry **lower) {

    /* Checks if the tree is currently empty */
    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *lowerNode = findLower(tree, key);
    if (lowerNode == NULL)
        return STAT_NOT_FOUND;
    *lower = lowerNode->entry;

    return STAT_SUCCESS;
}

static Node *findHigher(TreeMap *tree, void *key) {

    Node *temp = NULL;
    Node *current = tree->root;

    while (current != NULL) {
        int cmp = (*tree->comparator)(key, current->entry->key);
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

    /* Checks if the tree is currently empty */
    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *higherNode = findHigher(tree, key);
    if (higherNode == NULL)
        return STAT_NOT_FOUND;
    *higherKey = higherNode->entry->key;

    return STAT_SUCCESS;
}

Status treemap_higher(TreeMap *tree, void *key, TmEntry **higher) {

    /* Checks if the tree is currently empty */
    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *higherNode = findHigher(tree, key);
    if (higherNode == NULL)
        return STAT_NOT_FOUND;
    *higher = higherNode->entry;

    return STAT_SUCCESS;
}

Boolean treemap_containsKey(TreeMap *tree, void *key) {
    return ( fetchNode(tree, key) != NULL ) ? TRUE : FALSE;
}

Status treemap_get(TreeMap *tree, void *key, void **value) {

    /* Checks if the tree is currently empty */
    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *temp = fetchNode(tree, key);
    if (temp == NULL)
        return STAT_NOT_FOUND;
    *value = temp->entry->value;

    return STAT_SUCCESS;

}

Status treemap_pollFirst(UNUSED TreeMap *tree,UNUSED void **firstKey,UNUSED void **firstValue) {return STAT_SUCCESS;}

Status treemap_pollLast(UNUSED TreeMap *tree,UNUSED void **lastKey,UNUSED void **lastValue) {return STAT_SUCCESS;}

Status treemap_remove(UNUSED TreeMap *tree,UNUSED void *key,UNUSED void **value) {return STAT_SUCCESS;}

/*
 * Function used to clear out all tree items via a post-order traversal.
 */
static void clearTree(Node *node, void (*keyDestructor)(void *), void (*valueDestructor)(void *)) {

    if (node == NULL)
        return;
    clearTree(node->left, keyDestructor, valueDestructor);
    clearTree(node->right, keyDestructor, valueDestructor);

    /* Free the allocated node */
    if (keyDestructor != NULL)
        (*keyDestructor)(node->entry->key);
    if (valueDestructor != NULL)
        (*valueDestructor)(node->entry->value);
    free(node->entry);
    free(node);
}

void treemap_clear(TreeMap *tree, void (*valueDestructor)(void *)) {
    clearTree(tree->root, tree->keyDestructor, valueDestructor);
    tree->root = NULL;
    tree->size = 0L;
}

long treemap_size(TreeMap *tree) {
    return tree->size;
}

Boolean treemap_isEmpty(TreeMap *tree) {
    return ( tree->size == 0L ) ? TRUE : FALSE;
}

static void populateKeyArray(TreeIter *iter, Node *node) {

    if (node == NULL)
        return;
    populateKeyArray(iter, node->left);
    iter->items[iter->next++] = node->entry->key;
    populateKeyArray(iter, node->right);
}

static void populateEntryArray(TreeIter *iter, Node *node) {

    if (node == NULL)
        return;
    populateEntryArray(iter, node->left);
    iter->items[iter->next++] = node->entry;
    populateEntryArray(iter, node->right);
}

static Status generateKeyArray(TreeMap *tree, void ***array, long *len) {

    size_t bytes;
    void **items = NULL;

    /* Does not create the array if empty */
    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Allocates memory for the array */
    bytes = ( tree->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    /* Populates the array with treeset items */
    TreeIter iter = {items, 0L};
    populateKeyArray(&iter, tree->root);
    *len = tree->size;
    *array = iter.items;

    return STAT_SUCCESS;
}

Status treemap_keyArray(TreeMap *tree, void ***keys, long *len) {
    return generateKeyArray(tree, keys, len);
}

static Status generateEntryArray(TreeMap *tree, TmEntry ***array, long *len) {

    size_t bytes;
    void **items = NULL;

    /* Does not create the array if empty */
    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Allocates memory for the array */
    bytes = ( tree->size * sizeof(void *) );
    items = (TmEntry **)malloc(bytes);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    /* Populates the array with treeset items */
    TreeIter iter = {items, 0L};
    populateEntryArray(&iter, tree->root);
    *len = tree->size;
    *array = iter.items;

    return STAT_SUCCESS;
}

Status treemap_entryArray(TreeMap *tree, TmEntry ***entries, long *len) {
    return generateEntryArray(tree, entries, len);
}

Status treemap_iterator(TreeMap *tree, Iterator **iter) {

    Iterator *temp = NULL;
    TmEntry **items = NULL;
    long len;

    /* Generates the array of treeset items for iterator */
    Status status = generateEntryArray(tree, &items, &len);
    if (status != STAT_SUCCESS)
        return status;

    /* Creates a new iterator with the list items */
    status = iterator_new(&temp, (void **)items, len);
    if (status != STAT_SUCCESS) {
        free(items);
        return status;
    }
    *iter = temp;

    return STAT_SUCCESS;
}

void treemap_destroy(TreeMap *tree, void (*valueDestructor)(void *)) {
    clearTree(tree->root, tree->keyDestructor, valueDestructor);
    free(tree);
}

void *tmentry_getKey(TmEntry *entry) {
    return entry->key;
}

void *tmentry_getValue(TmEntry *entry) {
    return entry->value;
}
