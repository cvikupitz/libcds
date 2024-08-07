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
#include "hash_set.h"

/*
 * Struct for an entry in the HashSet ADT.
 */
typedef struct hs_entry {
    struct hs_entry *next;      /* Pointer to the next bucket */
    void *payload;              /* Points to the element */
} HsEntry;

/*
 * The struct for the hashset ADT.
 */
struct hashset {
    long (*hash)(void *, long); /* Hashing function for hashset items */
    int (*cmp)(void *, void *); /* Comparator function for hashset items */
    HsEntry **buckets;          /* Array of buckets containing the elements */
    long size;                  /* The hashset's current size */
    long capacity;              /* The hashset's current capacity */
    long changes;               /* Number of changes since last trigger */
    double load;                /* The hashset's current load */
    double loadFactor;          /* The hashset's load factor */
    double delta;               /* Increment applied for every insertion/deletion */
};

/* Default capacity to use if capacity supplied is invalid */
#define DEFAULT_CAPACITY 16L
/* Default capacity to use if capacity supplied is invalid */
#define DEFAULT_LOADFACTOR 0.75
/* Maximum amount of buckets set can hold at once */
#define MAX_CAPACITY 147483647L

Status hashset_new(HashSet **set, long (*hash)(void *, long),
        int (*comparator)(void *, void *), long capacity, double loadFactor) {

    /* Allocate the struct, check for allocation failure */
    HashSet *temp = (HashSet *)malloc(sizeof(HashSet));
    if (temp == NULL)
        return ALLOC_FAILURE;

    /* Initialize the remaining struct memebers */
    long cap = ( capacity <= 0L ) ? DEFAULT_CAPACITY : capacity;
    if (cap > MAX_CAPACITY)
        cap = MAX_CAPACITY;
    double ldf = ( loadFactor < 0.000001 ) ? DEFAULT_LOADFACTOR : loadFactor;
    size_t bytes = (cap * sizeof(HsEntry *));
    HsEntry **buckets = (HsEntry **)malloc(bytes);

    /* Checks for allocation failures */
    if (buckets == NULL) {
        free(temp);
        return ALLOC_FAILURE;
    }

    /* Initializes the remaining struct members */
    temp->hash = hash;
    temp->cmp = comparator;
    temp->buckets = buckets;
    temp->size = 0L;
    temp->capacity = cap;
    temp->changes = 0L;
    temp->load = 0.0;
    temp->loadFactor = ldf;
    temp->delta = ( 1.0 / (double)cap );
    /* Need to nullify each entry in array */
    long i;
    for (i = 0L; i < cap; i++)
        temp->buckets[i] = NULL;
    *set = temp;

    return OK;
}

/*
 * Fetches the entry with the item from the set and returns it.
 */
static HsEntry *fetchEntry(HashSet *set, void *item, long *index) {

    HsEntry *temp;
    long i = set->hash(item, set->capacity);
    *index = i;

    /* Traverse down to bucket with item */
    for (temp = set->buckets[i]; temp != NULL; temp = temp->next)
        if (!set->cmp(item, temp->payload))
            break;
    return temp;
}

/*
 * Allocates and returns a new hashset entry ADT.
 */
static HsEntry *mallocEntry(void *item) {

    HsEntry *entry = (HsEntry *)malloc(sizeof(HsEntry));
    if (entry != NULL) {
        entry->payload = item;
        entry->next = NULL;
    }

    return entry;
}

/*
 * Resizes the hashset by doubling its capacity once its loadfactor is reached.
 */
static void resizeSet(HashSet *set) {

    HsEntry **buckets, *temp, *next;
    size_t bytes;
    long i, index, cap = (set->capacity * 2);

    /* Allocates the new array of buckets */
    if (cap > MAX_CAPACITY)
        cap = MAX_CAPACITY;
    bytes = (cap * sizeof(HsEntry *));
    buckets = (HsEntry **)malloc(bytes);
    if (buckets == NULL)
        return;
    for (i = 0L; i < cap; i++)
        buckets[i] = NULL;

    /* After resizing, need to rehash all entries into new indecies */
    for (i = 0L; i < set->capacity; i++) {
        temp = set->buckets[i];
        while (temp != NULL) {
            next = temp->next;
            index = set->hash(temp->payload, cap);
            temp->next = buckets[index];
            buckets[index] = temp;
            temp = next;
        }
    }

    /* Updates hashset attributes after resize */
    free(set->buckets);
    set->buckets = buckets;
    set->capacity = cap;
    set->delta = ( 1.0 / (double)cap );
    set->changes = 0L;
    set->load /= 2.0;
}

/* Default trigger: check load factor after this many changes */
#define TRIGGER 100L

/* Macro to check if the map is currently empty */
#define IS_EMPTY(x)  ( ((x)->size == 0L) ? TRUE : FALSE )

Status hashset_add(HashSet *set, void *item) {

    Status status;

    /* Check load factor, resize when needed */
    if (set->changes >= TRIGGER) {
        set->changes = 0L;
        if (set->load >= set->loadFactor && set->capacity < MAX_CAPACITY)
            resizeSet(set);
    }

    long i;
    HsEntry *temp = fetchEntry(set, item, &i);
    if (temp == NULL) {
        /* Allocates and insert new entry */
        HsEntry *entry = mallocEntry(item);
        if (entry != NULL) {
            /* Adds the new element into the set */
            entry->next = set->buckets[i];
            set->buckets[i] = entry;
            set->changes++;
            set->load += set->delta;
            set->size++;
            status = OK;
        } else {
            status = ALLOC_FAILURE;
        }
    } else {
        status = ALREADY_EXISTS;
    }

    return status;
}

Boolean hashset_contains(HashSet *set, void *item) {
    long i;
    return ( fetchEntry(set, item, &i) != NULL ) ? TRUE : FALSE;
}

Status hashset_remove(HashSet *set, void *item, void (*destructor)(void *)) {

    /* Checks if the set is currently empty */
    if (IS_EMPTY(set) == TRUE)
        return STRUCT_EMPTY;

    long i;
    HsEntry *temp = fetchEntry(set, item, &i);
    /* Entry is not present */
    if (temp == NULL)
        return NOT_FOUND;

    /* Fetches the entry from the hashset */
    HsEntry *prev = NULL, *curr = set->buckets[i];
    while (curr != temp) {
        prev = curr;
        curr = curr->next;
    }
    /* Unlinks the bucket item from the set */
    if (prev == NULL)
        set->buckets[i] = temp->next;
    else
        prev->next = temp->next;

    /* Free allocated node and attributes */
    if (destructor != NULL)
        (*destructor)(temp->payload);
    free(temp);
    set->changes++;
    set->load -= set->delta;
    set->size--;

    return OK;
}

/*
 * Clears out the hashset of all its elements. Frees up all reserved memory
 * back to the heap.
 */
static void clearSet(HashSet *set, void (*destructor)(void *)) {

    HsEntry *temp, *next;
    long i;

    for (i = 0L; i < set->capacity; i++) {
        temp = set->buckets[i];
        while (temp != NULL) {
            next = temp->next;
            /* Deallocates the node and attributes */
            if (destructor != NULL)
                (*destructor)(temp->payload);
            free(temp);
            temp = next;
        }
        set->buckets[i] = NULL;
    }
}

void hashset_clear(HashSet *set, void (*destructor)(void *)) {
    clearSet(set, destructor);
    set->size = 0L;
    set->changes = 0L;
    set->load = 0.0;
}

long hashset_size(HashSet *set) {
    return set->size;
}

Boolean hashset_isEmpty(HashSet *set) {
    return IS_EMPTY(set);
}

/*
 * Allocates and creates an array representation of the hashset.
 */
static void **generateArray(HashSet *set) {

    HsEntry *temp = NULL;
    long i, j = 0L;
    size_t bytes;
    void **items = NULL;

    /* Allocate memory for the array */
    bytes = ( set->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return NULL;

    /* Populates the array with hashset entries */
    for (i = 0L; i < set->capacity; i++)
        for (temp = set->buckets[i]; temp != NULL; temp = temp->next)
            items[j++] = temp->payload;

    return items;
}

Status hashset_toArray(HashSet *set, Array **array) {

    /* Do not create the array if currently empty */
    if (IS_EMPTY(set) == TRUE)
        return STRUCT_EMPTY;

    /* Generate the array of hashset items */
    void **items = generateArray(set);
    if (items == NULL)
        return ALLOC_FAILURE;

    /* Allocate memory for the array struct */
    Array *temp = (Array *)malloc(sizeof(Array));
    if (temp == NULL) {
        free(items);
        return ALLOC_FAILURE;
    }

    /* Initializes the remaining struct members */
    temp->items = items;
    temp->len = set->size;
    *array = temp;

    return OK;
}

Status hashset_iterator(HashSet *set, Iterator **iter) {

    Iterator *temp = NULL;

    /* Do not create the array if currently empty */
    if (IS_EMPTY(set) == TRUE)
        return STRUCT_EMPTY;

    /* Generates the array of items for iterator */
    void **items = generateArray(set);
    if (items == NULL)
        return ALLOC_FAILURE;

    /* Creates a new iterator with the items */
    Status status = iterator_new(&temp, items, set->size);
    if (status != OK) {
        free(items);
        return status;
    }
    *iter = temp;

    return OK;
}

void hashset_destroy(HashSet *set, void (*destructor)(void *)) {
    clearSet(set, destructor);
    free(set->buckets);
    free(set);
}
