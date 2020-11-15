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
#include "stack.h"

/*
 * Struct for a node inside the stack.
 */
typedef struct node {
    struct node *next;  /* Points to the next node */
    void *data;         /* Pointer that holds the element */
} Node;

/*
 * The struct for the stack ADT.
 */
struct stack {
    Node *top;          /* Pointer to the top element of the stack */
    long size;          /* The stack's current size */
};

Status stack_new(Stack **stack) {

    /* Allocates the struct, check for allocation failure */
    Stack *temp = (Stack *)malloc(sizeof(Stack));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Initializes the stack's struct members */
    temp->top = NULL;
    temp->size = 0L;
    *stack = temp;

    return STAT_SUCCESS;
}

/* Macro to check if the stack is currently empty */
#define IS_EMPTY(x)  ( ((x)->size == 0L) ? TRUE : FALSE )

Status stack_push(Stack *stack, void *item) {

    /* Generate node for pushing item */
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
        return STAT_ALLOC_FAILURE;

    /* Links node to stack, store data into node */
    node->next = stack->top;
    node->data = item;
    stack->top = node;
    stack->size++;

    return STAT_SUCCESS;
}

Status stack_peek(Stack *stack, void **top) {

    /* Checks if the stack is empty */
    if (IS_EMPTY(stack) == TRUE)
        return STAT_STRUCT_EMPTY;
    /* Extracts the top element, saves to pointer */
    *top = stack->top->data;

    return STAT_SUCCESS;
}

Status stack_pop(Stack *stack, void **top) {

    /* Checks if the stack is empty */
    if (IS_EMPTY(stack) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Unlinks the node from stack */
    Node *temp = stack->top;
    stack->top = temp->next;
    *top = temp->data;
    /* Free the allocated node */
    free(temp);
    stack->size--;

    return STAT_SUCCESS;
}

/*
 * Clears out the stack of all its elements.
 */
static void clearStack(Stack *stack, void (*destructor)(void *)) {

    Node *curr = stack->top, *next = NULL;
    long i;

    for (i = 0L; i < stack->size; i++) {
        next = curr->next;
        /* Free all allocated memory */
        if (destructor != NULL)
            (*destructor)(curr->data);
        free(curr);
        curr = next;
    }
}

void stack_clear(Stack *stack, void (*destructor)(void *)) {
    clearStack(stack, destructor);
    stack->top = NULL;
    stack->size = 0L;
}

long stack_size(Stack *stack) {
    return stack->size;
}

Boolean stack_isEmpty(Stack *stack) {
    return IS_EMPTY(stack);
}

/*
 * Generates and returns an array representation of the stack.
 */
static void **generateArray(Stack *stack) {

    Node *temp = NULL;
    long i = 0L;
    size_t bytes;
    void **items = NULL;

    /* Allocates memory for the array */
    bytes = ( stack->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return NULL;

    /* Populates the array with the stack items */
    for (temp = stack->top; temp != NULL; temp = temp->next)
        items[i++] = temp->data;

    return items;
}

Status stack_toArray(Stack *stack, Array **array) {

    /* Checks if the stack is empty */
    if (IS_EMPTY(stack) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Generate the array of stack items */
    void **items = generateArray(stack);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    /* Allocate memory for the array struct */
    Array *temp = (Array *)malloc(sizeof(Array));
    if (temp == NULL) {
        free(items);
        return STAT_ALLOC_FAILURE;
    }

    /* Initialize the array struct members */
    temp->items = items;
    temp->len = stack->size;
    *array = temp;

    return STAT_SUCCESS;
}

Status stack_iterator(Stack *stack, Iterator **iter) {

    Iterator *temp = NULL;

    /* Checks if the stack is empty */
    if (IS_EMPTY(stack) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Generates the array of items for iterator */
    void **items = generateArray(stack);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    /* Creates a new iterator with the items */
    Status status = iterator_new(&temp, items, stack->size);
    if (status != STAT_SUCCESS) {
        free(items);
        return status;
    }
    *iter = temp;

    return STAT_SUCCESS;
}

void stack_destroy(Stack *stack, void (*destructor)(void *)) {
    clearStack(stack, destructor);
    free(stack);
}
