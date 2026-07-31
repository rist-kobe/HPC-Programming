COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
## GNU (8.5.0)
FC=gfortran
FFLAGS=-O0 -g -Wall -cpp 
INCPATH= 
LIBPATH= 
LIB= 
###########################################################################
else ifeq ($(COMPILER),intel)
## Intel Fortran (oneAPI 2022.3.1)
FC=ifx
#FC=ifort
FFLAGS=-O0 -g -warn all -fpp 
#INCPATH= 
#LIBPATH= 
#LIB= 
###########################################################################
endif

SRC= \
     main.f90

OBJS = \
       $(SRC:.f90=.o)

.SUFFIXES: .f90 .o

run.x: main.o
	$(FC) $(FFLAGS) $(INCPATH) $(LIBPATH) $(OBJS) $(LIB) -o $@

.f90.o:
	$(FC) $(FFLAGS) $(INCPATH) -c $< -o $@

main.o : main.f90

.PHONY: clean
clean:
	rm -f *.o *~

.PHONY: veryclean
veryclean: clean
	rm -f *.x
