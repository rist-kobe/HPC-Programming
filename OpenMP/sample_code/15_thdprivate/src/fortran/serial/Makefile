FC=gfortran
FCFLAGS=-g -O0

.SUFFIXES: .F90 .o

run.x: serial.o
	$(FC) $(FCFLAGS) serial.o -o run.x

.F90.o:
	$(FC) $(FCFLAGS) -c $< -o $@ 

.PHONY: clean
clean:
	rm -f *.o *.mod *.x
