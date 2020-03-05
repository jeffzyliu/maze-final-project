# Makefile for Output Directory

# Libraries & object files
PROG = ui
LIBS= output.a
OBJS = logfile.o ui.o

# Used for assembling it as an executable for unit testing
# uncomment out, clean, and remake for unit testing
CFLAGS = -Wall -pedantic -std=c11 -ggdb #-DUNIT_TEST 
CC = gcc
MAKE = make

# Dependencies

$(LIBS): $(OBJS)
	ar cr $(LIBS) $(OBJS)

# For unit testing
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean unit

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(LIBS)
	rm -f $(OBJS)
	rm -f $(PROG)

unit: $(PROG)