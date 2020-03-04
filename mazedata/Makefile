# Makefile for maze
#	Compiles maze.c into maze.o
#	Willem Klein Wassink, CS50, Winter 2020

# Object files
PROG = maze
OBJS = maze.o 
LIB = mazedata.a

# Used for assembling it as an executable for unit testing
# uncomment out, clean, and remake for unit testing
CFLAGS = -Wall -pedantic -std=c11 -ggdb #-DUNIT_TEST 
CC = gcc
MAKE = make

# Dependencies
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# For unit testing
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

maze.o: ../amazing.h

.PHONY: clean unit

clean:
	rm -f core
	rm -f maze.o
	rm -f mazedata.a
	rm -f maze

unit: $(PROG)
