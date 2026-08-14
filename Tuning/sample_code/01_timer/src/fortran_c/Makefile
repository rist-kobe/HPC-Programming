FC=gfortran

## Select timer mode: make MODE=elp | cpu  (default: elp)
MODE ?= elp

BASE_FFLAGS=-g -Wall -cpp -O0

ifeq ($(MODE),elp)
  ## Use of timer for Elapsed time
  FFLAGS=$(BASE_FFLAGS) -DUSE_ELP_TIMER
else ifeq ($(MODE),cpu)
  ## Use of timer for CPU time
  FFLAGS=$(BASE_FFLAGS) -DUSE_CPU_TIMER
else
  $(error Unknown MODE '$(MODE)'. Use MODE=elp or MODE=cpu)
endif

CC=gcc
CFLAGS=-g -Wall -O0 -std=gnu11 


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
	rm -f *.o *.mod *~ *genmod.f90

.PHONY: veryclean
veryclean: clean
	rm -f run.x
