############################################
# Makefile for libcds library
# Author: Cole Vikupitz
############################################
CC=gcc
CFLAGS=-W -Wall -g
IFLAG=-Iinclude
OBJS=arraylist.o circularlist.o deque.o hashmap.o hashset.o heap.o \
     iterator.o linkedlist.o stack.o
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
stack.o:
	$(CC) $(CFLAGS) $(IFLAG) -c stack.c
