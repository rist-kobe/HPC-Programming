COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
## GNU 8.5.0, 9.3.1
CC=gcc
CFLAGS=-g -Wall -O0 -std=gnu99
LIB=
##########################################################################
else ifeq ($(COMPILER),intel)
# Intel DPC/C++ (oneAPI 2022.3.1)
CC=icx
CFLAGS=-g -Wall -std=gnu99 -O0 #-DNOT_USE_GNULOC
##########################################################################
else ifeq ($(COMPILER),clang)
## LLVM 13.0.0, Apple clang 15.0.0
CC=clang
CFLAGS=-g -Wall -O0 -std=gnu99
##########################################################################
endif

SRC= \
     main.c mykernel.c

OBJS = \
       $(SRC:.c=.o)

.SUFFIXES: .c .o

run.x: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

main.o: mykernel.o main.c

mykernel.o: mykernel.c

.PHONY: clean
clean:
	rm -f *.o *~

.PHONY: veryclean
veryclean: clean
	rm -f *.x
