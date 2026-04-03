## GNU 8.5.0 and 9.3.1
FC=gfortran
FCFLAGS=-g -fopenmp -O0
#FCFLAGS=-g -O0 # No OpenMP

.SUFFIXES: .F90 .o

SRC=main.F90

run.x: main.o
	$(FC) $(FCFLAGS) main.o -o $@ 

.F90.o:
	$(FC) $(FCFLAGS) -c $< -o  $@ 

main.o: main.F90

.PHONY:
clean:
	rm -f *.o run.x
