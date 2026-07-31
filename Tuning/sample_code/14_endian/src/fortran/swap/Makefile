## GNU 8.5.0
FC=gfortran
FFLAGS=-O0 -g -Wall -fconvert=swap
## Intel Fortran (Intel oneAPI 2022.3.1) in little-endian machine
#FC=ifx #ifort
#FFLAGS=-O0 -g -warn -convert big_endian 
###############################################
SRC= \
	main.f90

OBJ=$(SRC:.f90=.o)

.SUFFIXES: .f90 .o

run.x: $(OBJ)
	$(FC) $(FFLAGS) $(OBJ) -o $@

.f90.o:
	$(FC) $(FFLAGS) -c $< -o $@

main.o: main.f90

.PHONY: clean
clean:
	rm -f *.f90~ *.o *.mod

.PHONY: veryclean
veryclean: clean
	rm -f run.x

