FC = gfortran
FFLAGS = -g -Wall -O3 -fopenmp
LIB =
INC =

TARGET = run.x

OBJS = mytype.o mykernel.o main.o

.SUFFIXES: .o .F90

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(FC) $(FFLAGS) $(INC) $(OBJS) $(LIB) -o $@

main.o: mytype.o main.F90

mykernel.o: mytype.o mykernel.F90

mytype.o: mytype.F90

.F90.o:
	$(FC) $(FFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *~ *.o *.mod

.PHONY: veryclean
veryclean: clean
	rm -f $(TARGET)
