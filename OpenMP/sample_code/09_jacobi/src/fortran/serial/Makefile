COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
FC = gfortran
FFLAGS = -g -O2 -fopenmp
###################################################################
endif

TARGET = run.x

OBJS = main.o mytype.o

.SUFFIXES: .F90 .o

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(FC) $(FFLAGS) $(OBJS) -o $@

main.o: mytype.o main.F90

mytype.o: mytype.F90

.F90.o:
	$(FC) $(FFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *.mod *~

.PHONY: veryclean
veryclean: clean
	rm -f $(TARGET)
