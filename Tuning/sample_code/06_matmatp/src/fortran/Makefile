COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
## GNU 8.5.0, 9.3.1
FC=gfortran
ARCHFLAGS=-mavx2 # x86 with AVX2
FOPT=-O2 -fopt-info-all=optinfo.all 
LIB=
INC= 
FFLAGS=-g -Wall $(ARCHFLAGS) $(FOPT) $(INC) 

CC=gcc
CFLAGS=-g -O0 -std=gnu11
else ifeq ($(COMPILER),intel)
## Intel oneAPI 2022.3.1
FC=ifx
#FC=ifort
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
FOPT=-O2 -qopt-report=2 
LIB=
INC= 
FFLAGS=-g -warn $(ARCHFLAGS) $(FOPT) $(INC) 

CC=icx
CFLAGS=-g -O0
else ifeq ($(COMPILER),intel-classic)
## Intel oneAPI 2022.3.1
#FC=ifx
FC=ifort
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
#ARCHFLAGS=-xCORE_AVX512 # xeon (with CORE-AVX512)
FOPT=-O2 -qopt-report=2 
LIB=
INC= 
FFLAGS=-g -warn $(ARCHFLAGS) $(FOPT) $(INC) 

CC=icx
CFLAGS=-g -O0
endif

SRC = \
	main.F90     \
	mykernel.F90 \
	mytype.F90 

OBJ  = $(SRC:.F90=.o)

.SUFFIXES: .o .F90 .c

all: run.x 

run.x: timer.o $(OBJ)
	$(FC) $(FFLAGS) timer.o $(OBJ) $(LIB) -o $@	

main.o: mytype.o mykernel.o main.F90

mykernel.o: mytype.o mykernel.F90

mytype.o: mytype.F90

.F90.o:
	$(FC) $(FFLAGS) -c $< -o $@

timer.o:
	$(CC) $(CFLAGS) -c timer.c -o $@

.PHONY:
clean:
	rm -f *~ *.o *.mod *genmod* *optinfo* *.yaml *optrpt

.PHONY:
veryclean: clean
	rm -f *.x
	rm -rf bin/gnu
	rm -rf bin/intel
	rm -rf bin/intel-classic
