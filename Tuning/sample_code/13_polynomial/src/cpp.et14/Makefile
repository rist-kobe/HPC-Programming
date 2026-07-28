COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
## GNU 8.5.0, 9.3.1
CC=g++
ARCHFLAGS=-mavx2 # x86-64 CPUs with AVX2 support
#ARCHFLAGS=-march=armv8-a # Arm-based systems (example)
CFLAGS=-g -std=gnu++14 $(ARCHFLAGS) -O3 -ffast-math -funroll-loops -ftree-vectorize -ftree-loop-vectorize -fopt-info-all=optinfo.all 
###################################################################################
endif

SRC= \
     main.cc \
     mykernel.cc

OBJS = \
       $(SRC:.cc=.o)

.SUFFIXES: .cc .o

run.x: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB) -o $@

.cc.o:
	$(CC) $(CFLAGS) -c $< $(LIB) -o $@

main.o: mykernel.o main.cc vector_et.h

mykernel.o: mykernel.cc vector_et.h

.PHONY: clean
clean:
	rm -f *.o *~

.PHONY: veryclean
veryclean: clean
	rm -f *.x
