# Makefile for algorithms Directory
# author: Jeff Liu, Willem Klein Vassink, Celina Tala

M = ../mazedata

PROG = move
LIBS = algorithms.a
OBJS = move.o
LLIBS = $M/mazedata.a

# Used for assembling it as an executable for unit testing
# uncomment define to test
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$M # -DALG_TEST 
CC = gcc

$(LIBS): $(OBJS)
	ar cr $(LIBS) $(OBJS)

# For unit testing
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean test

move.o: ../amazing.h ../mazedata/maze.h

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(LIBS)
	rm -f $(OBJS)
	rm -f $(PROG)

test: $(PROG)