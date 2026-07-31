COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
## GNU 8.5.0, 9.3.1
FC=gfortran

## Please choose one of the following flag settings
## FLAGS for no timer
#FFLAGS=-Wall -g -cpp -O0
## FLAGS for use of timer (cpu time)
FFLAGS=-Wall -g -cpp -D__USE_CPU_TIMER__ -O0

#######################################################################
else ifeq ($(COMPILER),intel)
# Intel Fortran (oneAPI 2022.3.1)
FC=ifx
#FC=ifort ## traditional

## Please choose one of the following flag settings
## FLAGS for no timer
#FFLAGS=-g -warn all -fpp -O0
## FLAGS for use of timer (cpu time)
FFLAGS=-g -warn all -fpp -D__USE_CPU_TIMER__ -O0

#######################################################################
endif

SRC = \
	main.f90

OBJS = $(SRC:.f90=.o)

.SUFFIXES: .o .f90

run.x: main.o 
	$(FC) $(FFLAGS) $(OBJS) -o $@

.f90.o:
	$(FC) $(FFLAGS) -c $< -o $@

main.o: main.f90

.PHONY: clean
clean:
	rm -f *.o *~ *.mod *genmod.f90

.PHONY: veryclean
veryclean: clean
	rm -f run.x
