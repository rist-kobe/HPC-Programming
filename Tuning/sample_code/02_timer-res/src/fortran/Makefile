COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
## GNU 8.5.0, 9.3.1
FC=gfortran
FFLAGS=-Wall -O0 

#######################################################################
else ifeq ($(COMPILER),intel)
# Intel Fortran (oneAPI 2022.3.1)
FC=ifx
#FC=ifort ## classic
FFLAGS=-warn all -O0 

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
