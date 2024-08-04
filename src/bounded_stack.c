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
#include "bounded_stack.h"

/*
 * The struct for the bounded stack ADT.
 */
struct bounded_stack {
    void **data;        /* Array of the stack's elements */
    long size;          /* The stack's current size */
    long capacity;      /* The stack's capacity */
};

/* The default capacity to assign when the capacity given is invalid */
#define DEFAULT_CAPACITY 16L

Status boundedstack_new(BoundedStack **stack, long capacity) {

    /* Allocate the struct, check for allocation failures */
    BoundedStack *temp = (BoundedStack *)malloc(sizeof(BoundedStack));
    if (temp == NULL)
        return ALLOC_FAILURE;

    /* Initialize capacity, sets up remaining structure members */
    long cap = ( capacity <= 0L ) ? DEFAULT_CAPACITY : capacity;
    size_t bytes = (cap * sizeof(void *));
    void **array = (void **)malloc(bytes);

    /* Check for allocation failures */
    if (array == NULL) {
        free(temp);
        return ALLOC_FAILURE;
    }

    /* Initialize the remaining struct members */
    long i;
    for (i = 0L; i < cap; i++)
        array[i] = NULL;
    temp->data = array;
    temp->size = 0L;
    temp->capacity = cap;
    *stack = temp;

    return OK;
}

/* Macro to check if the stack is currently empty */
#define IS_EMPTY(x) ( ((x)->size == 0L) ? TRUE : FALSE )
/* Macro to check if the stack is currently full */
#define IS_FULL(x)  ( ((x)->size == (x)->capacity) ? TRUE : FALSE )

Status boundedstack_push(BoundedStack *stack, void *item) {

    /* Checks if the stack is full */
    if (IS_FULL(stack) == TRUE)
        return STRUCT_FULL;
    /* Inserts item into the stack */
    stack->data[stack->size++] = item;

    return OK;
}

Status boundedstack_peek(BoundedStack *stack, void **top) {

    /* Checks if the stack is empty */
    if (IS_EMPTY(stack) == TRUE)
        return STRUCT_EMPTY;
    /* Extracts the top element, saves into pointer */
    *top = stack->data[stack->size - 1];

    return OK;
}

Status boundedstack_pop(BoundedStack *stack, void **top) {

    /* Checks if the stack is empty */
    if (IS_EMPTY(stack) == TRUE)
        return STRUCT_EMPTY;
    /* Removes the item, saves into pointer */
    *top = stack->data[stack->size - 1];
    stack->data[--stack->size] = NULL;

    return OK;
}

/*
 * Clears out the stack of all its elements.
 */
static void clearStack(BoundedStack *stack, void (*destructor)(void *)) {
    long i;
    for (i = 0L; i < stack->size; i++) {
        if (destructor != NULL)
            (*destructor)(stack->data[i]);
        stack->data[i] = NULL;
    }
}

void boundedstack_clear(BoundedStack *stack, void (*destructor)(void *)) {
    clearStack(stack, destructor);
    stack->size = 0L;
}

long boundedstack_size(BoundedStack *stack) {
    return stack->size;
}

long boundedstack_capacity(BoundedStack *stack) {
    return stack->capacity;
}

Boolean boundedstack_isEmpty(BoundedStack *stack) {
    return IS_EMPTY(stack);
}

Boolean boundedstack_isFull(BoundedStack *stack) {
    return IS_FULL(stack);
}

/*
 * Generates and returns an array representation of the stack.
 */
static void **generateArray(BoundedStack *stack) {

    long i, j;
    size_t bytes;
    void **items = NULL;

    /* Allocates memory for the array */
    bytes = (stack->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return NULL;

    /* Populates the array with the stack items */
    for (i = 0L, j = stack->size - 1; j >= 0L; i++, j--)
        items[i] = stack->data[j];

    return items;
}

Status boundedstack_toArray(BoundedStack *stack, Array **array) {

    /* Does not create array if currently empty */
    if (IS_EMPTY(stack) == TRUE)
        return STRUCT_EMPTY;

    /* Generate the array of stack items */
    void **items = generateArray(stack);
    if (items == NULL)
        return ALLOC_FAILURE;

    /* Allocate memory for the array struct */
    Array *temp = (Array *)malloc(sizeof(Array));
    if (temp == NULL) {
        free(items);
        return ALLOC_FAILURE;
    }

    /* Initialize the remainder of the struct members */
    temp->items = items;
    temp->len = stack->size;
    *array = temp;

    return OK;
}

Status boundedstack_iterator(BoundedStack *stack, Iterator **iter) {

    Iterator *temp = NULL;

    /* Does not create array if currently empty */
    if (boundedstack_isEmpty(stack) == TRUE)
        return STRUCT_EMPTY;

    /* Generates the array of items for iterator */
    void **items = generateArray(stack);
    if (items == NULL)
        return ALLOC_FAILURE;

    /* Creates a new iterator with th items */
    Status status = iterator_new(&temp, items, stack->size);
    if (status != OK) {
        free(items);
        return status;
    }
    *iter = temp;

    return OK;
}

void boundedstack_destroy(BoundedStack *stack, void (*destructor)(void *)) {
    clearStack(stack, destructor);
    free(stack->data);
    free(stack);
}
