##########################################
# Makefile for CUnit Testing Files
# Author: Cole Vikupitz
##########################################
CC=gcc
CFLAGS=-W -Wall -g
IFLAG=-Iinclude
LFLAGS=-L. -lcds
CUNIT=-lcunit
OBJS=iterator_tests.o queue_tests.o stack_tests.o
EXECS=iterator_tests queue_tests stack_tests

# Builds executable for all tests
all: $(EXECS)

# Builds executables to separately run ADT tests
iterator_tests: iterator_tests.o
	$(CC) $(CFLAGS) $(IFLAG) iterator_tests.o -o iterator_tests $(LFLAGS) $(CUNIT)
queue_tests: queue_tests.o
	$(CC) $(CFLAGS) $(IFLAG) queue_tests.o -o queue_tests $(LFLAGS) $(CUNIT)
stack_tests: stack_tests.o
	$(CC) $(CFLAGS) $(IFLAG) stack_tests.o -o stack_tests $(LFLAGS) $(CUNIT)

# Cleans up files
clean:
	rm -f $(OBJS) $(EXECS)

# Builds the .obj files
iterator_tests.o:
	$(CC) $(CFLAGS) $(IFLAG) -c iterator_tests.c
queue_tests.o:
	$(CC) $(CFLAGS) $(IFLAG) -c queue_tests.c
stack_tests.o:
	$(CC) $(CFLAGS) $(IFLAG) -c stack_tests.c
