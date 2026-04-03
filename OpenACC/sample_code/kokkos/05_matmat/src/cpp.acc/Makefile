DFLAGS ?= 
DFLAGS := $(DFLAGS)

CXX = nvc++ 
CXXFLAGS = -g -O3 --c++17 -acc=gpu -tp=zen3 -gpu=cc90,sm_90,cuda12.3 -Minfo=accel #-v -Xcompiler -mp,-tp=zen3
LINKFLAGS = -g -O3 --c++17 -acc=gpu -cuda -gpu=cc90,sm_90,cuda12.3 -cudalib=cublas #-v #-Xcompiler -mp=multicore,-tp=zen3 #-v
INC = -cuda 
LIB = 

CXXFLAGS += $(INC) $(DFLAGS)

CUDA_CC=nvcc
CUDA_CFLAGS=-O2 -arch=sm_90 #H100

SRC = main.cpp mykernel.cpp timer.cpp

OBJ = $(SRC:.cpp=.o)

EXE = run.x

.SUFFIXES: .o .cpp .cu

$(EXE): mykernel_cuda.o $(OBJ) 
	$(CXX) $(LINKFLAGS) -o $@ $(OBJ) mykernel_cuda.o $(LIB)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.cu.o:
	$(CUDA_CC) $(CUDA_CFLAGS) -c $< -o $@

main.o: main.cpp mykernel.o timer.o

mykernel.o: mykernel.cpp 

mykernel_cuda.o: mykernel_cuda.cu

timer.o: timer.cpp 

.PHONY:
clean:
	rm -f *.o $(EXE)
