############################################
# Makefile for libcds library
# Author: Cole Vikupitz
############################################
CC=gcc
CFLAGS=-W -Wall -g
IFLAG=-Iinclude
OBJS=arraylist.o boundedstack.o boundedqueue.o circularlist.o deque.o hashmap.o hashset.o \
     heap.o iterator.o linkedlist.o queue.o stack.o treemap.o treeset.o \
     ts_arraylist.o ts_boundedqueue.o ts_boundedstack.o ts_circularlist.o ts_deque.o \
     ts_hashmap.o ts_hashset.o ts_heap.o ts_iterator.o ts_linkedlist.o ts_queue.o ts_stack.o \
     ts_treemap.o ts_treeset.o
LIB=libcds.a

# Compiles and builds the static library
$(LIB): $(OBJS)
	ar -rcs $(LIB) $(OBJS)

# Compiles .obj files for all ADTs
all: $(OBJS)

# Cleans up files
clean:
	rm -f $(OBJS) $(LIB)

# Targets for compiling the .obj files
arraylist.o:
	$(CC) $(CFLAGS) $(IFLAG) -c arraylist.c
boundedqueue.o:
	$(CC) $(CFLAGS) $(IFLAG) -c boundedqueue.c
boundedstack.o:
	$(CC) $(CFLAGS) $(IFLAG) -c boundedstack.c
circularlist.o:
	$(CC) $(CFLAGS) $(IFLAG) -c circularlist.c
deque.o:
	$(CC) $(CFLAGS) $(IFLAG) -c deque.c
hashmap.o:
	$(CC) $(CFLAGS) $(IFLAG) -c hashmap.c
hashset.o:
	$(CC) $(CFLAGS) $(IFLAG) -c hashset.c
heap.o:
	$(CC) $(CFLAGS) $(IFLAG) -c heap.c
iterator.o:
	$(CC) $(CFLAGS) $(IFLAG) -c iterator.c
linkedlist.o:
	$(CC) $(CFLAGS) $(IFLAG) -c linkedlist.c
queue.o:
	$(CC) $(CFLAGS) $(IFLAG) -c queue.c
stack.o:
	$(CC) $(CFLAGS) $(IFLAG) -c stack.c
treemap.o:
	$(CC) $(CFLAGS) $(IFLAG) -c treemap.c
treeset.o:
	$(CC) $(CFLAGS) $(IFLAG) -c treeset.c

ts_arraylist.o:
	$(CC) $(CFLAGS) $(IFLAG) -c ts_arraylist.c
ts_boundedqueue.o:
	$(CC) $(CFLAGS) $(IFLAG) -c ts_boundedqueue.c
ts_boundedstack.o:
	$(CC) $(CFLAGS) $(IFLAG) -c ts_boundedstack.c
ts_circularlist.o:
	$(CC) $(CFLAGS) $(IFLAG) -c ts_circularlist.c
ts_deque.o:
	$(CC) $(CFLAGS) $(IFLAG) -c ts_deque.c
ts_hashmap.o:
	$(CC) $(CFLAGS) $(IFLAG) -c ts_hashmap.c
ts_hashset.o:
	$(CC) $(CFLAGS) $(IFLAG) -c ts_hashset.c
ts_heap.o:
	$(CC) $(CFLAGS) $(IFLAG) -c ts_heap.c
ts_iterator.o:
	$(CC) $(CFLAGS) $(IFLAG) -c ts_iterator.c
ts_linkedlist.o:
	$(CC) $(CFLAGS) $(IFLAG) -c ts_linkedlist.c
ts_queue.o:
	$(CC) $(CFLAGS) $(IFLAG) -c ts_queue.c
ts_stack.o:
	$(CC) $(CFLAGS) $(IFLAG) -c ts_stack.c
ts_treemap.o:
	$(CC) $(CFLAGS) $(IFLAG) -c ts_treemap.c
ts_treeset.o:
	$(CC) $(CFLAGS) $(IFLAG) -c ts_treeset.c
