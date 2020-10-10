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
#include "treeset.h"

/*
 * The struct for a node inside the tree
 */
typedef struct node {
    struct node *left;          /* Pointer to left child node */
    struct node *right;         /* Pointer to right child node */
    void *data;                 /* The stored node data */
    unsigned int height;        /* Height of node in tree */
} Node;

/*
 * The struct for the treeset ADT.
 */
struct treeset {
    int (*cmp)(void *, void *); /* Comparator used for comparing items in tree */
    Node *root;                 /* Root node of the tree */
    long size;                  /* The treeset's size */
};

/*
 * Makeshift iterator for populating tree elements via tree traversal.
 */
typedef struct tree_iter {
    void **items;       /* Array of treeset items */
    long next;          /* Next index in iteration */
} TreeIter;

Status treeset_new(TreeSet **tree, int (*comparator)(void *, void *)) {

    /* Allocate the struct, check for allocation failures */
    TreeSet *temp = (TreeSet *)malloc(sizeof(TreeSet));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Initialize remaining struct members */
    temp->cmp = comparator;
    temp->root = NULL;
    temp->size = 0L;
    *tree = temp;

    return STAT_SUCCESS;
}

/*
 * Fetches and returns the node with the specified value, or NULL if the item is
 * not present.
 */
static Node *fetchNode(TreeSet *tree, void *data) {

    Node *temp = tree->root;
    while (temp != NULL) {
        int cmp = (*tree->cmp)(data, temp->data);
        if (cmp == 0)
            break;
        temp = (cmp < 0) ? temp->left : temp->right;
    }

    return temp;
}

/* Returns the height of the given node */
#define HEIGHT(x) (x != NULL) ? x->height : 0

/*
 * Returns the maximum of the two specified values.
 */
static int maximum(unsigned int a, unsigned int b) {
    return (a > b) ? a : b;
}

/*
 * Performs a rotation to the left on the specified node, updates the new
 * heights, then returns the root of the new subtree.
 */
static Node *rotateWithRightChild(Node *node) {

    Node *temp = node->right;
    node->right = temp->left;
    temp->left = node;
    node->height = maximum( HEIGHT(node->left), HEIGHT(node->right) ) + 1;
    temp->height = maximum( HEIGHT(node), HEIGHT(temp->right) ) + 1;

    return temp;
}

/*
 * Performs a rotation to the right on the specified node, updates the new
 * heights, then returns the root of the new subtree.
 */
static Node *rotateWithLeftChild(Node *node) {

    Node *temp = node->left;
    node->left = temp->right;
    temp->right = node;
    node->height = maximum( HEIGHT(node->left), HEIGHT(node->right) ) + 1;
    temp->height = maximum( HEIGHT(temp->left), HEIGHT(node) ) + 1;

    return temp;
}

/*
 * Performs a left-right rotation on the specified node, then returns the
 * root of the new subtree.
 */
static Node *doubleWithLeftChild(Node *node) { 
    node->left = rotateWithRightChild(node->right);
    return rotateWithLeftChild(node);
}

/*
 * Performs a right-left rotation on the specified node, then returns the
 * root of the new subtree.
 */
static Node *doubleWithRightChild(Node *node) {
    node->right = rotateWithLeftChild(node->left);
    return rotateWithRightChild(node);
}

/*
 * Recursive method to insert the node 'node' into the new subtree with the
 * specified root 'temp'. Returns the root of the new modified subtree.
 */
static Node *insertNode(Node *node, Node *temp, int (*cmp)(void *, void *)) {

    if (temp == NULL) {
        temp = node;
    } else if ( (*cmp)(node->data, temp->data) < 0 ) {
        
        temp->left = insertNode(node, temp->left, cmp);
        if ( HEIGHT(temp->left) - HEIGHT(temp->right) == 2 ) {
            if ( (*cmp)(node->data, temp->data) < 0 )
                temp = rotateWithLeftChild(temp);
            else
                return doubleWithLeftChild(temp);
        }
    } else if ( (*cmp)(node->data, temp->data) > 0 ) {
    
        temp->right = insertNode(node, temp->right, cmp);
        if ( HEIGHT(temp->right) - HEIGHT(temp->left) == 2 ) {
            if ( (*cmp)(node->data, temp->data) > 0 )
                temp = rotateWithRightChild(temp);
            else
                temp = doubleWithRightChild(temp);
        }
    } else {
        /*
         * Duplicate entry was found, so do nothing.
         * Should never reach here.
         */
    }

    temp->height = maximum( HEIGHT(temp->left), HEIGHT(temp->right) ) + 1;
    return temp;
}

Status treeset_add(TreeSet *tree, void *item) {

    /* Checks if the element is already present */
    if ( fetchNode(tree, item) != NULL )
        return STAT_KEY_ALREADY_EXISTS;

    /* Allocates the node for insertion */
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
        return STAT_ALLOC_FAILURE;
    node->data = NULL;
    node->left = node->right = NULL;
    node->height = 1;

    /* Inserts the node into the tree */
    tree->root = insertNode(node, tree->root, tree->cmp);
    tree->size++;
    
    return STAT_SUCCESS;
}

Boolean treeset_contains(TreeSet *tree, void *item) {
    return ( fetchNode(tree, item) != NULL ) ? TRUE : FALSE;
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

Status treeset_first(TreeSet *tree, void **first) {

    /* Checks if the tree is currently empty */
    if (treeset_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *minNode = getMin(tree->root);
    *first = minNode->data;

    return STAT_SUCCESS;
}

Status treeset_last(TreeSet *tree, void **last) {

    /* Checks if the tree is currently empty */
    if (treeset_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    Node *maxNode = getMax(tree->root);
    *last = maxNode->data;

    return STAT_SUCCESS;
}

Status treeset_floor(TreeSet *tree, void *item, void **floor) {

    Node *temp = NULL;
    Node *current = tree->root;

    /* Checks if the tree is currently empty */
    if (treeset_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    while (current != NULL) {
        int cmp = (*tree->cmp)(item, current->data);
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

    if (temp == NULL)
        return STAT_NOT_FOUND;
    *floor = temp->data;

    return STAT_SUCCESS;
}

Status treeset_ceiling(TreeSet *tree, void *item, void **ceiling) {

    Node *temp = NULL;
    Node *current = tree->root;

    /* Checks if the tree is currently empty */
    if (treeset_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    while (current != NULL) {
        int cmp = (*tree->cmp)(item, current->data);
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

    if (temp == NULL)
        return STAT_NOT_FOUND;
    *ceiling = temp->data;

    return STAT_SUCCESS;
}

Status treeset_lower(TreeSet *tree, void *item, void **lower) {

    Node *temp = NULL;
    Node *current = tree->root;

    /* Checks if the tree is currently empty */
    if (treeset_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    while (current != NULL) {
        int cmp = (*tree->cmp)(item, current->data);
        if (cmp >= 0) {
            current = current->left;
        } else {
            temp = current;
            current = current->right;
        }
    }

    if (temp == NULL)
        return STAT_NOT_FOUND;
    *lower = temp->data;

    return STAT_SUCCESS;
}

Status treeset_higher(TreeSet *tree, void *item, void **higher) {

    Node *temp = NULL;
    Node *current = tree->root;

    /* Checks if the tree is currently empty */
    if (treeset_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    while (current != NULL) {
        int cmp = (*tree->cmp)(item, current->data);
        if (cmp >= 0) {
            temp = current;
            current = current->right;
        } else {
            current = current->left;
        }
    }

    if (temp == NULL)
        return STAT_NOT_FOUND;
    *higher = temp->data;

    return STAT_SUCCESS;
}

/*
 * Recursive method that removes the specified element from the specified subtree.
 * Returns the root of the new modified subtree.
 */
static Node *deleteNode(Node *node, void *item, int (*cmp)(void *, void *), void (*destructor)(void *)) {

    int res = (*cmp)(node->data, item);
    if (res < 0) {
        node->left = deleteNode(node->left, item, cmp, destructor);
        // FIXME rotations
    } else if (res > 0) {
        node->right = deleteNode(node->right, item, cmp, destructor);
        // FIXME rotations
    } else {
        if (destructor != NULL)
            (*destructor)(node->data);
        free(node);
        node = NULL;
    }
}

Status treeset_pollFirst(TreeSet *tree, void **first) {

    /* Fetches the minimum node from the tree */
    Node *node = getMin(tree->root);
    if (node == NULL)
        return STAT_STRUCT_EMPTY;

    /* Removes the node from the tree */
    *first = node->data;
    tree->root = deleteNode(tree->root, node->data, tree->cmp, NULL);
    tree->size--;

    return STAT_SUCCESS;
}

Status treeset_pollLast(TreeSet *tree, void **last) {

    /* Fetches the maximum node from the tree */
    Node *node = getMax(tree->root);
    if (node == NULL)
        return STAT_STRUCT_EMPTY;

    /* Removes the node from the tree */
    *last = node->data;
    tree->root = deleteNode(tree->root, node->data, tree->cmp, NULL);
    tree->size--;

    return STAT_SUCCESS;
}

Status treeset_remove(TreeSet *tree, void *item, void (*destructor)(void *)) {

    /* Checks if the tree is currently empty */
    if (tree->size == 0L)
        return STAT_STRUCT_EMPTY;

    /* Find the node in the tree, return error if not found */
    Node *node = fetchNode(tree, item);
    if (node == NULL)
        return STAT_NOT_FOUND;

    /* Remove the node from the tree */
    tree->root = deleteNode(tree->root, item, tree->cmp, destructor);
    tree->size--;

    return STAT_SUCCESS;
}

/*
 * Function used to clear out all tree items via a post-order traversal.
 */
static void clearTree(Node *node, void (*destructor)(void *)) {

    if (node == NULL)
        return;
    clearTree(node->left, destructor);
    clearTree(node->right, destructor);

    if (destructor != NULL)
        (*destructor)(node->data);
    free(node);
}

void treeset_clear(TreeSet *tree, void (*destructor)(void *)) {
    clearTree(tree->root, destructor);
    tree->root = NULL;
    tree->size = 0L;
}

long treeset_size(TreeSet *tree) {
    return tree->size;
}

Boolean treeset_isEmpty(TreeSet *tree) {
    return ( tree->size == 0L ) ? TRUE : FALSE;
}

/*
 * Populates the array with treeset items via an in-order traversal.
 */
static void populateArray(TreeIter *iter, Node *node) {

    if (node == NULL)
        return;
    populateArray(iter, node->left);
    iter->items[iter->next++] = node->data;
    populateArray(iter, node->right);
}

/*
 * Local method to allocate and create an array representation of the treeset.
 */
static Status generateArray(TreeSet *tree, void ***array, long *len) {

    size_t bytes;
    void **items = NULL;

    /* Does not create the array if empty */
    if (treeset_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Allocates memory for the array */
    bytes = ( tree->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    /* Populates the array with treeset items */
    TreeIter iter = {items, 0L};
    populateArray(&iter, tree->root);
    *len = tree->size;
    *array = iter.items;

    return STAT_SUCCESS;
}

Status treeset_toArray(TreeSet *tree, void ***array, long *len) {
    return generateArray(tree, array, len);
}

Status treeset_iterator(TreeSet *tree, Iterator **iter) {

    Iterator *temp = NULL;
    void **items = NULL;
    long len;

    /* Generates the array of treeset items for iterator */
    Status status = generateArray(tree, &items, &len);
    if (status != STAT_SUCCESS)
        return status;

    /* Creates a new iterator with the list items */
    status = iterator_new(&temp, items, len);
    if (status != STAT_SUCCESS) {
        free(items);
        return status;
    }
    *iter = temp;

    return STAT_SUCCESS;
}

void treeset_destroy(TreeSet *tree, void (*destructor)(void *)) {
    clearTree(tree->root, destructor);
    free(tree);
}
