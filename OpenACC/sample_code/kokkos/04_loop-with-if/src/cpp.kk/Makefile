# Two environment variables, KOKKOS_ROOT and KOKKOS_LIBDIR, must be set.
# * KOKKOS_ROOT  : Install directory of Kokkos
# * KOKKOS_LIBDIR: Location of libkokkoscore.so. Typically, "KOKKOS_ROOT/lib64".
KOKKOS_ROOT ?= ../../../00_install
KOKKOS_ROOT := $(KOKKOS_ROOT)

KOKKOS_LIBDIR ?= $(KOKKOS_ROOT)/lib64
KOKKOS_LIBDIR := $(KOKKOS_LIBDIR)

CC=${KOKKOS_ROOT}/bin/nvcc_wrapper
CFLAGS=-g --c++17 -O3 -mp=multicore -tp=zen3 -I$(KOKKOS_ROOT)/include  #-v --extended-lambda
LIB = -L$(KOKKOS_LIBDIR) -lkokkoscore

.SUFFIXES: .o .cpp

SRC = main.cpp mykernel_kk.cpp timer.cpp

OBJ = $(SRC:.cpp=.o)

EXE=run.x

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIB)

mykernel_kk.o: mykernel_kk.cpp

main.o: mykernel_kk.o timer.o main.cpp

timer.o: timer.cpp

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:
clean:
	rm -f *.o *.lst

.PHONY:
veryclean: clean
	rm -f $(EXE)
