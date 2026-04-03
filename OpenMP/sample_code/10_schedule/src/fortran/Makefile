FC=gfortran
FCFLAGS=-g -O2 -fopenmp 

.SUFFIXES: .F90 .o

run.x: main.o
	$(FC) $(FCFLAGS) main.o -o run.x

.F90.o:
	$(FC) $(FCFLAGS) -c $< -o $@ 

.PHONY:
clean:
	rm -f *.o *.mod *.x
