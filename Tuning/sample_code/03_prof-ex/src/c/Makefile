#
# Preprocessor option
# __DIFFUSE__: Boundary condition (not periodic boundary condition)
#

#
# GNU compiler: gcc
#
CC=gcc

## Select build mode: make [MODE=std|gprof]  (default: gprof)
MODE ?= gprof

BASE_CFLAGS=-g -Wall -std=gnu11 -O2 -D__DIFFUSE__

ifeq ($(MODE),std)
  ## standard
  CFLAGS=$(BASE_CFLAGS) -fopt-info-all
else ifeq ($(MODE),gprof)
  ## for using gprof
  CFLAGS=-pg $(BASE_CFLAGS)
else
  $(error Unknown MODE '$(MODE)'. Use MODE=std or MODE=gprof)
endif

LIB=-lm

SETSRC = \
	 message.c \
	 setup.c   \
	 trim.c    \

RNDSRC  = \
	  mt19937ar.c    \
	  rndvar.c

CALSRC = \
	 calc_mass.c     \
	 calc_iniposit.c \
	 calc_inivel.c   \
	 calc_force.c    \
	 calc_positr.c

MAIN   = main.c

OBJS = \
      $(SETSRC:.c=.o)    \
      $(RNDSRC:.c=.o)    \
      $(CALSRC:.c=.o)    \
      $(MAIN:.c=.o)

.SUFFIXES: .c .o

.PHONY: rnd setup calc main

diffuse.x: rnd setup calc main
	$(CC) $(CFLAGS) $(OBJS) $(LIB) -o $@ 

# mt19937ar.c and mt19937ar.h are NOT included in this sample.
# See README.md for instructions on how to obtain them.
mt19937ar.c mt19937ar.h:
	$(error $@ is missing. Download the Mersenne-Twister sources (mt19937ar.c and mt19937ar.h) and copy them into this directory. See README.md for details)

rnd: mt19937ar.c mt19937ar.h
	$(CC) $(CFLAGS) -c $(RNDSRC)

setup: 
	$(CC) $(CFLAGS) -c $(SETSRC)

calc: 
	$(CC) $(CFLAGS) -c $(CALSRC)

main:
	$(CC) $(CFLAGS) -c $(MAIN)

.PHONY: clean
clean:
	rm -f *.o *~ gmon.out

.PHONY: veryclean
veryclean: clean
	rm -f *.x
