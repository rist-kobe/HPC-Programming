## GNU  (8.5.0)
FC=gfortran
FFLAGS=-g -Wall -O0 -fconvert=native
# Intel Fortran (Intel oneAPI 2022.3.1)
#FC=ifx #ifort
#FFLAGS=-g -warn -O0 -convert native

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

