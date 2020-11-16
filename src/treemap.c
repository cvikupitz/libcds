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

/*
 * Struct for an entry in the treemap.
 */
struct tm_entry {
    void *key;          /* The entry's key */
    void *value;        /* The entry's value */
};

/*
 * Struct for a node inside the tree.
 */
typedef struct node {
    struct node *parent;        /* Pointer to the parent node */
    struct node *left;          /* Pointer to the left child node */
    struct node *right;         /* Pointer to the right child node */
    char color;                 /* The node's current color (RED or BLACK) */
    TmEntry *entry;             /* Pointer to the treemap entry */
} Node;

/*
 * Struct for the treemap ADT.
 */
struct treemap {
    int (*keyCmp)(void *, void *);      /* Function for comparing keys in the tree */
    void (*keyDxn)(void *);             /* Function for destroying treemap keys */
    Node *root;                         /* Pointer to the tree's root node */
    long size;                          /* The treemap's current size */
};

/*
 * Struct iterator for populating tree elements via tree traversal.
 */
typedef struct {
    void **items;       /* Array of treemap items */
    long next;          /* Next index in the iteration */
} TreeIter;

/* Value for painting a node RED */
#define RED 0
/* Value for painting a node BLACK */
#define BLACK 1
/* Macro for evaluating the color of the given node */
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

/* Macro to check if the treemap is currently empty */
#define IS_EMPTY(x)  ( ((x)->size == 0L) ? TRUE : FALSE )

/*
 * Allocates and returns a new node for the treemap.
 */
static Node *allocNode(void *key, void *value) {

    /* Allocate memory for the tree node */
    Node *node = (Node *)malloc(sizeof(Node));
    if (node != NULL) {
        /* Allocate memory for the entry */
        TmEntry *entry = (TmEntry *)malloc(sizeof(TmEntry));
        if (entry != NULL) {
            /* Initializes the node and entry members */
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

/*
 * Searches the tree and returns the node with the specified item.
 */
static Node *findNode(TreeMap *tree, void *item) {

    Node *temp = tree->root;
    while (temp != NULL) {
        int cmp = (*tree->keyCmp)(item, temp->entry->key);
        if (cmp == 0)  /* Node is found */
            break;
        temp = (cmp < 0) ? temp->left : temp->right;
    }

    return temp;
}

/*
 * Performs a single left rotation on the specified node.
 */
static void rotateLeft(TreeMap *tree, Node *node) {

    /* Links min node in right subtree to node */
    Node *temp = node->right;
    node->right = temp->left;
    if (temp->left != NULL)
        temp->left->parent = node;

    /* Links the nodes to perform the left rotation */
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

/*
 * Performs a single right rotation on the specified node
 */
static void rotateRight(TreeMap *tree, Node *node) {

    /* Links max node in left subtree to node */
    Node *temp = node->left;
    node->left = temp->right;
    if (temp->right != NULL)
        temp->right->parent = node;

    /* Links the nodes to perform the right rotation */
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

/*
 * Inserts the specified node into the tree.
 */
static void insertNode(TreeMap *tree, Node *node) {

    Node *temp = tree->root, *parent = NULL;
    int cmp = 0;
    tree->size++;

    /* Traverse down to the NIL node where the node is to be placed */
    while (temp != NULL) {
        parent = temp;
        cmp = (*tree->keyCmp)(node->entry->key, temp->entry->key);
        temp = (cmp < 0) ? temp->left : temp->right;
    }

    /* Links the node into place with the tree */
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

/*
 * Performs recoloring and rotations on the tree after an insertion to
 * ensure the red-black tree properties are upheld.
 */
static void insertFix(TreeMap *tree, Node *node) {

    /* Rebalance needed only if the parent is red */
    while (COLOR(node->parent) == RED) {
        if (node->parent == node->parent->parent->left) {
            Node *uncle = node->parent->parent->right;
            if (COLOR(uncle) == RED) {
                /* Case: uncle is red, so need to color parent, uncle and grandparent */
                /* Advance up to grandparent node */
                node->parent->color = BLACK;
                uncle->color = BLACK;
                uncle->parent->color = RED;
                node = uncle->parent;
            } else {
                /* Case: uncle is black and node is right child */
                /* Advance up to parent node and perform left rotation */
                if (node == node->parent->right) {
                    node = node->parent;
                    rotateLeft(tree, node);
                }
                /* Case: uncle is black and node is left child */
                /* Recolor parent, grandparent, and rotate grandparent right */
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotateRight(tree, node->parent->parent);
            }
        } else {
            Node *uncle = node->parent->parent->left;
            if (COLOR(uncle) == RED) {
                /* Case: uncle is red, so need to color parent, uncle and grandparent */
                /* Advance up to grandparent node */
                node->parent->color = BLACK;
                uncle->color = BLACK;
                uncle->parent->color = RED;
                node = uncle->parent;
            } else {
                /* Case: uncle is black and node is left child */
                /* Advance up to parent node and perform right rotation */
                if (node == node->parent->left) {
                    node = node->parent;
                    rotateRight(tree, node);
                }
                /* Case: uncle is black and node is left child */
                /* Recolor parent, grandparent, and rotate grandparent left */
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotateLeft(tree, node->parent->parent);
            }
        }
    }

    /* Tree root node must always be black */
    tree->root->color = BLACK;
}

Status treemap_put(TreeMap *tree, void *key, void *value, void **previous) {

    /* Searches for the node with the specified key */
    Node *node = findNode(tree, key);
    if (node != NULL) {
        /* Replaces the old value with the new value */
        *previous = node->entry->value;
        node->entry->value = value;
        return STAT_ENTRY_REPLACED;
    }

    /* Allocates memory for the new node */
    Node *temp = allocNode(key, value);
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Inserts the new entry in the tree */
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

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Fetches the first key, saves into pointer */
    Node *node = getMin(tree->root);
    *firstKey = node->entry->key;

    return STAT_SUCCESS;
}

Status treemap_first(TreeMap *tree, TmEntry **first) {

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Fetches the first entry, saves into pointer */
    Node *node = getMin(tree->root);
    *first = node->entry;

    return STAT_SUCCESS;
}

Status treemap_lastKey(TreeMap *tree, void **lastKey) {

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Fetches the last key, saves into pointer */
    Node *node = getMax(tree->root);
    *lastKey = node->entry->key;

    return STAT_SUCCESS;
}

Status treemap_last(TreeMap *tree, TmEntry **last) {

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Fetches the last entry, saves into pointer */
    Node *node = getMax(tree->root);
    *last = node->entry;

    return STAT_SUCCESS;
}

/*
 * Fetches and returns the floor node with respect to the given item.
 */
static Node *getFloorNode(TreeMap *tree, void *item) {

    Node *temp = NULL;
    Node *current = tree->root;

    while (current != NULL) {
        int cmp = (*tree->keyCmp)(item, current->entry->key);
        if (cmp == 0) {
            /* Value found, return as floor */
            temp = current;
            break;
        }
        if (cmp < 0) {
            current = current->left;
        } else {
            /* Potential floor value found */
            temp = current;
            current = current->right;
        }
    }

    return temp;
}

Status treemap_floorKey(TreeMap *tree, void *key, void **floorKey) {

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Fetches the floor node */
    Node *node = getFloorNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    /* Extracts the floor key, saves into pointer */
    *floorKey = node->entry->key;

    return STAT_SUCCESS;
}

Status treemap_floor(TreeMap *tree, void *key, TmEntry **floor) {

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Fetches the floor node */
    Node *node = getFloorNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    /* Extracts the floor entry, saves into pointer */
    *floor = node->entry;

    return STAT_SUCCESS;
}

/*
 * Fetches and returns the ceiling node with respect to the given item.
 */
static Node *getCeilingNode(TreeMap *tree, void *item) {

    Node *temp = NULL;
    Node *current = tree->root;

    while (current != NULL) {
        int cmp = (*tree->keyCmp)(item, current->entry->key);
        if (cmp == 0) {
            /* Value found, return as ceiling */
            temp = current;
            break;
        }
        if (cmp > 0) {
            current = current->right;
        } else {
            /* Potential ceiling value found */
            temp = current;
            current = current->left;
        }
    }

    return temp;
}

Status treemap_ceilingKey(TreeMap *tree, void *key, void **ceilingKey) {

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Fetches the ceiling node */
    Node *node = getCeilingNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    /* Extracts the ceiling key, saves into pointer */
    *ceilingKey = node->entry->key;

    return STAT_SUCCESS;
}

Status treemap_ceiling(TreeMap *tree, void *key, TmEntry **ceiling) {

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Fetches the ceiling node */
    Node *node = getCeilingNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    /* Extracts the ceiling entry, saves into pointer */
    *ceiling = node->entry;

    return STAT_SUCCESS;
}

/*
 * Fetches and returns the lower node with respect to the given item.
 */
static Node *getLowerNode(TreeMap *tree, void *item) {

    Node *temp = NULL;
    Node *current = tree->root;

    while (current != NULL) {
        int cmp = (*tree->keyCmp)(item, current->entry->key);
        if (cmp <= 0) {
            current = current->left;
        } else {
            /* Potential lower value found */
            temp = current;
            current = current->right;
        }
    }

    return temp;
}

Status treemap_lowerKey(TreeMap *tree, void *key, void **lowerKey) {

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Fetches the lower node */
    Node *node = getLowerNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    /* Extracts the lower key, saves into pointer */
    *lowerKey = node->entry->key;

    return STAT_SUCCESS;
}

Status treemap_lower(TreeMap *tree, void *key, TmEntry **lower) {

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Fetches the lower node */
    Node *node = getLowerNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    /* Extracts the lower entry, saves into pointer */
    *lower = node->entry;

    return STAT_SUCCESS;
}

/*
 * Fetches and returns the higher node with respect to the given item.
 */
static Node *getHigherNode(TreeMap *tree, void *item) {

    Node *temp = NULL;
    Node *current = tree->root;

    while (current != NULL) {
        int cmp = (*tree->keyCmp)(item, current->entry->key);
        if (cmp >= 0) {
            current = current->right;
        } else {
            /* Potential higher value found */
            temp = current;
            current = current->left;
        }
    }

    return temp;
}

Status treemap_higherKey(TreeMap *tree, void *key, void **higherKey) {

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Fetches the higher node */
    Node *node = getHigherNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    /* Extracts the higher key, saves into pointer */
    *higherKey = node->entry->key;

    return STAT_SUCCESS;
}

Status treemap_higher(TreeMap *tree, void *key, TmEntry **higher) {

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Fetches the higher node */
    Node *node = getHigherNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    /* Extracts the higher entry, saves into pointer */
    *higher = node->entry;

    return STAT_SUCCESS;
}

Boolean treemap_containsKey(TreeMap *tree, void *key) {
    return ( findNode(tree, key) != NULL ) ? TRUE : FALSE;
}

Status treemap_get(TreeMap *tree, void *key, void **value) {

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Fetches the node with the specified key */
    Node *node = findNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    /* Retrieves the value, saves into pointer */
    *value = node->entry->value;

    return STAT_SUCCESS;
}

/*
 * Performs recoloring and rotations on the tree after a deletion to
 * ensure the red-black tree properties are upheld.
 */
static void deleteFixup(TreeMap *tree, Node *node, Node *parent) {

    /* Fixup required only if node deleted is black */
    while (node != tree->root && COLOR(node) == BLACK) {

        if (node == parent->left) {
            Node *sibling = parent->right;
            /* Case: sibling is red */
            /* Recolor sibling and parent, then rotate parent left */
            if (COLOR(sibling) == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                rotateLeft(tree, parent);
                sibling = parent->right;
            }

            if (COLOR(sibling->left) == BLACK && COLOR(sibling->right) == BLACK) {
                /* Case: sibling has no red children */
                /* Recolor sibling, advance to parent node */
                sibling->color = RED;
                node = parent;
                parent = parent->parent;
            } else {
                if (COLOR(sibling->right) == BLACK) {
                    /* Case: sibling has left red child */
                    /* Recolor sibling & sibling's left child, rotate sibling right */
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rotateRight(tree, sibling);
                    sibling = parent->right;
                }
                /* Case: sibling has right red child */
                /* Recolor parent, sibling, & sibling's right child */
                /* Rotate the parent node left */
                sibling->color = parent->color;
                parent->color = BLACK;
                sibling->right->color = BLACK;
                rotateLeft(tree, parent);
                node = tree->root;
            }
        } else {
            Node *sibling = parent->left;
            /* Case: sibling is red */
            /* Recolor sibling and parent, then rotate parent right */
            if (COLOR(sibling) == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                rotateRight(tree, parent);
                sibling = parent->left;
            }
            if (COLOR(sibling->right) == BLACK && COLOR(sibling->left) == BLACK) {
                /* Case: sibling has no red children */
                /* Recolor sibling, advance to parent node */
                sibling->color = RED;
                node = parent;
                parent = parent->parent;
            } else {
                if (COLOR(sibling->left) == BLACK) {
                    /* Case: sibling has right red child */
                    /* Recolor sibling & sibling's right child, rotate sibling left */
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    rotateLeft(tree, sibling);
                    sibling = parent->left;
                }
                /* Case: sibling has left red child */
                /* Recolor parent, sibling, & sibling's left child */
                /* Rotate the parent node right */
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

/*
 * Deletes the specified node from the tree, then performs a fixup to ensure the
 * red-black tree properties are upheld. The node to be destroyed is then stored
 * into '*src'.
 */
static void deleteNode(TreeMap *tree, Node *node, Node **src) {

    Node *splice, *child;
    tree->size--;

    /* Finds the node to be removed, swaps elements within predecessor */
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
        node->entry->key = splice->entry->key;
        node->entry->value = splice->entry->value;
    }

    /* Unlinks the node from the tree */
    Node *parent = splice->parent;
    if (child != NULL)
        child->parent = parent;
    if (parent == NULL) {
        tree->root = child;
        *src = splice;
        return;
    }

    /* Links up the remaining nodes */
    if (splice == parent->left)
        parent->left = child;
    else
        parent->right = child;

    /* Performs a fixup on the tree after removal */
    if (splice->color == BLACK)
        deleteFixup(tree, child, parent);
    *src = splice;
}

Status treemap_pollFirst(TreeMap *tree, void **firstKey, void **firstValue) {

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Retrieves the minimum node, saves the data into the pointers */
    Node *node = getMin(tree->root);
    *firstKey = node->entry->key;
    *firstValue = node->entry->value;

    /* Removes the node from the tree, frees the allocated memory */
    Node *temp;
    deleteNode(tree, node, &temp);
    free(temp->entry);
    free(temp);

    return STAT_SUCCESS;
}

Status treemap_pollLast(TreeMap *tree, void **lastKey, void **lastValue) {

    /* Checks if the tree is currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Retrieves the maximum node, saves the data into the pointers */
    Node *node = getMax(tree->root);
    *lastKey = node->entry->key;
    *lastValue = node->entry->value;

    /* Removes the node from the tree, frees the allocated memory */
    Node *temp;
    deleteNode(tree, node, &temp);
    free(temp->entry);
    free(temp);

    return STAT_SUCCESS;
}

Status treemap_remove(TreeMap *tree, void *key, void **value) {

    /* Checks if the tree is currently empty */
    if (treemap_isEmpty(tree) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Fetches the node from the tree, saves the value into the pointer */
    Node *node = findNode(tree, key);
    if (node == NULL)
        return STAT_NOT_FOUND;
    *value = node->entry->value;

    /* Removes the node from the tree, frees the allocated memory */
    Node *temp;
    void *toDelete = node->entry->key;
    deleteNode(tree, node, &temp);
    if (tree->keyDxn != NULL)
        (*tree->keyDxn)(toDelete);
    free(temp->entry);
    free(temp);

    return STAT_SUCCESS;
}

/*
 * Clears out the treemap of all its elements via a post-order traversal. Frees up all
 * reserved memory back to the heap.
 */
static void clearTree(Node *node, void (*keyDxn)(void *), void (*valueDxn)(void *)) {

    if (node == NULL)
        return;
    clearTree(node->left, keyDxn, valueDxn);
    clearTree(node->right, keyDxn, valueDxn);

    /* Destroys the node and its entry values */
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
    return IS_EMPTY(tree);
}

/*
 * Populates the iteration of treemap keys via an in-order traversal.
 */
static void populateKeyArray(Node *node, TreeIter *iter) {

    if (node == NULL)
        return;
    populateKeyArray(node->left, iter);
    iter->items[iter->next++] = node->entry->key;
    populateKeyArray(node->right, iter);
}

Status treemap_keyArray(TreeMap *tree, Array **keys) {

    size_t bytes;
    void **items = NULL;

    /* Does not create array if currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Allocates memory for the array */
    Array *temp = (Array *)malloc(sizeof(Array));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Allocates memory for the internal array of keys */
    bytes = ( tree->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL) {
        free(temp);
        return STAT_ALLOC_FAILURE;
    }

    /* Populates the iterator with the key, saves the results into pointer */
    TreeIter iter = {items, 0L};
    populateKeyArray(tree->root, &iter);
    temp->items = iter.items;
    temp->len = tree->size;
    *keys = temp;

    return STAT_SUCCESS;
}

/*
 * Populates the iteration of treemap entries via an in-order traversal.
 */
static void populateEntryArray(Node *node, TreeIter *iter) {

    if (node == NULL)
        return;
    populateEntryArray(node->left, iter);
    iter->items[iter->next++] = node->entry;
    populateEntryArray(node->right, iter);
}

/*
 * Allocates and creates an array representation of the treemap.
 */
static TmEntry **generateEntryArray(TreeMap *tree) {

    size_t bytes;
    TmEntry **items = NULL;

    /* Allocates memory for the array */
    bytes = ( tree->size * sizeof(TmEntry *) );
    items = (TmEntry **)malloc(bytes);
    if (items == NULL)
        return NULL;

    /* Collects the array of treemap entries */
    TreeIter iter = {(void **)items, 0L};
    populateEntryArray(tree->root, &iter);

    return (TmEntry **)iter.items;
}

Status treemap_entryArray(TreeMap *tree, Array **entries) {

    /* Does not create array if currently empty */
    if (IS_EMPTY(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Generates the array of treemap items */
    TmEntry **items = generateEntryArray(tree);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    /* Allocates memory for the array struct */
    Array *temp = (Array *)malloc(sizeof(Array));
    if (temp == NULL) {
        free(items);
        return STAT_ALLOC_FAILURE;
    }

    /* Initializes the remaining struct members */
    temp->items = (void **)items;
    temp->len = tree->size;
    *entries = temp;

    return STAT_SUCCESS;
}

Status treemap_iterator(TreeMap *tree, Iterator **iter) {

    Iterator *temp = NULL;

    /* Does not create the array if empty */
    if (IS_EMPTY(tree) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Generates the array of items for iterator */
    TmEntry **items = generateEntryArray(tree);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    /* Creates a new iterator with the items */
    Status status = iterator_new(&temp, (void **)items, tree->size);
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

void tmentry_setValue(TmEntry *entry, void *value) {
    entry->value = value;
}
