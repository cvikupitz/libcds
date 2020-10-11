##########################################
# Makefile for CUnit Testing Files
# Author: Cole Vikupitz
##########################################
CC=gcc
CFLAGS=-W -Wall -g
IFLAG=-Iinclude
LFLAGS=-L. -lcds
CUNIT=-lcunit
OBJS=arraylist_tests.o boundedqueue_tests.o boundedstack_tests.o circularlist_tests.o \
     deque_tests.o hashmap_tests.o hashset_tests.o heap_tests.o iterator_tests.o \
     linkedlist_tests.o queue_tests.o stack_tests.o treeset_tests.o
EXECS=arraylist_tests boundedqueue_tests boundedstack_tests circularlist_tests deque_tests \
      hashmap_tests hashset_tests heap_tests iterator_tests linkedlist_tests queue_tests \
      stack_tests treeset_tests

# Builds executable for all tests
all: $(EXECS)

# Builds executables to separately run ADT tests
arraylist_tests: arraylist_tests.o
	$(CC) $(CFLAGS) $(IFLAG) arraylist_tests.o -o arraylist_tests $(LFLAGS) $(CUNIT)
boundedqueue_tests: boundedqueue_tests.o
	$(CC) $(CFLAGS) $(IFLAG) boundedqueue_tests.o -o boundedqueue_tests $(LFLAGS) $(CUNIT)
boundedstack_tests: boundedstack_tests.o
	$(CC) $(CFLAGS) $(IFLAG) boundedstack_tests.o -o boundedstack_tests $(LFLAGS) $(CUNIT)
circularlist_tests: circularlist_tests.o
	$(CC) $(CFLAGS) $(IFLAG) circularlist_tests.o -o circularlist_tests $(LFLAGS) $(CUNIT)
deque_tests: deque_tests.o
	$(CC) $(CFLAGS) $(IFLAG) deque_tests.o -o deque_tests $(LFLAGS) $(CUNIT)
hashmap_tests: hashmap_tests.o
	$(CC) $(CFLAGS) $(IFLAG) hashmap_tests.o -o hashmap_tests $(LFLAGS) $(CUNIT)
hashset_tests: hashset_tests.o
	$(CC) $(CFLAGS) $(IFLAG) hashset_tests.o -o hashset_tests $(LFLAGS) $(CUNIT)
heap_tests: heap_tests.o
	$(CC) $(CFLAGS) $(IFLAG) heap_tests.o -o heap_tests $(LFLAGS) $(CUNIT)
iterator_tests: iterator_tests.o
	$(CC) $(CFLAGS) $(IFLAG) iterator_tests.o -o iterator_tests $(LFLAGS) $(CUNIT)
linkedlist_tests: linkedlist_tests.o
	$(CC) $(CFLAGS) $(IFLAG) linkedlist_tests.o -o linkedlist_tests $(LFLAGS) $(CUNIT)
queue_tests: queue_tests.o
	$(CC) $(CFLAGS) $(IFLAG) queue_tests.o -o queue_tests $(LFLAGS) $(CUNIT)
stack_tests: stack_tests.o
	$(CC) $(CFLAGS) $(IFLAG) stack_tests.o -o stack_tests $(LFLAGS) $(CUNIT)
treeset_tests: treeset_tests.o
	$(CC) $(CFLAGS) $(IFLAG) treeset_tests.o -o treeset_tests $(LFLAGS) $(CUNIT)

# Cleans up files
clean:
	rm -f $(OBJS) $(EXECS)

# Builds the .obj files
arraylist_tests.o:
	$(CC) $(CFLAGS) $(IFLAG) -c arraylist_tests.c
boundedqueue_tests.o:
	$(CC) $(CFLAGS) $(IFLAG) -c boundedqueue_tests.c
boundedstack_tests.o:
	$(CC) $(CFLAGS) $(IFLAG) -c boundedstack_tests.c
circularlist_tests.o:
	$(CC) $(CFLAGS) $(IFLAG) -c circularlist_tests.c
deque_tests.o:
	$(CC) $(CFLAGS) $(IFLAG) -c deque_tests.c
hashmap_tests.o:
	$(CC) $(CFLAGS) $(IFLAG) -c hashmap_tests.c
hashset_tests.o:
	$(CC) $(CFLAGS) $(IFLAG) -c hashset_tests.c
heap_tests.o:
	$(CC) $(CFLAGS) $(IFLAG) -c heap_tests.c
iterator_tests.o:
	$(CC) $(CFLAGS) $(IFLAG) -c iterator_tests.c
linkedlist_tests.o:
	$(CC) $(CFLAGS) $(IFLAG) -c linkedlist_tests.c
queue_tests.o:
	$(CC) $(CFLAGS) $(IFLAG) -c queue_tests.c
stack_tests.o:
	$(CC) $(CFLAGS) $(IFLAG) -c stack_tests.c
treeset_tests.o:
	$(CC) $(CFLAGS) $(IFLAG) -c treeset_tests.c
