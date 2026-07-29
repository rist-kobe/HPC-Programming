COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
CC=g++
CCFLAGS=-g -Wall -O0 -std=c++11
############################
endif

SRC= \
	main.cc

OBJ=$(SRC:.cc=.o)

.SUFFIXES: .cc .o

run.x: $(OBJ) 
	$(CC) $(CCFLAGS) $(OBJ) -o run.x

.cc.o:
	$(CC) $(CCFLAGS) -c $< -o $@

main.o: main.cc

.PHONY: clean
clean:
	rm -f *.cc~ *.o *.mod

.PHONY: veryclean
veryclean: clean
	rm -f *.x
