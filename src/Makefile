CC=gcc
CFLAGS=-W -Wall -g
IFLAG=-Iinclude
OBJS=arraylist.o boundedqueue.o boundedstack.o circularlist.o \
     deque.o hashmap.o hashset.o heap.o iterator.o linkedlist.o \
     queue.o stack.o treeset.o
LIB=libcds.a

libcds.a: $(OBJS)
	ar rcs $(LIB) $(OBJS)

all: $(OBJS)

clean:
	rm -f $(OBJS) $(LIB)

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
treeset.o: 
	$(CC) $(CFLAGS) $(IFLAG) -c treeset.c
