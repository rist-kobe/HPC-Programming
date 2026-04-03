## GNU 8.5.0, 9.3.1
FC=gfortran

## Please choose one of the following flag settings
## FLAGS for no timer
#FFLAGS=-Wall -g -cpp -O0
## FLAGS for use of timer (cpu time)
FFLAGS=-Wall -g -cpp -D__USE_CPU_TIMER__ -O0

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
