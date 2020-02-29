# Makefile for algorithms Directory
# author: Jeff Liu, Willem Klein Vassink, Celina Tala

LIBS = algorithms.a
OBJS = move.o

$(LIBS): $(OBJS)
	ar cr $(LIBS) $(OBJS)

move.o: ../amazing.h ../mazedata/maze.h

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(LIBS)
	rm -f $(OBJS)
