############################################
# Makefile for libcds library
# Author: Cole Vikupitz
############################################
CC=gcc
CFLAGS=-W -Wall -Wextra -g
INCLUDE=-I../include
COMPILE=$(CC) $(CFLAGS) $(INCLUDE) -c $^
OBJS=arraylist.o boundedstack.o boundedqueue.o circularlist.o hashmap.o hashset.o heap.o iterator.o \
     linkedlist.o queue.o stack.o treemap.o treeset.o \
     ts_arraylist.o ts_boundedqueue.o ts_boundedstack.o ts_circularlist.o ts_hashmap.o ts_hashset.o \
     ts_heap.o ts_iterator.o ts_linkedlist.o ts_queue.o ts_stack.o ts_treemap.o ts_treeset.o
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
arraylist.o: arraylist.c
	$(COMPILE)
boundedqueue.o: boundedqueue.c
	$(COMPILE)
boundedstack.o: boundedstack.c
	$(COMPILE)
circularlist.o: circularlist.c
	$(COMPILE)
hashmap.o: hashmap.c
	$(COMPILE)
hashset.o: hashset.c
	$(COMPILE)
heap.o: heap.c
	$(COMPILE)
iterator.o: iterator.c
	$(COMPILE)
linkedlist.o: linkedlist.c
	$(COMPILE)
queue.o: queue.c
	$(COMPILE)
stack.o: stack.c
	$(COMPILE)
treemap.o: treemap.c
	$(COMPILE)
treeset.o: treeset.c
	$(COMPILE)

ts_arraylist.o: ts_arraylist.c
	$(COMPILE)
ts_boundedqueue.o: ts_boundedqueue.c
	$(COMPILE)
ts_boundedstack.o: ts_boundedstack.c
	$(COMPILE)
ts_circularlist.o: ts_circularlist.c
	$(COMPILE)
ts_hashmap.o: ts_hashmap.c
	$(COMPILE)
ts_hashset.o: ts_hashset.c
	$(COMPILE)
ts_heap.o: ts_heap.c
	$(COMPILE)
ts_iterator.o: ts_iterator.c
	$(COMPILE)
ts_linkedlist.o: ts_linkedlist.c
	$(COMPILE)
ts_queue.o: ts_queue.c
	$(COMPILE)
ts_stack.o: ts_stack.c
	$(COMPILE)
ts_treemap.o: ts_treemap.c
	$(COMPILE)
ts_treeset.o: ts_treeset.c
	$(COMPILE)

