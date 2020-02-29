#Makefile for Algorithms
LIBS= algorithms.a
OBJS = move.o

$(LIBS): $(OBJS)
	ar cr $(LIBS) $(OBJS)

move.o: ../amazing.h 

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(LIBS)
	rm -f $(OBJS)