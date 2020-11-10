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

#define RED 0
#define BLACK 1
#define COLOR(x) ( ( (x) != NULL ) ? x->color : BLACK )

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

static Node *allocNode(void *key, void *value) {

    Node *node = (Node *)malloc(sizeof(Node));
    if (node != NULL) {
        TmEntry *entry = (TmEntry *)malloc(sizeof(TmEntry));
        if (entry != NULL) {
            node->parent = NULL;
            node->left = NULL;
            node->right = NULL;
            node->color = RED;
            entry->key = key;
            entry->value = value;
            node->entry = entry;
        } else {
            free(node);
            node = NULL;
        }
    }

    return node;
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

static void rotateLeft(TreeMap *tree, Node *node) {

    Node *temp = node->right;
    node->right = temp->left;
    if (temp->left != NULL)
        temp->left->parent = node;

    temp->parent = node->parent;
    if (node->parent == NULL) {
        tree->root = temp;
    } else {
        if (node->parent->left == node)
            node->parent->left = temp;
        else
            node->parent->right = temp;
    }

    temp->left = node;
    node->parent = temp;
}

static void rotateRight(TreeMap *tree, Node *node) {

    Node *temp = node->left;
    node->left = temp->right;
    if (temp->right != NULL)
        temp->right->parent = node;

    temp->parent = node->parent;
    if (node->parent == NULL) {
        tree->root = temp;
    } else {
        if (node->parent->left == node)
            node->parent->left = temp;
        else
            node->parent->right = temp;
    }

    temp->right = node;
    node->parent = temp;
}

static void insertFix(TreeMap *tree, Node *node) {

    while (COLOR(node->parent) == RED) {
        if (node->parent == node->parent->parent->left) {
            Node *uncle = node->parent->parent->right;
            if (COLOR(uncle) == RED) {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                uncle->parent->color = RED;
                node = uncle->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    rotateLeft(tree, node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotateRight(tree, node->parent->parent);
            }
        } else {
            Node *uncle = node->parent->parent->left;
            if (COLOR(uncle) == RED) {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                uncle->parent->color = RED;
                node = uncle->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rotateRight(tree, node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotateLeft(tree, node->parent->parent);
            }
        }
    }

    tree->root->color = BLACK;
}

static void insertNode(TreeMap *tree, Node *node) {

    Node *temp = tree->root, *parent = NULL;
    int cmp = 0;
    tree->size++;

    while (temp != NULL) {
        parent = temp;
        cmp = (*tree->keyCmp)(node->entry->key, temp->entry->key);
        temp = (cmp < 0) ? temp->left : temp->right;
    }

    node->parent = parent;
    if (parent == NULL) {
        tree->root = node;
    } else {
        if (cmp < 0)
            parent->left = node;
        else
            parent->right = node;
    }
}

Status treemap_put(TreeMap *tree, void *key, void *value, void **previous) {

    Node *node = findNode(tree, key);
    if (node != NULL) {
        *previous = node->entry->value;
        node->entry->value = value;
        return STAT_ENTRY_REPLACED;
    }

    Node *temp = allocNode(key, value);
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    insertNode(tree, temp);
    insertFix(tree, temp);

    return STAT_ENTRY_INSERTED;
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

static void deleteFixup(TreeMap *tree, Node *node, Node *parent) {

    while (node != tree->root && COLOR(node) == BLACK) {

        if (node == parent->left) {
            Node *sibling = parent->right;
            if (COLOR(sibling) == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                rotateLeft(tree, parent);
                sibling = parent->right;
            }

            if (COLOR(sibling->left) == BLACK && COLOR(sibling->right) == BLACK) {
                sibling->color = RED;
                node = parent;
                parent = parent->parent;
            } else {
                if (COLOR(sibling->right) == BLACK) {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rotateRight(tree, sibling);
                    sibling = parent->right;
                }
                sibling->color = parent->color;
                parent->color = BLACK;
                sibling->right->color = BLACK;
                rotateLeft(tree, parent);
                node = tree->root;
            }
        } else {
            Node *sibling = parent->left;
            if (COLOR(sibling) == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                rotateRight(tree, parent);
                sibling = parent->left;
            }
            if (COLOR(sibling->right) == BLACK && COLOR(sibling->left) == BLACK) {
                sibling->color = RED;
                node = parent;
                parent = parent->parent;
            } else {
                if (COLOR(sibling->left) == BLACK) {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    rotateLeft(tree, sibling);
                    sibling = parent->left;
                }
                sibling->color = parent->color;
                parent->color = BLACK;
                sibling->left->color = BLACK;
                rotateRight(tree, parent);
                node = tree->root;
            }
        }
    }

    node->color = BLACK;
}

static void deleteNode(TreeMap *tree, Node *node, Node **src) {

    Node *splice, *child;
    tree->size--;

    if (node->left == NULL) {
        splice = node;
        child = node->right;
    } else if (node->right == NULL) {
        splice = node;
        child = node->left;
    } else {
        splice = node->left;
        while (splice->right != NULL)
            splice = splice->right;
        child = splice->left;
        node->entry = splice->entry;
    }

    Node *parent = splice->parent;
    if (child != NULL)
        child->parent = parent;
    if (parent == NULL) {
        tree->root = child;
        *src = splice;
        return;
    }

    if (splice == parent->left)
        parent->left = child;
    else
        parent->right = child;

    if (splice->color == BLACK)
        deleteFixup(tree, child, parent);
    *src = splice;
}

Status treemap_pollFirst(TreeMap *tree, void **firstKey, void **firstValue) {

    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    Node *node = getMin(tree->root);
    *firstKey = node->entry->key;
    *firstValue = node->entry->value;

    Node *temp;
    deleteNode(tree, node, &temp);
    free(temp->entry);
    free(temp);

    return STAT_SUCCESS;
}

Status treemap_pollLast(TreeMap *tree, void **lastKey, void **lastValue) {

    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    Node *node = getMax(tree->root);
    *lastKey = node->entry->key;
    *lastValue = node->entry->value;

    Node *temp;
    deleteNode(tree, node, &temp);
    free(temp->entry);
    free(temp);

    return STAT_SUCCESS;
}

Status treemap_remove(TreeMap *tree, void *key, void **value) {

    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    Node *node = findNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    *value = node->entry->value;

    Node *temp;
    deleteNode(tree, node, &temp);
    if (tree->keyDxn != NULL)
        (*tree->keyDxn)(temp->entry->key);
    free(temp->entry);
    free(temp);

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

static void populateKeyArray(Node *node, TreeIter *iter) {

    if (node == NULL)
        return;
    populateKeyArray(node->left, iter);
    iter->items[iter->next++] = node->entry->key;
    populateKeyArray(node->right, iter);
}

Status treemap_keyArray(TreeMap *tree, void ***keys, long *len) {

    size_t bytes;
    void **items = NULL;

    /* Does not create array if currently empty */
    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Allocates memory for the array */
    bytes = ( tree->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    TreeIter iter = {items, 0L};
    populateKeyArray(tree->root, &iter);
    *keys = iter.items;
    *len = tree->size;

    return STAT_SUCCESS;
}

static void populateEntryArray(Node *node, TreeIter *iter) {

    if (node == NULL)
        return;
    populateKeyArray(node->left, iter);
    iter->items[iter->next++] = node->entry;
    populateKeyArray(node->right, iter);
}

static Status generateEntryArray(TreeMap *tree, TmEntry ***entries, long *len) {

    size_t bytes;
    TmEntry **items = NULL;

    /* Does not create array if currently empty */
    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Allocates memory for the array */
    bytes = ( tree->size * sizeof(TmEntry *) );
    items = (TmEntry **)malloc(bytes);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    TreeIter iter = {(void **)items, 0L};
    populateEntryArray(tree->root, &iter);
    *entries = (TmEntry **)iter.items;
    *len = tree->size;

    return STAT_SUCCESS;
}

Status treemap_entryArray(TreeMap *tree, TmEntry ***entries, long *len) {
    return generateEntryArray(tree, entries, len);
}

Status treemap_iterator(TreeMap *tree, Iterator **iter) {

    Iterator *temp = NULL;
    TmEntry **items = NULL;
    long len;

    /* Generates the array of entry items for iterator */
    Status status = generateEntryArray(tree, &items, &len);
    if (status != STAT_SUCCESS)
        return status;

    /* Creates a new iterator with the stack items */
    status = iterator_new(&temp, (void **)items, len);
    if (status != STAT_SUCCESS) {
        free(items);
        return status;
    }
    *iter = temp;

    return STAT_SUCCESS;
}

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
