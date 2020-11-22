# libcds

A library containing several data structures implemented in C, for C. Contains many of the data structures you'd expect to find in a utility package that is provided by default with most programming languages. This one in particular models its API closely after the data structures provided in Java's  ```java.util``` package.

### Available Structures

This library comes with the following implemented data structures, each one coming with a thread-safe and non thread-safe version:

* [Stack](https://docs.oracle.com/javase/7/docs/api/java/util/Stack.html) (Bounded & Unbounded)
* [Queue](https://docs.oracle.com/javase/7/docs/api/java/util/Queue.html) (Bounded & Unbounded)
* [Linked List](https://docs.oracle.com/javase/7/docs/api/java/util/LinkedList.html)
* [Circular List](https://www.tutorialspoint.com/data_structures_algorithms/circular_linked_list_algorithm.htm#:~:text=Advertisements,into%20a%20circular%20linked%20list.)
* [Array List](https://docs.oracle.com/javase/7/docs/api/java/util/ArrayList.html)
* [Heap](https://docs.oracle.com/javase/7/docs/api/java/util/PriorityQueue.html)
* [Hash Map](https://docs.oracle.com/javase/7/docs/api/java/util/HashMap.html)
* [Hash Set](https://docs.oracle.com/javase/7/docs/api/java/util/HashSet.html)
* [Tree Map](https://docs.oracle.com/javase/7/docs/api/java/util/TreeMap.html)
* [Tree Set](https://docs.oracle.com/javase/7/docs/api/java/util/TreeSet.html)

### Examples

##### Basic ADT Usage

Below is an example of basic operations used for one of the ADTs.

```c
#include <stdio.h>
#include "stack.h"

#define LEN 6
static char *items[] = {"red","orange","yellow","green","blue","purple"};

int main(int argc, char **argv) {

    Stack *stack;
    Status stat;

    /* Creates the new stack */
    if ((stat = stack_new(&stack)) != STAT_SUCCESS) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return -1;
    }

    /* Pushes items onto the stack */
    int i;
    for (i = 0; i < LEN; i++)
        (void)stack_push(stack, &(items[i]));

    /* Peeks at the top item */
    char *top;
    (void)stack_peek(stack, (void **)&top);
    /* Expected output: "Top item is "purple" */
    fprintf(stdout, "Top item is: %s\n", top);

    /* Pops an item from the stack */
    (void)stack_pop(stack, (void **)&top);
    /* Expected output: "Popped purple from the stack" */
    fprintf(stdout, "Popped %s from the stack\n", top);

    /* Destroys the stack */
    stack_destroy(stack, NULL);

    return 0;
}
```

##### Creating & Accessing Arrays

Each ADT comes with a ```toArray()``` method that returns an array representation of the structure. An example of how to use this is shown below.

```c
/*
 * Calls toArray(), then prints the array elements.
 */
static void testArray(Stack *stack) {

    Array *array;
    Status status;
    long i;

    /* Creates an Array object from stack's toArray() invocation */
    if ((stat = stack_toArray(stack, &array)) != STAT_SUCCESS) {
        fprintf(stderr, "Failed to allocate memory.\n");
        exit(1);
    }

    /* Prints each of the array's elements */
    /* For stacks, items are ordered from top of the stack to the bottom */
    for (i = 0L; i < array->len; i++)
        fprintf(stdout, "Item %ld: %s\n", i, array->items[i]);

    /* Need to destroy the Array object once finished */
    free(array->items);
    free(array);
}
```

##### Iterators

Each ADT also comes with an ```iterator()``` method that creates and returns an iterator for the structure. The iterator will iterate over the elements in the same order as the array returned from the ```toArray()``` method. An example of this is shown below.

```c
/*
 * Calls iterator(), then iterates over the elements.
 */
static void testIterate(Stack *stack) {

    Iterator *iter;
    Status status;

    /* Creates the Iterator from the iterator() call */
    if ((stat = stack_iterator(stack, &iter)) != STAT_SUCCESS) {
        fprintf(stderr, "Failed to allocate memory.\n");
        exit(1);
    }

    /* Iterates over the elements, printing each one */
    char *item;
    while (iterator_hasNext(iter) == TRUE) {
        (void)iterator_next(iter, (void **)&item);
        fprintf(stdout, "%s\n", item);
    }

    /* Need to destroy the Iterator object once finished */
    iterator_destroy(iter);
}
```

### License

This project is licensed under the terms of the MIT license.

