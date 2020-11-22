############################################
# Makefile for libcds library
# Author: Cole Vikupitz
############################################

##### Directories for header, source, and test files
INCLUDE=./include
SRC=./src
TEST=./test

##### Macros used for compilation & linking stages
CC=gcc
CFLAGS=-W -Wall -Wextra -g
IFLAGS=-I$(INCLUDE)
LFLAGS=-L. -lcds -lcunit -lm
COMPILE=$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $^
LINK=$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

##### Name of the library to generate
LIB=libcds.a

##### List of .obj files to archive into library
LIB_OBJS=$(SRC)/arraylist.o $(SRC)/boundedstack.o $(SRC)/boundedqueue.o $(SRC)/circularlist.o \
     $(SRC)/hashmap.o $(SRC)/hashset.o $(SRC)/heap.o $(SRC)/iterator.o $(SRC)/linkedlist.o \
     $(SRC)/queue.o $(SRC)/stack.o $(SRC)/treemap.o $(SRC)/treeset.o \
     $(SRC)/ts_arraylist.o $(SRC)/ts_boundedqueue.o $(SRC)/ts_boundedstack.o $(SRC)/ts_circularlist.o \
     $(SRC)/ts_hashmap.o $(SRC)/ts_hashset.o $(SRC)/ts_heap.o $(SRC)/ts_iterator.o $(SRC)/ts_linkedlist.o \
     $(SRC)/ts_queue.o $(SRC)/ts_stack.o $(SRC)/ts_treemap.o $(SRC)/ts_treeset.o

##### Target to build the complete ADT library
$(LIB): $(LIB_OBJS)
	ar -rcs $@ $^
	ranlib $@

##### Single target used for building individual src/.obj files
$(SRC)/%.o: $(SRC)/%.c
	$(COMPILE)

##### List of .obj files for the test executables
TEST_OBJS=$(TEST)/arraylist_tests.o $(TEST)/boundedqueue_tests.o $(TEST)/boundedstack.o $(TEST)/circularlist_tests.o \
          $(TEST)/hashmap_tests.o $(TEST)/hashset_tests.o $(TEST)/heap_tests.o $(TEST)/iterator_tests.o \
          $(TEST)/linkedlist_tests.o $(TEST)/queue_tests.o $(TEST)/stack_tests.o $(TEST)/treemap_tests.o \
          $(TEST)/treeset_tests.o

##### List of testing executables to build
EXECS=$(TEST)/arraylist_tests $(TEST)/boundedqueue_tests $(TEST)/boundedstack_tests $(TEST)/circularlist_tests \
      $(TEST)/hashmap_tests $(TEST)/hashset_tests $(TEST)/heap_tests $(TEST)/iterator_tests $(TEST)/linkedlist_tests \
      $(TEST)/queue_tests $(TEST)/stack_tests $(TEST)/treemap_tests $(TEST)/treeset_tests

##### Creates all of the listed test executables
tests: $(LIB) $(EXECS)

##### Targets for creating individual testing executables
$(TEST)/arraylist_tests: $(TEST)/arraylist_tests.o
	$(LINK)
$(TEST)/boundedqueue_tests: $(TEST)/boundedqueue_tests.o
	$(LINK)
$(TEST)/boundedstack_tests: $(TEST)/boundedstack_tests.o
	$(LINK)
$(TEST)/circularlist_tests: $(TEST)/circularlist_tests.o
	$(LINK)
$(TEST)/hashmap_tests: $(TEST)/hashmap_tests.o
	$(LINK)
$(TEST)/hashset_tests: $(TEST)/hashset_tests.o
	$(LINK)
$(TEST)/heap_tests: $(TEST)/heap_tests.o
	$(LINK)
$(TEST)/iterator_tests: $(TEST)/iterator_tests.o
	$(LINK)
$(TEST)/linkedlist_tests: $(TEST)/linkedlist_tests.o
	$(LINK)
$(TEST)/queue_tests: $(TEST)/queue_tests.o
	$(LINK)
$(TEST)/stack_tests: $(TEST)/stack_tests.o
	$(LINK)
$(TEST)/treemap_tests: $(TEST)/treemap_tests.o
	$(LINK)
$(TEST)/treeset_tests: $(TEST)/treeset_tests.o
	$(LINK)

##### Single target used for building individual test/.obj files
$(TEST)/%.o: $(TEST)/%.c
	$(COMPILE)

##### Cleans up project files
clean:
	rm -f $(LIB_OBJS) $(LIB) $(TEST_OBJS) $(EXECS)

##### Collection of phony Makefile targets
.PHONY: clean tests help

