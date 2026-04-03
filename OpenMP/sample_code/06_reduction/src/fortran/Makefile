FC=gfortran
FCFLAGS=-g -O2 -fopenmp

SRC=main.F90

OBJ=main.o

.SUFFIXES: .F90 .o

run.x: $(OBJ)
	$(FC) $(FCFLAGS) $(OBJ) -o $@

main.o: main.F90

.F90.o:
	$(FC) $(FCFLAGS) -c $< -o $@

.PHONY:
clean:
	rm -f *.o *.mod run.x
