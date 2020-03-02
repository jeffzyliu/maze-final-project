#Makefile for Avatar Directory

LIBS= avatar.a
OBJS = messages.o avatar.o 

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb

$(LIBS): $(OBJS)
	ar cr $(LIBS) $(OBJS)

avatar.o: ../output/logfile.h ../algorithms/move.h ../amazing.h

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(LIBS)
	rm -f $(OBJS)
