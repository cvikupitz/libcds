/**
 * MIT License
 *
 * Copyright (c) 2020 Cole Vikupitz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _CDS_TS_STACK_H__
#define _CDS_TS_STACK_H__

#include "cds_common.h"
#include "ts_iterator.h"

/**
 * Interface for the thread-safe Stack ADT.
 *
 * The Stack class represents a last-in-first-out (LIFO) stack of objects. This is an unbounded
 * version, meaning that the stack does not impose a capacity on the number of items it may contain.
 * Pushes can be performed as long as memory or the system allows it.
 *
 * Modeled after the Java 7 Stack interface.
 */
typedef struct ts_stack ConcurrentStack;

/**
 * Creates a new stack instance, then stores the new instance into `*stack`.
 *
 * Params:
 *    stack - The pointer address to store the new Stack instance.
 * Returns:
 *    OK - Stack was successfully created.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_stack_new(ConcurrentStack **stack);

/**
 * Locks the stack, providing exclusive access to the calling thread. Caller is responsible for
 * unlocking the stack to allow other threads access.
 *
 * Params:
 *    stack - The stack to operate on.
 * Returns:
 *    None
 */
void ts_stack_lock(ConcurrentStack *stack);

/**
 * Unlocks the stack, releasing the exclusive access from the calling thread.
 *
 * Params:
 *    stack - The stack to operate on.
 * Returns:
 *    None
 */
void ts_stack_unlock(ConcurrentStack *stack);

/**
 * Pushes the specified element onto the stack.
 *
 * Params:
 *    stack - The stack to operate on.
 *    item - The item to be pushed onto the stack.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_stack_push(ConcurrentStack *stack, void *item);

/**
 * Retrieves, but does not remove, the top element from the stack and stores the result into `*top`.
 *
 * Params:
 *    stack - The stack to operate on.
 *    top - The pointer address to store the top element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Stack is currently empty.
 */
Status ts_stack_peek(ConcurrentStack *stack, void **top);

/**
 * Removes the top element from the stack and stores the result into `*top`.
 *
 * Params:
 *    stack - The stack to operate on.
 *    top - The pointer address to store the removed top element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Stack is currently empty.
 */
Status ts_stack_pop(ConcurrentStack *stack, void **top);

/**
 * Removes all elements from the stack. If `destructor` is not NULL, it will be invoked on each
 * element in the stack after being removed.
 *
 * Params:
 *    stack - The stack to operate on.
 *    destructor - Function to operate on each element after removal.
 * Returns:
 *    None
 */
void ts_stack_clear(ConcurrentStack *stack, void (*destructor)(void *));

/**
 * Returns the number of elements in the stack.
 *
 * Params:
 *    stack - The stack to operate on.
 * Returns:
 *    The stack's current size.
 */
long ts_stack_size(ConcurrentStack *stack);

/**
 * Returns TRUE if the stack contains no elements, FALSE if otherwise.
 *
 * Params:
 *    stack - The stack to operate on.
 * Returns:
 *    TRUE if the stack is empty, FALSE if not.
 */
Boolean ts_stack_isEmpty(ConcurrentStack *stack);

/**
 * Allocates and generates an array containing all of the stack's elements in proper sequence (from
 * the top to bottom element), then stores the array into `*array`. Caller is responsible for
 * freeing the array when finished.
 *
 * Params:
 *    stack - The stack to operate on.
 *    array - Address where the new array will be stored.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Stack is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_stack_toArray(ConcurrentStack *stack, Array **array);

/**
 * Creates an Iterator instance to iterate over the stack's elements in proper sequence (from top to
 * bottom element), then stores the iterator into `*iter`. Caller is responsible for destroying the
 * iterator instance when finished.
 *
 * Params:
 *    stack - The stack to operate on.
 *    iter - Address where the new iterator will be stored.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Stack is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_stack_iterator(ConcurrentStack *stack, ConcurrentIterator **iter);

/**
 * Destroys the stack instance by freeing all of its reserved memory. If `destructor` is not NULL,
 * it will be invoked on each element before the stack is destroyed.
 *
 * Params:
 *    stack - The stack to destroy.
 *    destructor - Function to operate on each element prior to stack destruction.
 * Returns:
 *    None
 */
void ts_stack_destroy(ConcurrentStack *stack, void (*destructor)(void *));

#endif  /* _CDS_TS_STACK_H__ */
