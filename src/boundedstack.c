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
#include "boundedstack.h"

/*
 * The struct for the bounded stack ADT.
 */
struct bounded_stack {
    void **data;        /* Array of the stack's elements */
    long top;           /* Index of the stack's top element */
    long capacity;      /* The stack's capacity */
};

/* The default capacity to assign when the capacity given is invalid */
#define DEFAULT_CAPACITY 16L

Status boundedstack_new(BoundedStack **stack, long capacity) {

    /* Allocate the struct, check for allocation failures */
    BoundedStack *temp = (BoundedStack *)malloc(sizeof(BoundedStack));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    /* Initialize capacity, sets up remaining structure members */
    long cap = ( capacity <= 0L ) ? DEFAULT_CAPACITY : capacity;
    size_t bytes = (cap * sizeof(void *));
    void **array = (void **)malloc(bytes);

    if (array == NULL) {
        free(temp);
        return STAT_ALLOC_FAILURE;
    }

    temp->data = array;
    temp->top = -1L;
    temp->capacity = cap;
    *stack = temp;

    return STAT_SUCCESS;
}

Status boundedstack_push(BoundedStack *stack, void *item) {

    /* Checks if the stack is full */
    if (boundedstack_isFull(stack) == TRUE)
        return STAT_STRUCT_FULL;
    stack->data[++stack->top] = item;

    return STAT_SUCCESS;
}

Status boundedstack_peek(BoundedStack *stack, void **top) {

    /* Checks if the stack is empty */
    if (boundedstack_isEmpty(stack) == TRUE)
        return STAT_STRUCT_EMPTY;
    *top = stack->data[stack->top];

    return STAT_SUCCESS;
}

Status boundedstack_pop(BoundedStack *stack, void **top) {

    /* Checks if the stack is empty */
    if (boundedstack_isEmpty(stack) == TRUE)
        return STAT_STRUCT_EMPTY;
    *top = stack->data[stack->top--];

    return STAT_SUCCESS;
}

/*
 * Local method to clear out the stack of its elements. If destructor != NULL, it is
 * invoked on each element after removal.
 */
static void clearStack(BoundedStack *stack, void (*destructor)(void *)) {

    void *temp;

    while (boundedstack_pop(stack, &temp) == STAT_SUCCESS)
        if (destructor != NULL)
            (*destructor)(temp);
}

void boundedstack_clear(BoundedStack *stack, void (*destructor)(void *)) {
    clearStack(stack, destructor);
}

long boundedstack_size(BoundedStack *stack) {
    return ( stack->top + 1 );
}

long boundedstack_capacity(BoundedStack *stack) {
    return stack->capacity;
}

Boolean boundedstack_isEmpty(BoundedStack *stack) {
    return ( stack->top == -1L ) ? TRUE : FALSE;
}

Boolean boundedstack_isFull(BoundedStack *stack) {
    return ( (stack->top + 1) == stack->capacity ) ? TRUE : FALSE;
}

/*
 * Local method to allocate and create an array representation of the stack.
 */
static Status generateArray(BoundedStack *stack, void ***array, long *len) {

    long i, j;
    size_t bytes;
    void **items = NULL;

    /* Does not create array if currently empty */
    if (boundedstack_isEmpty(stack) == TRUE)
        return STAT_STRUCT_EMPTY;

    /* Allocates memory for the array */
    bytes = ( (stack->top + 1) * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return STAT_ALLOC_FAILURE;

    /* Populates the array with the stack items */
    for (i = 0L, j = stack->top; j >= 0L; i++, j--)
        items[i] = stack->data[j];
    *array = items;
    *len = ( stack->top + 1 );

    return STAT_SUCCESS;
}

Status boundedstack_toArray(BoundedStack *stack, void ***array, long *len) {
    return generateArray(stack, array, len);
}

Status boundedstack_iterator(BoundedStack *stack, Iterator **iter) {

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

void boundedstack_destroy(BoundedStack *stack, void (*destructor)(void *)) {
    clearStack(stack, destructor);
    free(stack->data);
    free(stack);
}
