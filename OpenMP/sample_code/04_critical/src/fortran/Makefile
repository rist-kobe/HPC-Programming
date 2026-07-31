FC = gfortran
FFLAGS = -g -O2 -fopenmp #-DWO_CRITICAL or -DBETTER_IMPL

TARGET = run.x

OBJS = mymod.o main.o

.SUFFIXES: .F90 .o

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(FC) $(FFLAGS) $(OBJS) -o $@

.F90.o:
	$(FC) $(FFLAGS) -c $< -o $@

main.o: mymod.o main.F90

mymod.o: mymod.F90

.PHONY: clean
clean:
	rm -f *.o *.mod *~

.PHONY: veryclean
veryclean: clean
	rm -f $(TARGET)
