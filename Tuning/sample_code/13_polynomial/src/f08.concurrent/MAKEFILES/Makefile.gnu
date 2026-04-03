## GNU 8.5.0 
FC=gfortran
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
FFLAGS=-g -cpp $(ARCHFLAGS) -O3 -ffast-math -funroll-loops -ftree-loop-vectorize -ftree-slp-vectorize -ftree-vectorize -fopt-info-all=optinfo.all
##########################################################################
SRC= \
     main.f90 mykernel.f90 mytype.f90

OBJS = \
       $(SRC:.f90=.o)

.SUFFIXES: .f90 .o

run.x: $(OBJS)
	$(FC) $(FFLAGS) $(OBJS) -o $@

.f90.o:
	$(FC) $(FFLAGS) -c $< -o $@

main.o: mytype.o mykernel.o main.f90

mykernel.o: mytype.o mykernel.f90

mytype.o: mytype.f90

.PHONY: clean
clean:
	rm -f *.o *~ *.mod

.PHONY: veryclean
veryclean: clean
	rm -f *.x

