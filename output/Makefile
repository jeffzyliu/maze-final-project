#Makefile for Output Directory

LIBS= output.a
OBJS = logfile.o ui.o

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb

$(LIBS): $(OBJS)
	ar cr $(LIBS) $(OBJS)

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(LIBS)
	rm -f $(OBJS)
