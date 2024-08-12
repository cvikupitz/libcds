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
#include "tree_map.h"

/**
 * Struct for an entry in the treemap.
 */
struct tm_entry {
    void *key;              // The entry's key
    void *value;            // The entry's value
};

/**
 * Struct for a node inside the tree.
 */
typedef struct node {
    struct node *parent;        // Pointer to the parent node
    struct node *left;          // Pointer to the left child node
    struct node *right;         // Pointer to the right child node
    char color;                 // The node's current color (RED or BLACK)
    TmEntry *entry;             // Pointer to the treemap entry
} Node;

/*
 * Struct for the treemap ADT.
 */
struct treemap {
    int (*keyCmp)(void *, void *);      // Function for comparing keys in the tree
    void (*keyDxn)(void *);             // Function for destroying treemap keys
    Node *root;                         // Pointer to the tree's root node
    long size;                          // The treemap's current size
};

/*
 * Struct iterator for populating tree elements via tree traversal.
 */
typedef struct {
    void **items;       // Array of treemap items
    long next;          // Next index in the iteration
} TreeIter;

// Value for painting a node RED
#define RED 0
// Value for painting a node BLACK
#define BLACK 1
// Macro for evaluating the color of the given node `n`
#define COLOR(n) ( ( (n) != NULL ) ? n->color : BLACK )

Status treemap_new(TreeMap **tree, int (*keyComparator)(void *, void *),
                   void (*keyDestructor)(void *)) {

    // Allocate the struct, check for allocation failures
    TreeMap *temp = (TreeMap *)malloc(sizeof(TreeMap));
    if (temp == NULL) {
        return ALLOC_FAILURE;
    }

    // Initialize remaining struct members
    temp->keyCmp = keyComparator;
    temp->keyDxn = keyDestructor;
    temp->root = NULL;
    temp->size = 0L;
    *tree = temp;

    return OK;
}

// Macro to check if the treemap `t` is currently empty
#define IS_EMPTY(t)  ( ((t)->size == 0L) ? TRUE : FALSE )

/**
 * Allocates and returns a new node for the treemap with the key-value pairing `key` and `value`.
 */
static Node *_malloc_node(void *key, void *value) {

    // Allocate memory for the tree node
    Node *node = (Node *)malloc(sizeof(Node));
    if (node != NULL) {
        // Allocate memory for the entry
        TmEntry *entry = (TmEntry *)malloc(sizeof(TmEntry));
        if (entry != NULL) {
            // Initializes the node and entry members
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

/**
 * Searches the tree for the key `item` and returns the node, or NULL if no such element exists.
 */
static Node *_find_node(TreeMap *tree, void *item) {

    Node *temp = tree->root;
    while (temp != NULL) {
        int cmp = (*tree->keyCmp)(item, temp->entry->key);
        if (cmp == 0)  {
            break;  // Node is found
        }
        temp = ( cmp < 0 ) ? temp->left : temp->right;
    }

    return temp;
}

/**
 * Performs a single left rotation on the specified node `node` within the treemap `tree`.
 */
static void _rotate_left(TreeMap *tree, Node *node) {

    // Links min node in right subtree to node
    Node *temp = node->right;
    node->right = temp->left;
    if (temp->left != NULL) {
        temp->left->parent = node;
    }

    // Links the nodes to perform the left rotation
    temp->parent = node->parent;
    if (node->parent == NULL) {
        tree->root = temp;
    } else {
        if (node->parent->left == node) {
            node->parent->left = temp;
        } else {
            node->parent->right = temp;
        }
    }

    temp->left = node;
    node->parent = temp;
}

/**
 * Performs a single right rotation on the specified node `node` within the treemap `tree`.
 */
static void _rotate_right(TreeMap *tree, Node *node) {

    // Links max node in left subtree to node
    Node *temp = node->left;
    node->left = temp->right;
    if (temp->right != NULL) {
        temp->right->parent = node;
    }

    // Links the nodes to perform the right rotation
    temp->parent = node->parent;
    if (node->parent == NULL) {
        tree->root = temp;
    } else {
        if (node->parent->left == node) {
            node->parent->left = temp;
        } else {
            node->parent->right = temp;
        }
    }

    temp->right = node;
    node->parent = temp;
}

/**
 * Inserts the specified node `node` into the treemap `tree`.
 */
static void _insert_node(TreeMap *tree, Node *node) {

    Node *temp = tree->root, *parent = NULL;
    int cmp = 0;
    tree->size++;

    // Traverse down to the NIL node where the node is to be placed
    while (temp != NULL) {
        parent = temp;
        cmp = (*tree->keyCmp)(node->entry->key, temp->entry->key);
        temp = ( cmp < 0 ) ? temp->left : temp->right;
    }

    // Links the node into place with the tree
    node->parent = parent;
    if (parent == NULL) {
        tree->root = node;
    } else {
        if (cmp < 0) {
            parent->left = node;
        } else {
            parent->right = node;
        }
    }
}

/**
 * Performs recoloring and rotations on the tree after an insertion to ensure the red-black tree
 * properties are upheld.
 */
static void _insert_fix(TreeMap *tree, Node *node) {

    // Rebalance needed only if the parent is red
    while (COLOR(node->parent) == RED && node->parent->parent != NULL) {
        if (node->parent == node->parent->parent->left) {
            Node *uncle = node->parent->parent->right;
            if (COLOR(uncle) == RED) {

                /*
                 * Case: uncle is red, so need to color parent, uncle and grandparent. Advance up to
                 * grandparent node.
                 */
                node->parent->color = BLACK;
                uncle->color = BLACK;
                uncle->parent->color = RED;
                node = uncle->parent;
            } else {

                /*
                 * Case: uncle is black and node is right child. Advance up to parent node and
                 * perform left rotation.
                 */
                if (node == node->parent->right) {
                    node = node->parent;
                    _rotate_left(tree, node);
                }

                /*
                 * Case: uncle is black and node is left child. Recolor parent, grandparent, and
                 * rotate grandparent right.
                 */
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                _rotate_right(tree, node->parent->parent);
            }
        } else {
            Node *uncle = node->parent->parent->left;
            if (COLOR(uncle) == RED) {

                /*
                 * Case: uncle is red, so need to color parent, uncle and grandparent. Advance up to
                 * grandparent node.
                 */
                node->parent->color = BLACK;
                uncle->color = BLACK;
                uncle->parent->color = RED;
                node = uncle->parent;
            } else {

                /*
                 * Case: uncle is black and node is left child. Advance up to parent node and
                 * perform right rotation.
                 */
                if (node == node->parent->left) {
                    node = node->parent;
                    _rotate_right(tree, node);
                }

                /*
                 * Case: uncle is black and node is left child. Recolor parent, grandparent, and
                 * rotate grandparent left
                 */
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                _rotate_left(tree, node->parent->parent);
            }
        }
    }

    // Tree root node must always be black
    tree->root->color = BLACK;
}

Status treemap_put(TreeMap *tree, void *key, void *value, void **previous) {

    // Searches for the node with the specified key
    Node *node = _find_node(tree, key);
    if (node != NULL) {
        // Replaces the old value with the new value
        *previous = node->entry->value;
        node->entry->value = value;
        return REPLACED;
    }

    // Allocates memory for the new node
    Node *temp = _malloc_node(key, value);
    if (temp == NULL) {
        return ALLOC_FAILURE;
    }

    // Inserts the new entry in the tree
    _insert_node(tree, temp);
    _insert_fix(tree, temp);

    return INSERTED;
}

/**
 * Fetches and returns the node with the minimum value in the given subtree starting at node `node`.
 */
static Node *_get_min(Node *node) {

    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

/*
 * Fetches and returns the node with the maximum value in the given subtree starting at node `node`.
 */
static Node *_get_max(Node *node) {

    while (node->right != NULL) {
        node = node->right;
    }
    return node;
}

Status treemap_firstKey(TreeMap *tree, void **firstKey) {

    // Checks if the tree is currently empty
    if (IS_EMPTY(tree) == TRUE) {
        return STRUCT_EMPTY;
    }
    // Fetches the first key, saves into pointer
    Node *node = _get_min(tree->root);
    *firstKey = node->entry->key;

    return OK;
}

Status treemap_first(TreeMap *tree, TmEntry **first) {

    // Checks if the tree is currently empty
    if (IS_EMPTY(tree) == TRUE) {
        return STRUCT_EMPTY;
    }
    // Fetches the first entry, saves into pointer
    Node *node = _get_min(tree->root);
    *first = node->entry;

    return OK;
}

Status treemap_lastKey(TreeMap *tree, void **lastKey) {

    // Checks if the tree is currently empty
    if (IS_EMPTY(tree) == TRUE) {
        return STRUCT_EMPTY;
    }
    // Fetches the last key, saves into pointer
    Node *node = _get_max(tree->root);
    *lastKey = node->entry->key;

    return OK;
}

Status treemap_last(TreeMap *tree, TmEntry **last) {

    // Checks if the tree is currently empty
    if (IS_EMPTY(tree) == TRUE) {
        return STRUCT_EMPTY;
    }
    // Fetches the last entry, saves into pointer
    Node *node = _get_max(tree->root);
    *last = node->entry;

    return OK;
}

/**
 * Fetches and returns the floor node with respect to the given item, or NULL if no such element
 * exists.
 */
static Node *_get_floor_node(TreeMap *tree, void *item) {

    Node *temp = NULL;
    Node *current = tree->root;

    while (current != NULL) {
        int cmp = (*tree->keyCmp)(item, current->entry->key);
        if (cmp == 0) {
            // Value found, return as floor
            temp = current;
            break;
        }
        if (cmp < 0) {
            current = current->left;
        } else {
            // Potential floor value found
            temp = current;
            current = current->right;
        }
    }

    return temp;
}

Status treemap_floorKey(TreeMap *tree, void *key, void **floorKey) {

    // Checks if the tree is currently empty
    if (IS_EMPTY(tree) == TRUE) {
        return STRUCT_EMPTY;
    }
    // Fetches the floor node
    Node *node = _get_floor_node(tree, key);
    if (node == NULL) {
        return NOT_FOUND;
    }
    // Extracts the floor key, saves into pointer
    *floorKey = node->entry->key;

    return OK;
}

Status treemap_floor(TreeMap *tree, void *key, TmEntry **floor) {

    // Checks if the tree is currently empty
    if (IS_EMPTY(tree) == TRUE) {
        return STRUCT_EMPTY;
    }
    // Fetches the floor node
    Node *node = _get_floor_node(tree, key);
    if (node == NULL) {
        return NOT_FOUND;
    }
    // Extracts the floor entry, saves into pointer
    *floor = node->entry;

    return OK;
}

/**
 * Fetches and returns the ceiling node with respect to the given item, or NULL if no such element
 * exists.
 */
static Node *_get_ceiling_node(TreeMap *tree, void *item) {

    Node *temp = NULL;
    Node *current = tree->root;

    while (current != NULL) {
        int cmp = (*tree->keyCmp)(item, current->entry->key);
        if (cmp == 0) {
            // Value found, return as ceiling
            temp = current;
            break;
        }
        if (cmp > 0) {
            current = current->right;
        } else {
            // Potential ceiling value found
            temp = current;
            current = current->left;
        }
    }

    return temp;
}

Status treemap_ceilingKey(TreeMap *tree, void *key, void **ceilingKey) {

    // Checks if the tree is currently empty
    if (IS_EMPTY(tree) == TRUE) {
        return STRUCT_EMPTY;
    }
    // Fetches the ceiling node
    Node *node = _get_ceiling_node(tree, key);
    if (node == NULL) {
        return NOT_FOUND;
    }
    // Extracts the ceiling key, saves into pointer
    *ceilingKey = node->entry->key;

    return OK;
}

Status treemap_ceiling(TreeMap *tree, void *key, TmEntry **ceiling) {

    // Checks if the tree is currently empty
    if (IS_EMPTY(tree) == TRUE) {
        return STRUCT_EMPTY;
    }
    // Fetches the ceiling node
    Node *node = _get_ceiling_node(tree, key);
    if (node == NULL) {
        return NOT_FOUND;
    }
    // Extracts the ceiling entry, saves into pointer
    *ceiling = node->entry;

    return OK;
}

/*
 * Fetches and returns the lower node with respect to the given item, or NULL if no such element
 * exists.
 */
static Node *_get_lower_node(TreeMap *tree, void *item) {

    Node *temp = NULL;
    Node *current = tree->root;

    while (current != NULL) {
        int cmp = (*tree->keyCmp)(item, current->entry->key);
        if (cmp <= 0) {
            current = current->left;
        } else {
            // Potential lower value found
            temp = current;
            current = current->right;
        }
    }

    return temp;
}

Status treemap_lowerKey(TreeMap *tree, void *key, void **lowerKey) {

    // Checks if the tree is currently empty
    if (IS_EMPTY(tree) == TRUE) {
        return STRUCT_EMPTY;
    }
    // Fetches the lower node
    Node *node = _get_lower_node(tree, key);
    if (node == NULL) {
        return NOT_FOUND;
    }
    // Extracts the lower key, saves into pointer
    *lowerKey = node->entry->key;

    return OK;
}

Status treemap_lower(TreeMap *tree, void *key, TmEntry **lower) {

    // Checks if the tree is currently empty
    if (IS_EMPTY(tree) == TRUE) {
        return STRUCT_EMPTY;
    }
    // Fetches the lower node
    Node *node = _get_lower_node(tree, key);
    if (node == NULL) {
        return NOT_FOUND;
    }
    // Extracts the lower entry, saves into pointer
    *lower = node->entry;

    return OK;
}

/*
 * Fetches and returns the higher node with respect to the given item, or NULL if no such element
 * exists.
 */
static Node *_get_higher_node(TreeMap *tree, void *item) {

    Node *temp = NULL;
    Node *current = tree->root;

    while (current != NULL) {
        int cmp = (*tree->keyCmp)(item, current->entry->key);
        if (cmp >= 0) {
            current = current->right;
        } else {
            // Potential higher value found
            temp = current;
            current = current->left;
        }
    }

    return temp;
}

Status treemap_higherKey(TreeMap *tree, void *key, void **higherKey) {

    // Checks if the tree is currently empty
    if (IS_EMPTY(tree) == TRUE) {
        return STRUCT_EMPTY;
    }
    // Fetches the higher node
    Node *node = _get_higher_node(tree, key);
    if (node == NULL) {
        return NOT_FOUND;
    }
    // Extracts the higher key, saves into pointer
    *higherKey = node->entry->key;

    return OK;
}

Status treemap_higher(TreeMap *tree, void *key, TmEntry **higher) {

    // Checks if the tree is currently empty
    if (IS_EMPTY(tree) == TRUE) {
        return STRUCT_EMPTY;
    }
    // Fetches the higher node
    Node *node = _get_higher_node(tree, key);
    if (node == NULL) {
        return NOT_FOUND;
    }
    // Extracts the higher entry, saves into pointer
    *higher = node->entry;

    return OK;
}

Boolean treemap_containsKey(TreeMap *tree, void *key) {
    return ( _find_node(tree, key) != NULL ) ? TRUE : FALSE;
}

Status treemap_get(TreeMap *tree, void *key, void **value) {

    // Checks if the tree is currently empty
    if (IS_EMPTY(tree) == TRUE) {
        return STRUCT_EMPTY;
    }
    // Fetches the node with the specified key
    Node *node = _find_node(tree, key);
    if (node == NULL) {
        return NOT_FOUND;
    }
    // Retrieves the value, saves into pointer
    *value = node->entry->value;

    return OK;
}

/**
 * Performs recoloring and rotations on the tree after a deletion to ensure the red-black tree
 * properties are upheld.
 */
static void _delete_fixup(TreeMap *tree, Node *node, Node *parent) {

    // Fixup required only if node deleted is black
    while (node != tree->root && COLOR(node) == BLACK) {

        if (node == parent->left) {
            Node *sibling = parent->right;
            /*
             * Case: sibling is red; recolor sibling and parent, then rotate parent left
             */
            if (COLOR(sibling) == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                _rotate_left(tree, parent);
                sibling = parent->right;
            }

            if (COLOR(sibling->left) == BLACK && COLOR(sibling->right) == BLACK) {
                /*
                 * Case: sibling has no red children; recolor sibling, advance to parent node
                 */
                sibling->color = RED;
                node = parent;
                parent = parent->parent;
            } else {
                if (COLOR(sibling->right) == BLACK) {
                    /*
                     * Case: sibling has left red child; recolor sibling & sibling's left child,
                     * rotate sibling right
                     */
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    _rotate_right(tree, sibling);
                    sibling = parent->right;
                }
                /*
                 * Case: sibling has right red child; Recolor parent, sibling, & sibling's right
                 * child & rotate the parent node left
                 */
                sibling->color = parent->color;
                parent->color = BLACK;
                sibling->right->color = BLACK;
                _rotate_left(tree, parent);
                node = tree->root;
            }
        } else {
            Node *sibling = parent->left;
            /*
             * Case: sibling is red; Recolor sibling and parent, then rotate parent right
             */
            if (COLOR(sibling) == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                _rotate_right(tree, parent);
                sibling = parent->left;
            }
            if (COLOR(sibling->right) == BLACK && COLOR(sibling->left) == BLACK) {
                /*
                 * Case: sibling has no red children; Recolor sibling, advance to parent node
                 */
                sibling->color = RED;
                node = parent;
                parent = parent->parent;
            } else {
                if (COLOR(sibling->left) == BLACK) {
                    /*
                     * Case: sibling has right red child; Recolor sibling & sibling's right child,
                     * rotate sibling left
                     */
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    _rotate_left(tree, sibling);
                    sibling = parent->left;
                }
                /*
                 * Case: sibling has left red child; Recolor parent, sibling, & sibling's left child
                 * & rotate the parent node right
                 */
                sibling->color = parent->color;
                parent->color = BLACK;
                sibling->left->color = BLACK;
                _rotate_right(tree, parent);
                node = tree->root;
            }
        }
    }

    node->color = BLACK;
}

/**
 * Deletes the specified node from the tree, then performs a fixup to ensure the red-black tree
 * properties are upheld. The node to be destroyed is then stored into '*src'.
 */
static void _delete_node(TreeMap *tree, Node *node, Node **src) {

    Node *splice, *child;
    tree->size--;

    // Finds the node to be removed, swaps elements within predecessor
    if (node->left == NULL) {
        splice = node;
        child = node->right;
    } else if (node->right == NULL) {
        splice = node;
        child = node->left;
    } else {
        splice = node->left;
        while (splice->right != NULL) {
            splice = splice->right;
        }
        child = splice->left;
        node->entry->key = splice->entry->key;
        node->entry->value = splice->entry->value;
    }

    // Unlinks the node from the tree
    Node *parent = splice->parent;
    if (child != NULL) {
        child->parent = parent;
    }
    if (parent == NULL) {
        tree->root = child;
        *src = splice;
        return;
    }

    // Links up the remaining nodes
    if (splice == parent->left) {
        parent->left = child;
    } else {
        parent->right = child;
    }

    // Performs a fixup on the tree after removal
    if (splice->color == BLACK) {
        _delete_fixup(tree, child, parent);
    }
    *src = splice;
}

Status treemap_pollFirst(TreeMap *tree, void **firstKey, void **firstValue) {

    // Checks if the tree is currently empty
    if (IS_EMPTY(tree) == TRUE) {
        return STRUCT_EMPTY;
    }
    // Retrieves the minimum node, saves the data into the pointers
    Node *node = _get_min(tree->root);
    *firstKey = node->entry->key;
    *firstValue = node->entry->value;

    // Removes the node from the tree, frees the allocated memory
    Node *temp;
    _delete_node(tree, node, &temp);
    free(temp->entry);
    free(temp);

    return OK;
}

Status treemap_pollLast(TreeMap *tree, void **lastKey, void **lastValue) {

    // Checks if the tree is currently empty
    if (IS_EMPTY(tree) == TRUE) {
        return STRUCT_EMPTY;
    }
    // Retrieves the maximum node, saves the data into the pointers
    Node *node = _get_max(tree->root);
    *lastKey = node->entry->key;
    *lastValue = node->entry->value;

    // Removes the node from the tree, frees the allocated memory
    Node *temp;
    _delete_node(tree, node, &temp);
    free(temp->entry);
    free(temp);

    return OK;
}

Status treemap_remove(TreeMap *tree, void *key, void **value) {

    // Checks if the tree is currently empty
    if (treemap_isEmpty(tree) == TRUE) {
        return STRUCT_EMPTY;
    }
    // Fetches the node from the tree, saves the value into the pointer
    Node *node = _find_node(tree, key);
    if (node == NULL) {
        return NOT_FOUND;
    }
    *value = node->entry->value;

    // Removes the node from the tree, frees the allocated memory
    Node *temp;
    void *toDelete = node->entry->key;
    _delete_node(tree, node, &temp);
    if (tree->keyDxn != NULL) {
        (*tree->keyDxn)(toDelete);
    }
    free(temp->entry);
    free(temp);

    return OK;
}

/**
 * Clears out the treemap of all its elements via a post-order traversal, applying the destructor
 * method `keyDxn` on each element's key and `valueDxn` on each element's value (or if NULL, nothing
 * will be done to the key/value). Frees up all reserved memory back to the heap.
 */
static void _clear_tree(Node *node, void (*keyDxn)(void *), void (*valueDxn)(void *)) {

    Node *parent;

    while (node != NULL) {
        parent = node->parent;
        // Node is a leaf node, safe to delete
        if (node->left == NULL && node->right == NULL) {
            // Need to nullify parent's correct child node
            if (parent != NULL) {
                if (parent->left == node) {
                    parent->left = NULL;
                } else {
                    parent->right = NULL;
                }
            }
            // Destroys the node and its entry values
            if (keyDxn != NULL) {
                (*keyDxn)(node->entry->key);
            }
            if (valueDxn != NULL) {
                (*valueDxn)(node->entry->value);
            }
            free(node->entry);
            free(node);
            node = parent;
        } else if (node->left != NULL) {
            // Left child exists, traverse left subtree
            node = node->left;
        } else {
            // Right child exists, traverse right subtree
            node = node->right;
        }
    }
}

void treemap_clear(TreeMap *tree, void (*valueDestructor)(void *)) {
    _clear_tree(tree->root, tree->keyDxn, valueDestructor);
    tree->root = NULL;
    tree->size = 0L;
}

long treemap_size(TreeMap *tree) {
    return tree->size;
}

Boolean treemap_isEmpty(TreeMap *tree) {
    return IS_EMPTY(tree);
}

/**
 * Populates the iteration of treemap keys via an in-order traversal.
 */
static void _populate_key_array(TreeIter *iter, Node *node) {

    if (node == NULL) {
        return;
    }
    _populate_key_array(iter, node->left);
    iter->items[iter->next++] = node->entry->key;
    _populate_key_array(iter, node->right);
}

Status treemap_keyArray(TreeMap *tree, Array **keys) {

    size_t bytes;
    void **items = NULL;

    // Does not create array if currently empty
    if (IS_EMPTY(tree) == TRUE) {
        return STRUCT_EMPTY;
    }

    // Allocates memory for the array
    Array *temp = (Array *)malloc(sizeof(Array));
    if (temp == NULL) {
        return ALLOC_FAILURE;
    }

    // Allocates memory for the internal array of keys
    bytes = ( tree->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL) {
        free(temp);
        return ALLOC_FAILURE;
    }

    // Populates the iterator with the key, saves the results into pointer
    TreeIter iter = {items, 0L};
    _populate_key_array(&iter, tree->root);
    temp->items = iter.items;
    temp->len = tree->size;
    *keys = temp;

    return OK;
}

/**
 * Populates the iteration of treemap entries via an in-order traversal.
 */
static void _populate_entry_array(TreeIter *iter, Node *node) {

    if (node == NULL) {
        return;
    }
    _populate_entry_array(iter, node->left);
    iter->items[iter->next++] = node->entry;
    _populate_entry_array(iter, node->right);
}

/**
 * Helper method to generate an array representation of the tree map. Returns the allocated array
 * with the populated elements, or NULL if failed (allocation error).
 */
static TmEntry **_generate_entry_array(TreeMap *tree) {

    size_t bytes;
    TmEntry **items = NULL;

    // Allocates memory for the array
    bytes = ( tree->size * sizeof(TmEntry *) );
    items = (TmEntry **)malloc(bytes);
    if (items == NULL) {
        return NULL;
    }

    // Collects the array of treemap entries
    TreeIter iter = {(void **)items, 0L};
    _populate_entry_array(&iter, tree->root);

    return (TmEntry **)iter.items;
}

Status treemap_entryArray(TreeMap *tree, Array **entries) {

    // Does not create array if currently empty
    if (IS_EMPTY(tree) == TRUE) {
        return STRUCT_EMPTY;
    }

    // Generates the array of treemap items
    TmEntry **items = _generate_entry_array(tree);
    if (items == NULL) {
        return ALLOC_FAILURE;
    }

    // Allocates memory for the array struct
    Array *temp = (Array *)malloc(sizeof(Array));
    if (temp == NULL) {
        free(items);
        return ALLOC_FAILURE;
    }

    // Initializes the remaining struct members
    temp->items = (void **)items;
    temp->len = tree->size;
    *entries = temp;

    return OK;
}

Status treemap_iterator(TreeMap *tree, Iterator **iter) {

    Iterator *temp = NULL;

    // Does not create the array if empty
    if (IS_EMPTY(tree) == TRUE) {
        return STRUCT_EMPTY;
    }

    // Generates the array of items for iterator
    TmEntry **items = _generate_entry_array(tree);
    if (items == NULL) {
        return ALLOC_FAILURE;
    }

    // Creates a new iterator with the items
    Status status = iterator_new(&temp, (void **)items, tree->size);
    if (status != OK) {
        free(items);
        return status;
    }
    *iter = temp;

    return OK;
}

void treemap_destroy(TreeMap *tree, void (*valueDestructor)(void *)) {
    _clear_tree(tree->root, tree->keyDxn, valueDestructor);
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
