## GNU 8.5.0 
FC=gfortran
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
FFLAGS=-g -Wall $(ARCHFLAGS) -O3 -fopenmp -D__USE_F08__
FVEC=-ftree-loop-vectorize -ftree-slp-vectorize -ftree-vectorize -fopt-info-all-vec
FNVEC=-fno-tree-loop-vectorize -fno-tree-slp-vectorize -fno-tree-vectorize
LIB=
INC=
#
CC=gcc
CFLAGS=-g -O0 -std=gnu11

.SUFFIXES: .o .F90 .c

all: run_v.x run_nv.x

run_v.x: timer.o mytype.o mykernel_v.o dummy.o main.o
	$(FC) $(FFLAGS) $(INC) timer.o mytype.o mykernel_v.o dummy.o main.o $(LIB) -o $@	

run_nv.x: timer.o mytype.o mykernel_nv.o dummy.o main.o
	$(FC) $(FFLAGS) $(INC) timer.o mytype.o mykernel_nv.o dummy.o main.o $(LIB) -o $@	

dummy.o: mytype.o dummy.F90

main.o: mytype.o main.F90

mykernel_v.o: mytype.o mykernel.F90
	$(FC) $(FFLAGS) $(INC) $(FVEC) -c mykernel.F90 -o mykernel_v.o 

mykernel_v.s: mytype.o mykernel.F90
	$(FC) $(FFLAGS) $(INC) $(FVEC) -S -c mykernel.F90 -o mykernel_v.s 

mykernel_nv.o: mytype.o mykernel.F90
	$(FC) $(FFLAGS) $(INC) $(FNVEC) -c mykernel.F90 -o mykernel_nv.o 

mykernel_nv.s: mytype.o mykernel.F90
	$(FC) $(FFLAGS) $(INC) $(FNVEC) -S -c mykernel.F90 -o mykernel_nv.s 

mytype.o: mytype.F90

.F90.o:
	$(FC) $(FFLAGS) -c $< -o $@

timer.o:
	$(CC) $(CFLAGS) -c timer.c -o $@

.PHONY: clean
clean: 
	rm -f *~ *.o *.mod

.PHONY: veryclean
veryclean: clean
	rm -f *.x
