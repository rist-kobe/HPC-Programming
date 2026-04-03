## GNU 8.5.0
FC=gfortran
FCFLAGS=-g -O2 -fopenmp #-DWO_CRITICAL

EXE=run.x
OBJS=mymod.o main.o

.SUFFIXES: .F90 .o

$(EXE): $(OBJS)
	$(FC) $(FCFLAGS) $(OBJS) -o $@

.F90.o:
	$(FC) $(FCFLAGS) -c $< -o $@

main.o: mymod.o main.F90

mymod.o: mymod.F90

.PHONY:
clean:
	rm -f *.o *.mod $(EXE)
