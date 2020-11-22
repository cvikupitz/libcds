############################################
# Makefile for libcds library
# Author: Cole Vikupitz
############################################
CC=gcc
CFLAGS=-W -Wall -Wextra -g
INCLUDE=-I../include
CPFLAG=-c
COMPILE=$(CC) $(CFLAGS) $(INCLUDE) $(CPFLAG)
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
arraylist.o:
	$(COMPILE) arraylist.c
boundedqueue.o:
	$(COMPILE) boundedqueue.c
boundedstack.o:
	$(COMPILE) boundedstack.c
circularlist.o:
	$(COMPILE) circularlist.c
hashmap.o:
	$(COMPILE) hashmap.c
hashset.o:
	$(COMPILE) hashset.c
heap.o:
	$(COMPILE) heap.c
iterator.o:
	$(COMPILE) iterator.c
linkedlist.o:
	$(COMPILE) linkedlist.c
queue.o:
	$(COMPILE) queue.c
stack.o:
	$(COMPILE) stack.c
treemap.o:
	$(COMPILE) treemap.c
treeset.o:
	$(COMPILE) treeset.c

ts_arraylist.o:
	$(COMPILE) ts_arraylist.c
ts_boundedqueue.o:
	$(COMPILE) ts_boundedqueue.c
ts_boundedstack.o:
	$(COMPILE) ts_boundedstack.c
ts_circularlist.o:
	$(COMPILE) ts_circularlist.c
ts_hashmap.o:
	$(COMPILE) ts_hashmap.c
ts_hashset.o:
	$(COMPILE) ts_hashset.c
ts_heap.o:
	$(COMPILE) ts_heap.c
ts_iterator.o:
	$(COMPILE) ts_iterator.c
ts_linkedlist.o:
	$(COMPILE) ts_linkedlist.c
ts_queue.o:
	$(COMPILE) ts_queue.c
ts_stack.o:
	$(COMPILE) ts_stack.c
ts_treemap.o:
	$(COMPILE) ts_treemap.c
ts_treeset.o:
	$(COMPILE) ts_treeset.c
