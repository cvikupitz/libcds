############################################
# Makefile for libcds library
# Author: Cole Vikupitz
############################################

##### Uncomment this to suppress make from echoing the commands
#.SILENT:
##### Collection of phony Makefile targets
.PHONY: all test dist clean mostlyclean install uninstall help

##### Project metadata
NAME=libcds
VERSION=v1
DIST=$(NAME)-$(VERSION)

##### Root path where the installation takes place
##### Set this to your own user's home folder if needed
INSTALL_PATH?=/usr/local

##### Directories for header, source, and test files
INCLUDE=./include
SRC=./src
TEST=./test

##### Macros used for compilation & linking stages
CC=gcc
CFLAGS=-W -Wall -Wextra -g -fPIC -O2
IFLAGS=-I$(INCLUDE)
LIBS=-lpthread
LFLAGS=-L. -lcds -lcunit $(LIBS)
COMPILE=$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $^
LINK=$(CC) $(CFLAGS) -o $@ $@.o $(LFLAGS)

##### Name of the libraries to generate
STATIC=libcds.a
SHARED=libcds.so

##### List of .obj files to archive into library
LIB_OBJS=$(SRC)/array_list.o $(SRC)/bounded_stack.o $(SRC)/bounded_queue.o $(SRC)/circular_list.o $(SRC)/hash_map.o \
         $(SRC)/hash_set.o $(SRC)/heap.o $(SRC)/iterator.o $(SRC)/linked_list.o $(SRC)/queue.o $(SRC)/stack.o \
         $(SRC)/tree_map.o $(SRC)/tree_set.o \
         $(SRC)/ts_array_list.o $(SRC)/ts_bounded_queue.o $(SRC)/ts_bounded_stack.o $(SRC)/ts_circular_list.o \
         $(SRC)/ts_hash_map.o $(SRC)/ts_hash_set.o $(SRC)/ts_heap.o $(SRC)/ts_iterator.o $(SRC)/ts_linked_list.o \
         $(SRC)/ts_queue.o $(SRC)/ts_stack.o $(SRC)/ts_tree_map.o $(SRC)/ts_tree_set.o

##### Builds all libraries
all: $(STATIC) $(SHARED)

##### Target to build the complete ADT library
$(STATIC): $(LIB_OBJS)
	ar -crs $@ $^
	ranlib $@
$(SHARED): $(LIB_OBJS)
	$(CC) -shared -o $@ $^ $(LIBS)

##### Single target used for building individual src/.obj files
$(SRC)/%.o: $(SRC)/%.c
	$(COMPILE)

##### List of .obj files for the test executables
TEST_OBJS=$(TEST)/array_list_tests.o $(TEST)/bounded_queue_tests.o $(TEST)/bounded_stack_tests.o \
          $(TEST)/circular_list_tests.o $(TEST)/hash_map_tests.o $(TEST)/hash_set_tests.o $(TEST)/heap_tests.o \
          $(TEST)/iterator_tests.o $(TEST)/linked_list_tests.o $(TEST)/queue_tests.o $(TEST)/stack_tests.o \
          $(TEST)/tree_map_tests.o $(TEST)/tree_set_tests.o

##### List of testing executables to build
EXECS=$(TEST)/array_list_tests $(TEST)/bounded_queue_tests $(TEST)/bounded_stack_tests $(TEST)/circular_list_tests \
      $(TEST)/hash_map_tests $(TEST)/hash_set_tests $(TEST)/heap_tests $(TEST)/iterator_tests $(TEST)/linked_list_tests \
      $(TEST)/queue_tests $(TEST)/stack_tests $(TEST)/tree_map_tests $(TEST)/tree_set_tests

##### Creates all of the listed test executables
test: $(STATIC) $(EXECS)

##### Targets for creating individual testing executables
$(TEST)/array_list_tests: $(STATIC) $(TEST)/array_list_tests.o
	$(LINK)
$(TEST)/bounded_queue_tests: $(STATIC) $(TEST)/bounded_queue_tests.o
	$(LINK)
$(TEST)/bounded_stack_tests: $(STATIC) $(TEST)/bounded_stack_tests.o
	$(LINK)
$(TEST)/circular_list_tests: $(STATIC) $(TEST)/circular_list_tests.o
	$(LINK)
$(TEST)/hash_map_tests: $(STATIC) $(TEST)/hash_map_tests.o
	$(LINK)
$(TEST)/hash_set_tests: $(STATIC) $(TEST)/hash_set_tests.o
	$(LINK)
$(TEST)/heap_tests: $(STATIC) $(TEST)/heap_tests.o
	$(LINK)
$(TEST)/iterator_tests: $(STATIC) $(TEST)/iterator_tests.o
	$(LINK)
$(TEST)/linked_list_tests: $(STATIC) $(TEST)/linked_list_tests.o
	$(LINK)
$(TEST)/queue_tests: $(STATIC) $(TEST)/queue_tests.o
	$(LINK)
$(TEST)/stack_tests: $(STATIC) $(TEST)/stack_tests.o
	$(LINK)
$(TEST)/tree_map_tests: $(STATIC) $(TEST)/tree_map_tests.o
	$(LINK)
$(TEST)/tree_set_tests: $(STATIC) $(TEST)/tree_set_tests.o
	$(LINK)

##### Single target used for building individual test/.obj files
$(TEST)/%.o: $(TEST)/%.c
	$(COMPILE)

##### Creates a distribution tarball of the project
dist:
	mkdir $(DIST)/
	rsync -avz --exclude=$(DIST) * $(DIST)/
	tar -czvf $(DIST).tgz $(DIST)/
	rm -fr $(DIST)/

##### Installs the libraries, headers, and other necessary files
install: $(STATIC) $(SHARED)
	@echo "******** Installation path set to: $(INSTALL_PATH), creating the necessary directories..."
	sudo mkdir -vp $(INSTALL_PATH)/include/$(NAME)
	@echo "******** Installing the libraries $(STATIC) & $(SHARED) into $(INSTALL_PATH)/lib..."
	sudo rsync -avz $(STATIC) $(SHARED) $(INSTALL_PATH)/lib
	@echo "******** Libraries successfully installed, proceeding with header installation at $(INSTALL_PATH)/include/$(NAME)..."
	sudo rsync -avz $(INCLUDE)/*.h $(INSTALL_PATH)/include/$(NAME)
	@echo "******** Sucessfully installed headers, installation completed!"

##### Uninstalls the libraries, headers, and other files
uninstall:
	@echo "******** Installation path set to: $(INSTALL_PATH), preparing to uninstall..."
	@echo "******** Unistalling the libraries from $(INSTALL_PATH)/lib..."
	sudo rm -f $(INSTALL_PATH)/lib/$(STATIC) $(INSTALL_PATH)/lib/$(SHARED)
	@echo "******** Successfully uninstalled library files, proceeding to uninstall headers from $(INSTALL_PATH)/include/$(NAME)..."
	sudo rm -fr $(INSTALL_PATH)/include/$(NAME)
	@echo "******** Successfully uninstalled the headers, uninstallation complete!"

##### Targets for cleaning up project files
clean:
	rm -f $(LIB_OBJS) $(STATIC) $(SHARED) $(DIST).tgz $(TEST_OBJS) $(EXECS)
mostlyclean:
	rm -f $(LIB_OBJS) $(TEST_OBJS) $(EXECS)

##### Target for displaying the usage message
help:
	@echo "Possible targets for this Makefile are the following:"
	@echo "  all (default) : Compiles and builds the static (.a) and shared (.so) libraries."
	@echo "  libcds.a      : Compiles and builds the static library (libcds.a) only."
	@echo "  libcds.so     : Compiles and builds the shared library (libcds.so) only."
	@echo "  test          : Compiles and builds all test executables in the test folder."
	@echo "  dist          : Creates an archive (.tgz) of the complete repository."
	@echo "  clean         : Cleans all project files."
	@echo "  mostlyclean   : Cleans all project files except for the built libraries."
	@echo "  install       : Installs the libraries, headers, and other project files onto your system."
	@echo "  uninstall     : Uninstalls the libraries and all other related files from your system."
	@echo "  help          : Displays this help message."

