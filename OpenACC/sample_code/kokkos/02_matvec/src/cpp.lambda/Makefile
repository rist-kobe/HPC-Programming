# Two environment variables, KOKKOS_ROOT and KOKKOS_LIBDIR, must be set.
# * KOKKOS_ROOT  : Install directory of Kokkos
# * KOKKOS_LIBDIR: Location of libkokkoscore.so. Typically, "KOKKOS_ROOT/lib64".
KOKKOS_ROOT ?= ../../../00_install
KOKKOS_ROOT := $(KOKKOS_ROOT)

KOKKOS_LIBDIR ?= $(KOKKOS_ROOT)/lib64
KOKKOS_LIBDIR := $(KOKKOS_LIBDIR)

DFLAGS ?= 
DFLAGS := $(DFLAGS)

CXX = ${KOKKOS_ROOT}/bin/nvcc_wrapper
CXXFLAGS = -g -O2 --c++17 -mp -tp=zen3  -v --extended-lambda #-Xcompiler -mp,-tp=zen3 #-v
LINKFLAGS = -g -O2 --c++17 -mp -tp=zen3 -v #-Xcompiler -mp=multicore,-tp=zen3 #-v
INC = -I$(KOKKOS_ROOT)/include
LIB = -L$(KOKKOS_LIBDIR) -lkokkoscore

CXXFLAGS += $(INC) $(DFLAGS)

SRC = main.cpp

OBJ = $(SRC:.cpp=.o)

EXE = run.x

.SUFFIXES: .cpp .o

$(EXE): $(OBJ)
	$(CXX) $(LINKFLAGS) -o $@ $(OBJ) $(LIB)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

main.o: main.cpp

.PHONY:
clean:
	rm -f *.o $(EXE)
