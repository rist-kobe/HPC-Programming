# Two environment variables, KOKKOS_ROOT and KOKKOS_LIBDIR, must be set.
# * KOKKOS_ROOT  : Install directory of Kokkos
# * KOKKOS_LIBDIR: Location of libkokkoscore.so. Typically, "KOKKOS_ROOT/lib64".
KOKKOS_ROOT ?= ../../../00_install
KOKKOS_ROOT := $(KOKKOS_ROOT)

KOKKOS_LIBDIR ?= $(KOKKOS_ROOT)/lib64
KOKKOS_LIBDIR := $(KOKKOS_LIBDIR)

export MPI_CXX=${KOKKOS_ROOT}/bin/nvcc_wrapper
export OMPI_CXX=${KOKKOS_ROOT}/bin/nvcc_wrapper
CC=mpicxx
CFLAGS=-g --c++17 -O0 -mp=multicore -tp=zen3 # -gpu=cc90,sm_90,cuda12.3 -tp=zen3  #H100
INC = -I$(KOKKOS_ROOT)/include  #-cuda
LIB = -L$(KOKKOS_LIBDIR) -lkokkoscore
CFLAGS += $(INC)
LINKOPT=$(CFLAGS) #-cuda 

.SUFFIXES: .o .cpp

SRC = main.cpp

OBJ = $(SRC:.cpp=.o)

EXE=run.x

$(EXE): $(OBJ)
	$(CC) $(LINKOPT) -o $@ $(OBJ) $(LIB)

main.o: main.cpp

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:
clean:
	rm -f *.o 

.PHONY:
veryclean: clean
	rm -f $(EXE)
