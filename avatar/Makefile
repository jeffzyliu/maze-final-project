#Makefile for Avatar Directory

LIBS= avatar.a
OBJS = messages.o avatar.o 

$(LIBS): $(OBJS)
	ar cr $(LIBS) $(OBJS)

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(LIBS)
	rm -f $(OBJS)
