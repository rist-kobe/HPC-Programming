COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
## GNU 8.5.0, 9.3.1
FC=gfortran
FFLAGS=-O1 -Wall
else ifeq ($(COMPILER),intel)
## Intel oneAPI 2022.3.1
FC=ifx
#FC=ifort # classic
FFLAGS=-O1
endif

SRC= \
	main.f90 \
	mytype.f90 \
	timer.f90

OBJ=$(SRC:.f90=.o)

EXE=run.x

.SUFFIXES: .f90 .o

$(EXE): $(OBJ)
	$(FC) $(FFLAGS) $(OBJ) -o $@

.f90.o:
	$(FC) $(FFLAGS) -c $< -o $@

main.o: mytype.o timer.o main.f90

mytype.o: mytype.f90

timer.o: mytype.o timer.f90


.PHONY: clean
clean:
	rm -f *.f90~ *.o *.mod

.PHONY: veryclean
veryclean: clean
	rm -f $(EXE)
