##########################################
# Makefile for CUnit Testing Files
# Author: Cole Vikupitz
##########################################
CC=gcc
CFLAGS=-W -Wall -Wextra -g
INCLUDE=-I../include
COMPILE=$(CC) $(CFLAGS) $(INCLUDE) -c $^
LIBS=-L. -lcds -lcunit -lm
LINK=$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

OBJS=arraylist_tests.o boundedqueue_tests.o boundedstack_tests.o circularlist_tests.o hashmap_tests.o \
     hashset_tests.o heap_tests.o iterator_tests.o linkedlist_tests.o queue_tests.o stack_tests.o \
     treemap_tests.o treeset_tests.o
EXECS=arraylist_tests boundedqueue_tests boundedstack_tests circularlist_tests hashmap_tests hashset_tests \
      heap_tests iterator_tests linkedlist_tests queue_tests stack_tests treemap_tests treeset_tests

# Builds executable for all individual tests
all: $(EXECS)

# Builds executables to separately run ADT tests
arraylist_tests: arraylist_tests.o
	$(LINK)
boundedqueue_tests: boundedqueue_tests.o
	$(LINK)
boundedstack_tests: boundedstack_tests.o
	$(LINK)
circularlist_tests: circularlist_tests.o
	$(LINK)
hashmap_tests: hashmap_tests.o
	$(LINK)
hashset_tests: hashset_tests.o
	$(LINK)
heap_tests: heap_tests.o
	$(LINK)
iterator_tests: iterator_tests.o
	$(LINK)
linkedlist_tests: linkedlist_tests.o
	$(LINK)
queue_tests: queue_tests.o
	$(LINK)
stack_tests: stack_tests.o
	$(LINK)
treemap_tests: treemap_tests.o
	$(LINK)
treeset_tests: treeset_tests.o
	$(LINK)

# Cleans up files
clean:
	rm -f $(OBJS) $(EXECS)

# Builds the .obj files
arraylist_tests.o: arraylist_tests.c
	$(COMPILE)
boundedqueue_tests.o: boundedqueue_tests.c
	$(COMPILE)
boundedstack_tests.o: boundedstack_tests.c
	$(COMPILE)
circularlist_tests.o: circularlist_tests.c
	$(COMPILE)
hashmap_tests.o: hashmap_tests.c
	$(COMPILE)
hashset_tests.o: hashset_tests.c
	$(COMPILE)
heap_tests.o: heap_tests.c
	$(COMPILE)
iterator_tests.o: iterator_tests.c
	$(COMPILE)
linkedlist_tests.o: linkedlist_tests.c
	$(COMPILE)
queue_tests.o: queue_tests.c
	$(COMPILE)
stack_tests.o: stack_tests.c
	$(COMPILE)
treemap_tests.o: treemap_tests.c
	$(COMPILE)
treeset_tests.o: treeset_tests.c
	$(COMPILE)
