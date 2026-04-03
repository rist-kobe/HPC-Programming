# Intel Fortran (oneAPI 2022.3.1)
FC=ifx
#FC=ifort ## traditional

## Please choose one of the following flag settings
## FLAGS for no timer
#FFLAGS=-g -warn all -fpp -O0 
## FLAGS for use of timer (cpu time)
FFLAGS=-g -warn all -fpp -D__USE_CPU_TIMER__ -O0

#######################################################################
SRC = \
	main.f90

OBJS = $(SRC:.f90=.o)

.SUFFIXES: .o .f90

run.x: main.o 
	$(FC) $(FFLAGS) $(OBJS) -o $@

.f90.o:
	$(FC) $(FFLAGS) -c $< -o $@

main.o: main.f90

.PHONY: clean
clean:
	rm -f *.o *~

.PHONY: veryclean
veryclean: clean
	rm -f run.x
