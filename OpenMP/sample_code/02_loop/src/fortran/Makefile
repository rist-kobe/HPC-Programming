## GNU 8.5.0, 9.3.1
FC=gfortran
FFLAGS=-g -Wall -O3 -fopenmp
LIB=
INC=
#

.SUFFIXES: .o .F90 .c

all: run.x

run.x: mytype.o mykernel.o main.o
	$(FC) $(FFLAGS) -O0 $(INC) mytype.o mykernel.o main.o $(LIB) -o $@	

main.o: mytype.o main.F90

mykernel.o: mytype.o mykernel.F90

mytype.o: mytype.F90

.F90.o:
	$(FC) $(FFLAGS) -c $< -o $@

.PHONY:
clean:
	rm -f *~ *.o *.mod *.x
