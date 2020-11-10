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

typedef struct node {
    struct node *parent;
    struct node *left;
    struct node *right;
    char color;
    void *data;
} Node;

struct treeset {
    int (*cmp)(void *, void *);
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

static Node *allocNode(void *item) {

    Node *node = (Node *)malloc(sizeof(Node));
    if (node != NULL) {
        node->parent = NULL;
        node->left = NULL;
        node->right = NULL;
        node->color = RED;
        node->data = item;
    }

    return node;
}

static Node *findNode(TreeSet *tree, void *item) {

    Node *temp = tree->root;
    while (temp != NULL) {
        int cmp = (*tree->cmp)(item, temp->data);
        if (cmp == 0)
            break;
        temp = (cmp < 0) ? temp->left : temp->right;
    }

    return temp;
}

static void rotateLeft(TreeSet *tree, Node *node) {

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

static void rotateRight(TreeSet *tree, Node *node) {

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

static void insertFixup(TreeSet *tree, Node *node) {

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

static void insertNode(TreeSet *tree, Node *node) {

    Node *temp = tree->root, *parent = NULL;
    int cmp = 0;
    tree->size++;

    while (temp != NULL) {
        parent = temp;
        cmp = (*tree->cmp)(node->data, temp->data);
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

Status treeset_add(TreeSet *tree, void *item) {

    if (findNode(tree, item) != NULL)
        return STAT_KEY_ALREADY_EXISTS;

    Node *node = allocNode(item);
    if (node == NULL)
        return STAT_ALLOC_FAILURE;

    insertNode(tree, node);
    insertFixup(tree, node);

    return STAT_SUCCESS;
}

Boolean treeset_contains(TreeSet *tree, void *item) {
    return ( findNode(tree, item) != NULL ) ? TRUE : FALSE;
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

#define UNUSED __attribute__((unused))

static void deleteFixup(TreeSet *tree, Node *node, Node *parent) {

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

static void deleteNode(TreeSet *tree, Node *node, Node **src) {

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
        node->data = splice->data;
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

Status treeset_pollFirst(TreeSet *tree, void **first) {

    if (treeset_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    Node *node = getMin(tree->root);
    *first = node->data;

    Node *temp;
    deleteNode(tree, node, &temp);
    free(temp);

    return STAT_SUCCESS;
}

Status treeset_pollLast(TreeSet *tree, void **last) {

    if (treeset_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    Node *node = getMax(tree->root);
    *last = node->data;

    Node *temp;
    deleteNode(tree, node, &temp);
    free(temp);

    return STAT_SUCCESS;
}

Status treeset_remove(TreeSet *tree, void *item, void (*destructor)(void *)) {

    if (treeset_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    Node *node = findNode(tree, item);
    if (node == NULL)
        return STAT_NOT_FOUND;

    Node *temp;
    deleteNode(tree, node, &temp);
    if (destructor != NULL)
        (*destructor)(temp->data);
    free(node);

    return STAT_SUCCESS;
}

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


#include <stdio.h>

static void printNode(Node *node) {
    char color = (node->color == RED) ? 'R' : 'B';
    fprintf(stdout, "(%d %c)", *((int *)node->data), color);
}

static void preOrder(Node *node) {
    if (node == NULL)
        return;

    printNode(node);
    preOrder(node->left);
    preOrder(node->right);
}

static void inOrder(Node *node) {
    if (node == NULL)
        return;

    inOrder(node->left);
    printNode(node);
    inOrder(node->right);
}

static void postOrder(Node *node) {
    if (node == NULL)
        return;

    postOrder(node->left);
    postOrder(node->right);
    printNode(node);
}

void printTree(TreeSet *ts) {
    fprintf(stdout, "Root: %d, Size: %ld\n", *((int *)ts->root->data), ts->size);

    fprintf(stdout, "Pre-Order: ") ;preOrder(ts->root);
    fprintf(stdout, "\nIn-Order: ") ;inOrder(ts->root);
    fprintf(stdout, "\nPost-Order: ") ;postOrder(ts->root);
    fprintf(stdout, "\n");
}
