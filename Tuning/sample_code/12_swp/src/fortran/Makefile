COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
FC=gfortran
ARCHFLAGS=-mavx2 # x86-64 CPUs with AVX2 support
FFLAGS=-g -Wall $(ARCHFLAGS) -O1
endif

SRC= \
	main.F90 \
	mykernel.F90 \
	mytype.F90 \
	timer.F90

OBJ=$(SRC:.F90=.o)

EXE=run.x

.SUFFIXES: .f90 .F90 .o

$(EXE): $(OBJ)
	$(FC) $(FFLAGS) $(OBJ) -o $@

.f90.o:
	$(FC) $(FFLAGS) -c $< -o $@

.F90.o:
	$(FC) $(FFLAGS) -c $< -o $@

main.o: mykernel.o mytype.o timer.o main.F90

mykernel.o: mytype.o mykernel.F90

mytype.o: mytype.F90

timer.o: mytype.o timer.F90


.PHONY:
clean:
	rm -f *.f90~ *.F90~ *.o *.mod

.PHONY:
veryclean: clean
	rm -f $(EXE)
