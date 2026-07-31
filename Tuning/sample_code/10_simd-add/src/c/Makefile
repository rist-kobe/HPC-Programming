COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
## GNU 8.5.0, 9.3.1
CC=gcc
ARCHFLAGS=-mavx2 # amd64 (x86)
CFLAGS=-g -std=gnu99 -Wall $(ARCHFLAGS) -O3 -fopenmp -fstrict-aliasing
FVEC=-ftree-loop-vectorize -ftree-slp-vectorize -ftree-vectorize -fopt-info-all-vec
FNVEC=-fno-tree-loop-vectorize -fno-tree-slp-vectorize -fno-tree-vectorize
LIB=
INC=
CFLAGS0=-g -std=gnu99 -O0
else ifeq ($(COMPILER),intel)
## Intel DPC++/C++ (Intel oneAPI 2022.3.1)
CC=icx
ARCHFLAGS=-mavx2 # amd64 (x86)
CFLAGS=-g -std=gnu99 -Wall $(ARCHFLAGS) -O3 -fopenmp -fstrict-aliasing -ansi-alias
FVEC=-fvectorize -fslp-vectorize -qopt-report #-fsave-optimization-record 
FNVEC=-fno-vectorize -fno-slp-vectorize  
LIB=
INC=
CFLAGS0=-g -std=gnu99 -O0
else ifeq ($(COMPILER),clang)
# LLVM 13.0.0
CC=clang
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
#ARCHFLAGS=-mcpu=thunderx2t99 # ThunderX2 (Arm64)
CFLAGS=-g -std=gnu99 -Wall $(ARCHFLAGS) -O3 -fopenmp -fstrict-aliasing 
FVEC=-fvectorize -fslp-vectorize -fsave-optimization-record -Rpass=".*loop.*" -Rpass-missed=".*loop.*" #-Rpass-analysis=".*loop.*|regalloc"
FNVEC=-fno-vectorize -fno-slp-vectorize
LIB=
INC=
CFLAGS0=-g -std=gnu99 -O0
else ifeq ($(COMPILER),appleclang)
# Apple clang 15.0.0
CC=clang
ARCHFLAGS=-arch arm64 -mcpu=apple-m1 # MacM1 (Arm64)
CFLAGS=-g -std=gnu99 -Wall $(ARCHFLAGS) -O3 -fstrict-aliasing 
FVEC=-fvectorize -fslp-vectorize -fsave-optimization-record -Rpass=".*loop.*" -Rpass-missed=".*loop.*" #-Rpass-analysis=".*loop.*|regalloc"
FNVEC=-fno-vectorize -fno-slp-vectorize
LIB=
INC=
CFLAGS0=-g -std=gnu99 -O0
else ifeq ($(COMPILER),intel-classic)
## Intel oneAPI 2022.3.1
CC=icc
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
CFLAGS=-g -std=gnu99 -Wall $(ARCHFLAGS) -O3 -qopenmp -fstrict-aliasing -restrict -diag-disable=10441
FVEC=-qopt-report=5 -qopt-report-phase=vec
FNVEC=-no-vec -no-simd
LIB=
INC=
CFLAGS0=-g -std=gnu99 -O0 -diag-disable=10441
endif

.SUFFIXES: .o .c

all: run_v.x run_nv.x

run_v.x: timer.o mykernel_v.o dummy.o main.o
	$(CC) $(CFLAGS) $(INC) timer.o mykernel_v.o dummy.o main.o $(LIB) -o $@	

run_nv.x: timer.o mykernel_nv.o dummy.o main.o
	$(CC) $(CFLAGS) $(INC) timer.o mykernel_nv.o dummy.o main.o $(LIB) -o $@	

dummy.o: dummy.c

main.o: mykernel.h dummy.h timer.h main.c

mykernel_v.o: mykernel.c
	$(CC) $(CFLAGS) $(INC) $(FVEC) -c mykernel.c -o mykernel_v.o 

mykernel_v.s: mykernel.c
	$(CC) $(CFLAGS) $(INC) $(FVEC) -S -c mykernel.c -o mykernel_v.s 

mykernel_nv.o: mykernel.c
	$(CC) $(CFLAGS) $(INC) $(FNVEC) -c mykernel.c -o mykernel_nv.o 

mykernel_nv.s: mykernel.c
	$(CC) $(CFLAGS) $(INC) $(FNVEC) -S -c mykernel.c -o mykernel_nv.s 

timer.o:
	$(CC) $(CFLAGS0) -c timer.c -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *~ *.o 

.PHONY: veryclean
veryclean: clean
	rm -f *.x *.yaml *.optrpt
