## GNU 8.5.0, 9.3.1
FC=gfortran

## Please choose one of the following flag settings
## Use of timer for CPU time
FFLAGS=-g -Wall -cpp -O0 -DUSE_CPU_TIMER
## Use of timer (elapsed time)
#FFLAGS=-g -Wall -cpp -O0 -DUSE_ELP_TIMER

CC=gcc
CFLAGS=-g -Wall -O0 -std=gnu11 

#######################################################################
SRC = \
	main.f90

CSRC= \
	timer.c

OBJS = $(SRC:.f90=.o)
COBJS= $(CSRC:.c=.o)


.SUFFIXES: .f90 .c .o

run.x: $(OBJS) $(COBJS) 
	$(FC) $(FFLAGS) $(OBJS) $(COBJS) -o $@

.f90.o:
	$(FC) $(FFLAGS) -c $< -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.f90

timer.o: timer.h timer.c

.PHONY: clean
clean:
	rm -f *.o *.mod *~

.PHONY: veryclean
veryclean: clean
	rm -f run.x
