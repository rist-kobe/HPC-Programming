COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
## GNU 8.5.0, 9.3.1
CC=g++
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
#ARCHFLAGS=-arch arm64 -mcpu=apple-a13 # MacM1 (Arm64)
CFLAGS=-g -std=gnu++11 $(ARCHFLAGS) -O3 -ffast-math -funroll-loops -ftree-vectorize -ftree-loop-vectorize -fopt-info-all=optinfo.all 
###################################################################################
else ifeq ($(COMPILER),intel)
## Intel DPC++/C++ (Intel oneAPI 2022.3.1)
CC=icpx
ARCHFLAGS=-mavx2 # x86, AVX512
CFLAGS=-g -std=gnu++11 $(ARCHFLAGS) -O3 -fvectorize -fslp-vectorize -fstrict-aliasing -ansi-alias -qopt-report #\
	-fsave-optimization-record 
###################################################################################
else ifeq ($(COMPILER),clang)
## LLVM (13.0.0), Apple clang 15.0.0
CC=clang++
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
#ARCHFLAGS=-arch arm64 -mcpu=apple-m1 # MacM1 (Arm64)
CFLAGS=-g -std=gnu++11 $(ARCHFLAGS) -O3 -fstrict-aliasing -ffast-math -funroll-loops -fvectorize -fslp-vectorize \
	-fsave-optimization-record -Rpass=".*loop.*" -Rpass-missed=".*loop.*" #-Rpass-analysis=".*loop.*"
###################################################################################
else ifeq ($(COMPILER),intel-classic)
## Intel oneAPI 2022.3.1
CC=icpc
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
CFLAGS=-g -std=gnu++11 $(ARCHFLAGS) -O3 -fstrict-aliasing -restrict -diag-disable=10441 \
	-qopt-report=5 -qopt-report-phase=vec
###################################################################################
else ifeq ($(COMPILER),fjclang)
# Fujitsu Clang (4.10.0)
CC=FCCpx 
#CC=FCC # for native compiler
ARCHFLAGFS=-march=armv8.3-a+sve -mcpu=a64fx -msve-vector-bits=512  # aarch64 (arm64), SVE (fixed 512-bit)
CFLAGS=-Nclang -g -std=gnu++11 -stdlib=libstdc++ $(ARCHFLAGS) -Ofast -fno-omit-frame-pointer -fvectorize -fslp-vectorize \
	-ffj-no-interleave-loop-insns -ffj-swp -fstrict-aliasing -ffj-lst=t -ffj-line \
	-Rpass=".*vectorize.*|.*pipeline.*" -Rpass-missed=".*vectorize.*|.*pipeline.*" #\
	-Rpass-analysis=".*vectorize.*|.*pipeline.*"
#ARCHFLAGFS=-march=armv8.3-a+sve -mcpu=a64fx -msve-vector-bits=scalable  # aarch64 (arm64), SVE (vector-length Agnostic)
#CFLAGS=-Nclang -g -std=gnu++11 -stdlib=libstdc++ $(ARCHFLAGS) -Ofast -fno-omit-frame-pointer -fvectorize -fslp-vectorize \
	-ffj-interleave-loop-insns -ffj-no-swp -fstrict-aliasing -ffj-lst=t -ffj-line \
	-Rpass=".*vectorize.*|.*pipeline.*" -Rpass-missed=".*vectorize.*|.*pipeline.*" #\
	-Rpass-analysis=".*vectorize.*|.*pipeline.*"
###################################################################################
endif

SRC= \
     main.cc \
     mykernel.cc

OBJS = \
       $(SRC:.cc=.o)

.SUFFIXES: .cc .o

run.x: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB) -o $@

.cc.o:
	$(CC) $(CFLAGS) -c $< $(LIB) -o $@

main.o: mykernel.o main.cc vector_oo.h

mykernel.o: mykernel.cc vector_oo.h

.PHONY: clean
clean:
	rm -f *.o *~

.PHONY: veryclean
veryclean: clean
	rm -f *.x
