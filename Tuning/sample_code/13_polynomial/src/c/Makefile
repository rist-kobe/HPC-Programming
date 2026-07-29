COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
CC=gcc
ARCHFLAGS=-mavx2 # x86-64 CPUs with AVX2 support
CFLAGS=-g -std=gnu99 $(ARCHFLAGS) -O3 -ffast-math -funroll-loops -ftree-vectorize -ftree-loop-vectorize -fopt-info-all=optinfo.all 
##########################################################################
endif

SRC= \
     main.c mykernel.c

OBJS = \
       $(SRC:.c=.o)

.SUFFIXES: .c .o

run.x: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

main.o: mykernel.o mykernel.h main.c

.PHONY: clean
clean:
	rm -f *.o *~ optinfo.all

.PHONY: veryclean
veryclean: clean
	rm -f *.x
