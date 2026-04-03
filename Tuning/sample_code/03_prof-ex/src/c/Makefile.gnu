#
# Preprocessor option
# __DIFFUSE__: Boundary condition (not periodic boundary condition)
#

#
# GNU compiler: gcc
#
CC=gcc
## standard 
#CFLAGS=-g -Wall -std=gnu11 -O2 -fopt-info-all -D__DIFFUSE__ 
## for using gprof
CFLAGS=-Wall -std=gnu11 -pg -O2 -D__DIFFUSE__

LIB=-lm
#####################################################

.SUFFIXES: .c .o

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

diffuse.x: rnd setup calc main
	$(CC) $(CFLAGS) $(OBJS) $(LIB) -o $@ 

rnd:
	$(CC) $(CFLAGS) -c $(RNDSRC)

setup: 
	$(CC) $(CFLAGS) -c $(SETSRC)

calc: 
	$(CC) $(CFLAGS) -c $(CALSRC)

main:
	$(CC) $(CFLAGS) -c $(MAIN)

.PHONY: clean
clean:
	rm -f *.o *.c~

.PHONY: veryclean
veryclean: clean
	rm -f *.x 
