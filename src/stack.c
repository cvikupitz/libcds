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
 * The struct for a node inside the stack.
 */
typedef struct node {
    struct node *next;  /* Pointer to the next node */
    void *data;         /* Pointer to hold the element */
} Node;

/*
 * The struct for the stack ADT.
 */
struct stack {
    Node *top;          /* Points to the stack's top node */
    long size;          /* The stack's size */
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
    if (stack_isEmpty(stack) == TRUE)
        return STAT_STRUCT_EMPTY;
    *top = stack->top->data;

    return STAT_SUCCESS;
}

Status stack_pop(Stack *stack, void **top) {

    /* Checks if the stack is empty */
    if (stack_isEmpty(stack) == TRUE)
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
 * Local method to clear out the stack of its elements. If destructor != NULL, it is
 * invoked on each element after removal.
 */
static void clearStack(Stack *stack, void (*destructor)(void *)) {

    void *temp;

    while (stack_pop(stack, &temp) == STAT_SUCCESS)
        if (destructor != NULL)
            (*destructor)(temp);
}

void stack_clear(Stack *stack, void (*destructor)(void *)) {
    clearStack(stack, destructor);
}

long stack_size(Stack *stack) {
    return stack->size;
}

Boolean stack_isEmpty(Stack *stack) {
    return ( stack->size == 0L ) ? TRUE : FALSE;
}

/*
 * Local method to allocate and create an array representation of the stack.
 */
static Status generateArray(Stack *stack, void ***array, long *len) {

    Node *temp = NULL;
    long i = 0L;
    size_t bytes;
    void **items = NULL;

    /* Does not create array if currently empty */
    if (stack_isEmpty(stack) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Allocates memory for the array */
    bytes = ( stack->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    /* Populates the array with the stack items */
    for (temp = stack->top; temp != NULL; temp = temp->next)
        items[i++] = temp->data;
    *array = items;
    *len = stack->size;

    return STAT_SUCCESS;
}

Status stack_toArray(Stack *stack, void ***array, long *len) {
    return generateArray(stack, array, len);
}

Status stack_iterator(Stack *stack, Iterator **iter) {

    Iterator *temp = NULL;
    void **items = NULL;
    long len;

    /* Generates the array of stack items for iterator */
    Status status = generateArray(stack, &items, &len);
    if (status != STAT_SUCCESS)
        return status;

    /* Creates a new iterator with the stack items */
    status = iterator_new(&temp, items, len);
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
