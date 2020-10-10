CC=gcc
CFLAGS=-W -Wall -g -c
INCLUDES=-Iinclude
OBJS=arraylist.o boundedqueue.o boundedstack.o circularlist.o \
     deque.o hashmap.o hashset.o heap.o iterator.o linkedlist.o \
     queue.o stack.o treeset.o
LIBS=libcds.a

all: $(OBJS)

clean:
	rm -f $(OBJS) $(EXEC)

arraylist.o: arraylist.c arraylist.h common.h iterator.h
boundedqueue.o: boundedqueue.c boundedqueue.h common.h iterator.h
boundedstack.o: boundedstack.c boundedstack.h common.h iterator.h
circularlist.o: circularlist.c circularlist.h common.h iterator.h
deque.o: deque.c deque.h common.h iterator.h
hashmap.o: hashmap.c hashmap.h common.h iterator.h
hashset.o: hashset.c hashset.h common.h iterator.h
heap.o: heap.c heap.h common.h iterator.h
iterator.o: iterator.c iterator.h common.h
linkedlist.o: linkedlist.c linkedlist.h common.h iterator.h
queue.o: queue.c queue.h common.h iterator.h
stack.o: stack.c stack.h common.h iterator.h
treeset.o: treeset.c treeset.h common.h iterator.h
