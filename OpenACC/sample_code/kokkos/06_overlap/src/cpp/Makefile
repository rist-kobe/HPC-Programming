# Two environment variables, KOKKOS_ROOT and KOKKOS_LIBDIR, must be set.
# * KOKKOS_ROOT  : Install directory of Kokkos
# * KOKKOS_LIBDIR: Location of libkokkoscore.so. Typically, "KOKKOS_ROOT/lib64".
KOKKOS_ROOT ?= ../../../00_install
KOKKOS_ROOT := $(KOKKOS_ROOT)

KOKKOS_LIBDIR ?= $(KOKKOS_ROOT)/lib64
KOKKOS_LIBDIR := $(KOKKOS_LIBDIR)

DFLAGS ?= 
DFLAGS := $(DFLAGS)

CC=${KOKKOS_ROOT}/bin/nvcc_wrapper
CFLAGS=-g --c++17 -O3 -mp=multicore -tp=zen3 --extended-lambda #-Minfo=opt,vect,accel 
INC = -I$(KOKKOS_ROOT)/include 
LIB = -L$(KOKKOS_ROOT)/lib64 -lkokkoscore

CFLAGS += $(INC) $(DFLAGS) -DUSE_FP_DP
##########################################################################
SRC = \
	main.cpp \

OBJ = $(SRC:.cpp=.o)

.SUFFIXES: .cpp .o

run.x: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LIB)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.cpp

.PHONY: clean
clean:
	rm -f *.o *~ *.lst

.PHONY: veryclean
veryclean: clean
	rm -f *.x
