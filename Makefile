CC     = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -g

.PHONY: all demo test clean

# Default: build everything and run tests
all: demo test

# Build the interactive demo executable
demo: demo.c linked_list.c linked_list.h
	$(CC) $(CFLAGS) -o demo demo.c linked_list.c

# Build and immediately run the full test suite
test: tests/test_linked_list.c linked_list.c linked_list.h
	$(CC) $(CFLAGS) -o test_runner tests/test_linked_list.c linked_list.c
	./test_runner

# Remove all build artefacts
clean:
	rm -f demo test_runner
