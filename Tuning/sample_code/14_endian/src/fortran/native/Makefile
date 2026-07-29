COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
## GNU  (8.5.0)
FC = gfortran
FFLAGS = -g -Wall -O0 -fconvert=native
###############################################
endif

TARGET = run.x

SRC = \
	main.f90

OBJS = $(SRC:.f90=.o)

.SUFFIXES: .f90 .o

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(FC) $(FFLAGS) $(OBJS) -o $@

.f90.o:
	$(FC) $(FFLAGS) -c $< -o $@

main.o: main.f90

.PHONY: clean
clean:
	rm -f *.f90~ *.o *.mod *~

.PHONY: veryclean
veryclean: clean
	rm -f $(TARGET)

