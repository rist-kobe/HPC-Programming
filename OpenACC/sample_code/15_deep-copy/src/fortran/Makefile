##  AMD EPYC Zen4 + H100 w/ Nvidia HPC SDK 24.1
FC=nvfortran
FCFLAGS=-g -acc=gpu  -gpu=cc90,cuda12.3,sm_90 -O0 -Minfo=accel -tp=znver4 # H100 
INC=
LIN=

FCFLAGS += $(INC)
LINKOPT = $(FCFLAGS)

SRC = main.F90 mymod.F90
OBJ = $(SRC:.F90=.o)

.SUFFIXES: .F90 .o

EXE=run.x

$(EXE): $(OBJ)
	$(FC) $(LINKOPT) -o $(EXE) $(OBJ) $(LIB)

.F90.o:
	$(FC) $(FCFLAGS) -c -o $@ $<

main.o: main.F90 mymod.o

mymod.o: mymod.F90

.PHONY:
clean:
	rm -f *.o *.mod *~ $(EXE)
