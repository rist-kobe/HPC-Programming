FC=gfortran
FCFLAGS=-g -fopenmp -O0

.SUFFIXES: .F90 .o

SRC=main.F90

run.x: main.o
	$(FC) $(FCFLAGS) main.o -o $@ 

.F90.o:
	$(FC) $(FCFLAGS) -c $< -o  $@ 

main.o: main.F90

.PHONY:
clean:
	rm -f *.o *.mod run.x

