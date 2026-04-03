## GNU 8.5.0, 9.3.1
FC=gfortran

## Please choose one of the following flag settings
## Use of timer for CPU time
FFLAGS=-g -Wall -cpp -O0 -DUSE_CPU_TIMER
## Use of timer (elapsed time)
#FFLAGS=-g -Wall -cpp -O0 -DUSE_ELP_TIMER
## Use of gprof
#FFLAGS=-pg -g -Wall -cpp -O0

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
	rm -f *.o *.mod *~

.PHONY: veryclean
veryclean: clean
	rm -f run.x
