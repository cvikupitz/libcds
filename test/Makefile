CC=gcc
CFLAGS=-W -Wall -g
IFLAG=-Iinclude
LFLAGS=-L. -lcds
CUNIT=-lcunit
OBJS=iterator_tests.o stack_tests.o
EXECS=iterator_tests stack_tests

all: $(EXECS)

iterator_tests: iterator_tests.o
	$(CC) $(CFLAGS) $(IFLAG) iterator_tests.o -o iterator_tests $(LFLAGS) $(CUNIT)
stack_tests: stack_tests.o
	$(CC) $(CFLAGS) $(IFLAG) stack_tests.o -o stack_tests $(LFLAGS) $(CUNIT)

clean:
	rm -f $(OBJS) $(EXECS)

iterator_tests.o:
	$(CC) $(CFLAGS) $(IFLAG) -c iterator_tests.c
stack_tests.o:
	$(CC) $(CFLAGS) $(IFLAG) -c stack_tests.c
