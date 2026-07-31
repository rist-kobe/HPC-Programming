COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
## GNU 8.5.0, 9.3.1
CC=gcc

## Please choose one of the following flag settings
## No timer
#CFLAGS=-Wall -g -std=c99 -O0
## Use of timer (cpu time)
CFLAGS=-Wall -g -std=gnu99 -D__USE_CPU_TIMER__ -O0

LIB=-lm
#LIB=-lm -lrt # if linking is failed
#######################################################################
else ifeq ($(COMPILER),intel)
# Intel DPC++/C++ (oneAPI 2022.3.1)
CC=icx

## Please choose one of the following flag settings
## No timer
#CFLAGS=-g -Wall -std=gnu99 -O0 
## Use of timer (cpu time)
CFLAGS=-g -Wall -std=gnu11 -D__USE_CPU_TIMER__ -O0

LIB=-lm
#LIB=-lm -lrt # if linking is failed
#######################################################################
else ifeq ($(COMPILER),clang)
## LLVM 13.0.0, Apple clang 15.0.0
CC=clang

## Please choose one of the following flag settings
## No timer
#CFLAGS=-Wall -g -std=c99 -O0
## Use of timer (cpu time)
CFLAGS=-Wall -g -std=gnu99 -D__USE_CPU_TIMER__ -O0

LIB=-lm
#LIB=-lm -lrt # if linking is failed
#######################################################################
endif

SRC = \
	timer.c \
	main.c

OBJS = $(SRC:.c=.o)

.SUFFIXES: .o .c

run.x: main.o timer.o 
	$(CC) $(CFLAGS) $(OBJS) $(LIB) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c

timer.o: timer.c timer.h

.PHONY: clean
clean:
	rm -f *.o *~

.PHONY: veryclean
veryclean: clean
	rm -f run.x
