COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
CC=g++
ARCHFLAGS=-mavx2 # x86-64 CPUs with AVX2 support
CFLAGS=-g -std=gnu++11 $(ARCHFLAGS) -O3 -ffast-math -funroll-loops -ftree-vectorize -ftree-loop-vectorize -fopt-info-all=optinfo.all 
###################
endif

SRC = \
	main.cc \
	mykernel.cc

OBJ = $(SRC:.cc=.o)

.SUFFIXES: .cc .o

run.x: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

.cc.o:
	$(CC) $(CFLAGS) -c $< -o $@

main.o: mykernel.o main.cc

mykernel.o: mykernel.cc

.PHONY: clean
clean:
	rm -f *.o *~ optinfo.all

.PHONY: veryclean
veryclean: clean
	rm -f *.x
