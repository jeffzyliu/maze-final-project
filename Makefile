PROG = AMStartup
OBJS = AMStartup.o


#TESTING = -DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$C -I$L
CC = gcc
MAKE = Makefile

$(PROG): $(OBJS)
	 $(CC) $(CFLAGS) $^ $(LIBS) -o $@

AMStartup.o = amazing.h 

.PHONY: test clean


clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f AMStartup.o