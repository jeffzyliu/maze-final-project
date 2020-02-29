# Makefile for maze
#	Compiles maze.c into maze.o

# Object files
OBJS = maze.o
LIB = mazedata.a

# Dependencies
$(LIB): $(OBJS)
		ar cr $(LIB) $(OBJS)

maze.o: ../amazing.h

.PHONY: clean

clean:
	rm -f core
	rm -f maze.o
	rm -f mazedata.a