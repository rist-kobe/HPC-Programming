COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
## GNU 8.5.0, 9.3.1
CC=g++
CCFLAGS=-g -Wall -O0 -std=c++11
############################
else ifeq ($(COMPILER),intel)
## Intel DPC++/C++ (Intel oneAPI 2022.3.1)
CC=icpx
CCFLAGS=-g -Wall -O0 -std=c++11
############################
else ifeq ($(COMPILER),clang)
## Apple clang 15.0.0
CC=clang++
CCFLAGS=-g -Wall -std=c++11 -O0
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
