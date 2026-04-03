FC=gfortran
FCFLAGS=-g -O2 -fopenmp

.SUFFIXES: .F90 .o

EXE=run.x

OBJS= main.o mytype.o


$(EXE): main.o mytype.o
	$(FC) $(FCFLAGS) $(OBJS) -o $(EXE)

main.o: mytype.o main.F90

mytype.o: mytype.F90

.F90.o:
	$(FC) $(FCFLAGS) -c $< -o $@

.PHONY:
clean:
	rm -f *.o *.mod $(EXE)
