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

#include <pthread.h>
#include <stdlib.h>
#include "stack.h"
#include "ts_stack.h"

struct ts_stack {
    pthread_mutex_t lock;
    Stack *instance;
};

#define LOCK(x)    pthread_mutex_lock( &((x)->lock) )
#define UNLOCK(x)  pthread_mutex_unlock( &((x)->lock) )

Status ts_stack_new(ConcurrentStack **stack) {

    ConcurrentStack *temp;
    Status status;
    pthread_mutexattr_t attr;

    temp = (ConcurrentStack *)malloc(sizeof(ConcurrentStack));
    if (temp == NULL)
        return STAT_ALLOC_FAILURE;

    status = stack_new(&(temp->instance));
    if (status != STAT_SUCCESS) {
        free(temp);
        return status;
    }

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&(temp->lock), &attr);
    pthread_mutexattr_destroy(&attr);
    *stack = temp;

    return STAT_SUCCESS;
}

void ts_stack_lock(ConcurrentStack *stack) {
    LOCK(stack);
}

void ts_stack_unlock(ConcurrentStack *stack) {
    UNLOCK(stack);
}

Status ts_stack_push(ConcurrentStack *stack, void *item) {

    LOCK(stack);
    Status status = stack_push(stack->instance, item);
    UNLOCK(stack);

    return status;
}

Status ts_stack_peek(ConcurrentStack *stack, void **top) {

    LOCK(stack);
    Status status = stack_peek(stack->instance, top);
    UNLOCK(stack);

    return status;
}

Status ts_stack_pop(ConcurrentStack *stack, void **top) {

    LOCK(stack);
    Status status = stack_pop(stack->instance, top);
    UNLOCK(stack);

    return status;
}

void ts_stack_clear(ConcurrentStack *stack, void (*destructor)(void *)) {

    LOCK(stack);
    stack_clear(stack->instance, destructor);
    UNLOCK(stack);
}

long ts_stack_size(ConcurrentStack *stack) {

    LOCK(stack);
    long size = stack_size(stack->instance);
    UNLOCK(stack);

    return size;
}

Boolean ts_stack_isEmpty(ConcurrentStack *stack) {

    LOCK(stack);
    Boolean isEmpty = stack_isEmpty(stack->instance);
    UNLOCK(stack);

    return isEmpty;
}

Status ts_stack_toArray(ConcurrentStack *stack, Array **array) {

    LOCK(stack);
    Status status = stack_toArray(stack->instance, array);
    UNLOCK(stack);

    return status;
}

Status ts_stack_iterator(ConcurrentStack *stack, ConcurrentIterator **iter) {

    Array *array;
    Status status;

    LOCK(stack);
    status = stack_toArray(stack->instance, &array);
    if (status != STAT_SUCCESS) {
        UNLOCK(stack);
        return status;
    }

    status = ts_iterator_new(iter, &(stack->lock), array->items, array->len);
    if (status != STAT_SUCCESS) {
        FREE_ARRAY(array);
        UNLOCK(stack);
    } else {
        free(array);
    }

    return status;
}

void ts_stack_destroy(ConcurrentStack *stack, void (*destructor)(void *)) {

    LOCK(stack);
    stack_destroy(stack->instance, destructor);
    UNLOCK(stack);
    pthread_mutex_destroy(&(stack->lock));
    free(stack);
}
