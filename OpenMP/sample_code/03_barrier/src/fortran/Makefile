FC = gfortran
FFLAGS = -g -fopenmp -O0

TARGET = run.x

SRC = main.F90
OBJS = $(SRC:.F90=.o)

.SUFFIXES: .F90 .o

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(FC) $(FFLAGS) $(OBJS) -o $@

.F90.o:
	$(FC) $(FFLAGS) -c $< -o $@

main.o: main.F90

.PHONY: clean
clean:
	rm -f *.o *.mod *~

.PHONY: veryclean
veryclean: clean
	rm -f $(TARGET)
