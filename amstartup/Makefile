#Makefile for AMStartup
A = ../avatar
O = ../output
M = ../algorithms
B = ../mazedata
PROG = AMStartup
OBJS = AMStartup.o 
LLIBS = $A/avatar.a $O/output.a $M/algorithms.a $B/mazedata.a


#TESTING = -DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$A -I$O -I$M -I$B -lpthread
CC = gcc
MAKE = Makefile

$(PROG): $(OBJS) $(LLIBS) 
	 $(CC) $(CFLAGS) $(OBJS) $(LLIBS) -o $(PROG)

AMStartup.o: $O/logfile.h $A/avatar.h $A/messages.h ../amazing.h $M/move.o $B/maze.h

.PHONY: test clean


clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f AMStartup.o
	rm -rf ../logOutput/*.log
	rm -f core
	