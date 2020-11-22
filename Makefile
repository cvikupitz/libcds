############################################
# Makefile for libcds library
# Author: Cole Vikupitz
############################################
SRC=./src
TEST=./test

CC=gcc
CFLAGS=-W -Wall -Wextra -g
INCLUDE=-Iinclude
LIBS=-L. -lcds -lcunit -lm
COMPILE=$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $^
LINK=$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
LIB=libcds.a

LIB_OBJS=$(SRC)/arraylist.o $(SRC)/boundedstack.o $(SRC)/boundedqueue.o $(SRC)/circularlist.o \
     $(SRC)/hashmap.o $(SRC)/hashset.o $(SRC)/heap.o $(SRC)/iterator.o $(SRC)/linkedlist.o \
     $(SRC)/queue.o $(SRC)/stack.o $(SRC)/treemap.o $(SRC)/treeset.o \
     $(SRC)/ts_arraylist.o $(SRC)/ts_boundedqueue.o $(SRC)/ts_boundedstack.o $(SRC)/ts_circularlist.o \
     $(SRC)/ts_hashmap.o $(SRC)/ts_hashset.o $(SRC)/ts_heap.o $(SRC)/ts_iterator.o $(SRC)/ts_linkedlist.o \
     $(SRC)/ts_queue.o $(SRC)/ts_stack.o $(SRC)/ts_treemap.o $(SRC)/ts_treeset.o
TEST_OBJS=$(TEST)/arraylist_tests.o $(TEST)/boundedqueue_tests.o $(TEST)/boundedstack.o $(TEST)/circularlist_tests.o \
          $(TEST)/hashmap_tests.o $(TEST)/hashset_tests.o $(TEST)/heap_tests.o $(TEST)/iterator_tests.o \
          $(TEST)/linkedlist_tests.o $(TEST)/queue_tests.o $(TEST)/stack_tests.o $(TEST)/treemap_tests.o \
          $(TEST)/treeset_tests.o
EXECS=$(TEST)/arraylist_tests $(TEST)/boundedqueue_tests $(TEST)/boundedstack_tests $(TEST)/circularlist_tests \
      $(TEST)/hashmap_tests $(TEST)/hashset_tests $(TEST)/heap_tests $(TEST)/iterator_tests $(TEST)/linkedlist_tests \
      $(TEST)/queue_tests $(TEST)/stack_tests $(TEST)/treemap_tests $(TEST)/treeset_tests

$(LIB): $(LIB_OBJS)
	ar -rcs $@ $^
	ranlib $@

tests: $(LIB) $(EXECS)
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

.PHONY: clean
clean:
	rm -f $(LIB_OBJS) $(LIB) $(TEST_OBJS) $(EXECS)

$(SRC)/%.o: $(SRC)/%.c
	$(COMPILE)
$(TEST)/%.o: $(TEST)/%.c
	$(COMPILE)

