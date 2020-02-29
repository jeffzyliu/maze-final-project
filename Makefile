# Makefile for CS50 Final Maze Project
#
# Team Willpower, Feb 2020
# Jeff Liu, Celina Tala, Willem Klein Wassink

MAKE = make
.PHONY: all clean

############## default: make all libs and programs ##########
all: 
#	$(MAKE) -C mazedata 
	$(MAKE) -C avatar
#	$(MAKE) -C algorithms
	$(MAKE) -C output
	$(MAKE) -C amstartup

##############  clean  ##########
clean:
	rm -f *~
#	$(MAKE) -C mazedata clean
	$(MAKE) -C avatar clean
#	$(MAKE) -C algorithms clean
	$(MAKE) -C output clean
	$(MAKE) -C amstartup clean
