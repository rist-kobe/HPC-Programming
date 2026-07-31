FC = gfortran
FFLAGS = -g -O0

TARGET = run.x

SRC = serial.F90
OBJS = $(SRC:.F90=.o)

.SUFFIXES: .F90 .o

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(FC) $(FFLAGS) $(OBJS) -o $@

.F90.o:
	$(FC) $(FFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *.mod *~

.PHONY: veryclean
veryclean: clean
	rm -f $(TARGET)
