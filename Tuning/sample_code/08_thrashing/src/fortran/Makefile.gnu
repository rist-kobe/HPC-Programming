## GNU Fortran 8.5.0, 9.3.1
FC=gfortran
FFLAGS=-g -Wall -O0  #-DNOT_USE_GNULOC
##########################################################################
SRC= \
     main.F90

OBJS = \
       $(SRC:.F90=.o)

.SUFFIXES: .F90 .o

run.x: $(OBJS)
	$(FC) $(FFLAGS) $(OBJS) -o $@

.F90.o:
	$(FC) $(FFLAGS) -c $< -o $@

main.o: main.F90

.PHONY: clean
clean:
	rm -f *.o *~ *.mod

.PHONY: veryclean
veryclean: clean
	rm -f *.x
