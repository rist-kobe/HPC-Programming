COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
## GNU 8.5.0 
FC=gfortran
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
FFLAGS=-g -cpp $(ARCHFLAGS) -O3 -ffast-math -funroll-loops -ftree-loop-vectorize -ftree-slp-vectorize -ftree-vectorize -fopt-info-all=optinfo.all
##########################################################################
else ifeq ($(COMPILER),intel)
## Intel oneAPI 2022.3.1
FC=ifx
#FC=ifort # classic
ARCHFLAGS=-mavx2 # x86, AVX512
#ARCHFLAGS=-xCORE_AVX512 # Intel, AVX512
FFLAGS=-g -cpp $(ARCHFLAGS) -O3 -funroll-loops -qopt-report=2 -qopt-report-phase=vec
##########################################################################
else ifeq ($(COMPILER),fj)
## Fujitsu Fortran (4.10.0)
FC=frtpx
#FC=frt # native compiler
ARCHFLAGS=-KA64FX -KSVE # aarch64, SVE)
FFLAGS=-g -X08 -Cpp -V $(ARCHFLAGS) -Kfast -Knoomitfp -Ksimd=auto -Kswp -Kswp_policy=auto -Koptmsg=2 -Nlst=t 
##########################################################################
endif

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
	rm -f *.o *~ *.mod optinfo.all

.PHONY: veryclean
veryclean: clean
	rm -f *.x
