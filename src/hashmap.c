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
#include "hashmap.h"

/*
 * Struct for the hashmap entry ADT.
 */
struct hm_entry {
    HmEntry *next;      /* Pointer to the next bucket */
    void *key;          /* The entry's associated key */
    void *value;        /* The entry's associated value */
};

/*
 * Struct for the hashmap ADT.
 */
struct hashmap {
    long (*hash)(void *, long);         /* Hashing function for computing bucket placement */
    int (*keyCmp)(void *, void *);      /* Function for comparing keys in the tree */
    void (*keyDxn)(void *);             /* Function for destroying treemap keys */
    HmEntry **buckets;                  /* Array of buckets containing the entries */
    long size;                          /* The hashmap's current size */
    long capacity;                      /* The hashmap's current capacity */
    long changes;                       /* Number of changes since last trigger */
    double load;                        /* The hashmap's current load */
    double loadFactor;                  /* The hashmap's load factor */
    double delta;                       /* Increment applied for every insertion/deletion */
};

/* Default capacity to assign when capacity supplied is invalid */
#define DEFAULT_CAPACITY 16L
/* Default load factor to assign when load factor supplied is invalid */
#define DEFAULT_LOADFACTOR 0.75
/* Maximum amount of buckets map can hold at once */
#define MAX_CAPACITY 147483647L

Status hashmap_new(HashMap **map, long (*hash)(void *, long),
        int (*keyComparator)(void *, void *), long capacity, double loadFactor,
        void (*keyDestructor)(void *)) {

    /* Allocates the struct, checks for allocation failure */
    HashMap *temp = (HashMap *)malloc(sizeof(HashMap));
    if (temp == NULL)
        return ALLOC_FAILURE;

    /* Initializes the remaining struct members */
    long cap = ( capacity <= 0L ) ? DEFAULT_CAPACITY : capacity;
    if (cap > MAX_CAPACITY)
        cap = MAX_CAPACITY;
    double ldf = ( loadFactor < 0.000001 ) ? DEFAULT_LOADFACTOR : loadFactor;
    size_t bytes = (cap * sizeof(HmEntry *));
    HmEntry **buckets = (HmEntry **)malloc(bytes);

    /* Checks for allocation failures */
    if (buckets == NULL) {
        free(temp);
        return ALLOC_FAILURE;
    }

    /* Initializes the remaining struct members */
    temp->hash = hash;
    temp->keyCmp = keyComparator;
    temp->keyDxn = keyDestructor;
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
    *map = temp;

    return OK;
}

/*
 * Fetches the entry from the map and returns it.
 */
static HmEntry *fetchEntry(HashMap *map, void *key, long *index) {

    HmEntry *temp;
    long i = map->hash(key, map->capacity);
    *index = i;

    /* Traverses down to bucket with key */
    for (temp = map->buckets[i]; temp != NULL; temp = temp->next)
        if (map->keyCmp(key, temp->key) == 0)
            break;
    return temp;
}

/*
 * Allocates and returns a new hashmap entry ADT.
 */
static HmEntry *mallocEntry(char *key, void *value) {

    HmEntry *entry = (HmEntry *)malloc(sizeof(HmEntry));
    if (entry != NULL) {
        entry->key = key;
        entry->value = value;
        entry->next = NULL;
    }

    return entry;
}

/*
 * Resizes the hashmap by doubling its capacity once its loadfactor is reached.
 */
static void resizeMap(HashMap *map) {

    HmEntry **buckets, *temp, *next;
    size_t bytes;
    long i, index, cap = ( map->capacity * 2 );

    /* Do not extend if absolute max capacity is reached */
    if (cap > MAX_CAPACITY)
        cap = MAX_CAPACITY;
    /* Allocate the new array of buckets */
    bytes = (cap * sizeof(HmEntry *));
    buckets = (HmEntry **)malloc(bytes);
    if (buckets == NULL)
        return;
    for (i = 0L; i < cap; i++)
        buckets[i] = NULL;

    /* After resize, need to rehash all existing entries into new indecies */
    for (i = 0L; i < map->capacity; i++) {
        temp = map->buckets[i];
        while (temp != NULL) {
            next = temp->next;
            index = map->hash(temp->key, cap);
            temp->next = buckets[index];
            buckets[index] = temp;
            temp = next;
        }
    }

    /* Update the hashmap attributes after resize */
    free(map->buckets);
    map->buckets = buckets;
    map->capacity = cap;
    map->delta = ( 1.0 / (double)cap );
    map->changes = 0L;
    map->load /= 2.0;
}

/* Default trigger: check load factor after this many map changes */
#define TRIGGER 100L

/* Macro to check if the map is currently empty */
#define IS_EMPTY(x)  ( ((x)->size == 0L) ? TRUE : FALSE )

Status hashmap_put(HashMap *map, void *key, void *value, void **previous) {

    Status status;

    /* Checks the load factor, resize when needed */
    if (map->changes >= TRIGGER) {
        map->changes = 0L;
        if (map->load >= map->loadFactor && map->capacity < MAX_CAPACITY)
            resizeMap(map);
    }

    long i;
    HmEntry *temp = fetchEntry(map, key, &i);
    if (temp != NULL) {
        /* Entry already exists, replace the existing entry */
        *previous = temp->value;
        temp->value = value;
        map->changes++;
        status = REPLACED;
    } else {
        /* Otherwise, allocate and insert the new entry */
        HmEntry *entry = mallocEntry(key, value);
        if (entry != NULL) {
            /* Add bucket into targeted index */
            entry->next = map->buckets[i];
            map->buckets[i] = entry;
            map->changes++;
            map->load += map->delta;
            map->size++;
            status = INSERTED;
        } else {
            status = ALLOC_FAILURE;
        }
    }

    return status;
}

Boolean hashmap_containsKey(HashMap *map, void *key) {
    long i;
    return ( fetchEntry(map, key, &i) != NULL ) ? TRUE : FALSE;
}

Status hashmap_get(HashMap *map, void *key, void **value) {

    /* Checks if the map is currently empty */
    if (IS_EMPTY(map) == TRUE)
        return STRUCT_EMPTY;

    /* Fetches the node with the specified key */
    long index;
    HmEntry *temp = fetchEntry(map, key, &index);
    if (temp == NULL)
        return NOT_FOUND;
    /* Retrieves the value, saves into pointer */
    *value = temp->value;

    return OK;
}

Status hashmap_remove(HashMap *map, void *key, void **value) {

    /* Checks if the map is currently empty */
    if (IS_EMPTY(map) == TRUE)
        return STRUCT_EMPTY;

    /* Fetches the node with the specified key */
    long i;
    HmEntry *temp = fetchEntry(map, key, &i);
    if (temp == NULL)
        return NOT_FOUND;

    /* Fetch the node with the key's entry */
    HmEntry *prev = NULL, *curr = map->buckets[i];
    while (curr != temp) {
        prev = curr;
        curr = curr->next;
    }

    /* Relink nodes so that entry is removed */
    if (prev == NULL)
        map->buckets[i] = temp->next;
    else
        prev->next = temp->next;

    /* Free allocated node and attributes */
    *value = temp->value;
    free(temp->key);
    free(temp);
    map->changes++;
    map->load -= map->delta;
    map->size--;

    return OK;
}

/*
 * Clears out the hashmap of all its elements. Frees up all reserved memory
 * back to the heap.
 */
static void clearMap(HashMap *map, void (*valueDestructor)(void *)) {

    HmEntry *temp, *next;
    long i;

    for (i = 0L; i < map->capacity; i++) {
        temp = map->buckets[i];
        while (temp != NULL) {
            next = temp->next;
            /* Deallocate the node and attributes */
            if (map->keyDxn != NULL)
                (*map->keyDxn)(temp->key);
            if (valueDestructor != NULL)
                (*valueDestructor)(temp->value);
            free(temp->key);
            free(temp);
            temp = next;
        }
        map->buckets[i] = NULL;
    }
}

void hashmap_clear(HashMap *map, void (*valueDestructor)(void *)) {
    clearMap(map, valueDestructor);
    map->size = 0L;
    map->changes = 0L;
    map->load = 0.0;
}

long hashmap_size(HashMap *map) {
    return map->size;
}

Boolean hashmap_isEmpty(HashMap *map) {
    return IS_EMPTY(map);
}

Status hashmap_keyArray(HashMap *map, Array **keys) {

    HmEntry *temp = NULL;
    long i, j = 0L;
    size_t bytes;
    void **items = NULL;

    /* Does not create the array if empty */
    if (IS_EMPTY(map) == TRUE)
        return STRUCT_EMPTY;

    /* Allocates memory for the array struct */
    Array *array = (Array *)malloc(sizeof(Array));
    if (array == NULL)
        return ALLOC_FAILURE;

    /* Allocates memory for the array */
    bytes = ( map->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (array == NULL) {
        free(array);
        return ALLOC_FAILURE;
    }

    /* Populates the array with hashmap entries */
    for (i = 0L; i < map->capacity; i++)
        for (temp = map->buckets[i]; temp != NULL; temp = temp->next)
            items[j++] = temp->key;
    array->items = items;
    array->len = map->size;
    *keys = array;

    return OK;
}

/*
 * Allocates and create an array representation of the hashmap.
 */
static HmEntry **generateEntryArray(HashMap *map) {

    HmEntry *temp = NULL, **array = NULL;
    long i, j = 0L;
    size_t bytes;

    /* Allocates memory for the array */
    bytes = ( map->size * sizeof(HmEntry *) );
    array = (HmEntry **)malloc(bytes);
    if (array == NULL)
        return NULL;

    /* Populates the array with hashmap entries */
    for (i = 0L; i < map->capacity; i++)
        for (temp = map->buckets[i]; temp != NULL; temp = temp->next)
            array[j++] = temp;

    return array;
}

Status hashmap_entryArray(HashMap *map, Array **entries) {

    /* Does not create array if currently empty */
    if (IS_EMPTY(map) == TRUE)
        return STRUCT_EMPTY;

    /* Generates the array of hashmap entries */
    HmEntry **items = generateEntryArray(map);
    if (items == NULL)
        return ALLOC_FAILURE;

    /* Allocates memory for the array struct */
    Array *temp = (Array *)malloc(sizeof(Array));
    if (temp == NULL) {
        free(items);
        return ALLOC_FAILURE;
    }

    /* Initializes the remaining struct members */
    temp->items = (void **)items;
    temp->len = map->size;
    *entries = temp;

    return OK;
}

Status hashmap_iterator(HashMap *map, Iterator **iter) {

    Iterator *temp = NULL;

    /* Does not create array if currently empty */
    if (IS_EMPTY(map) == TRUE)
        return STRUCT_EMPTY;

    /* Generates the array of stack items for iterator */
    HmEntry **items = generateEntryArray(map);
    if (items == NULL)
        return ALLOC_FAILURE;

    /* Creates a new iterator with the stack items */
    Status status = iterator_new(&temp, (void **)items, map->size);
    if (status != OK) {
        free(items);
        return status;
    }
    *iter = temp;

    return OK;
}

void hashmap_destroy(HashMap *map, void (*valueDestructor)(void *)) {
    clearMap(map, valueDestructor);
    free(map->buckets);
    free(map);
}

void *hmentry_getKey(HmEntry *entry) {
    return entry->key;
}

void *hmentry_getValue(HmEntry *entry) {
    return entry->value;
}

void hmentry_setValue(HmEntry *entry, void *value) {
    entry->value = value;
}
