#Makefile for AMStartup
A = ../avatar
O = ../output
PROG = AMStartup
OBJS = AMStartup.o 
LLIBS = $A/avatar.a $O/output.a


#TESTING = -DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$A -I$O -lpthread
CC = gcc
MAKE = Makefile

$(PROG): $(OBJS) $(LLIBS) 
	 $(CC) $(CFLAGS) $(OBJS) $(LLIBS) -o $(PROG)

AMStartup.o: $A/avatar.h $A/messages.h ../amazing.h $O/logfile.h

.PHONY: test clean


clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f AMStartup.o
	rm -rf logFiles/*.log 
	rm -f core
	