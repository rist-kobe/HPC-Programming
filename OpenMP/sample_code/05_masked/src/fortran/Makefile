## GNU 14.1.0 (miniforge3 for x86_64)
FC=gfortran
DFLAGS := $(DFLAGS)
#DFLAGS := -DNOT_USE_MASKED # for GNU 8.5.0
FCFLAGS=-g -fopenmp -O0 $(DFLAGS) 

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
