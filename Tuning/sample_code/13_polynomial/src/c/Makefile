COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
## GNU 8.5.0, 9.3.1
CC=gcc
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
#ARCHFLAGS=-arch arm64 -mcpu=apple-a13 # MacM1 (Arm64)
CFLAGS=-g -std=gnu99 $(ARCHFLAGS) -O3 -ffast-math -funroll-loops -ftree-vectorize -ftree-loop-vectorize -fopt-info-all=optinfo.all 
##########################################################################
else ifeq ($(COMPILER),intel)
## Intel DPC++/C++ (Intel oneAPI 2022.3.1)
CC=icx
ARCHFLAGS=-mavx2 # x86, AVX512
CFLAGS=-g -std=gnu99 $(ARCHFLAGS) -O3 -fvectorize -fslp-vectorize -fstrict-aliasing -ansi-alias -qopt-report #-fsave-optimization-record 
##########################################################################
else ifeq ($(COMPILER),clang)
## LLVM (13.0.0)
CC=clang
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
#ARCHFLAGS=-arch arm64 -mcpu=apple-a13 # MacM1 (Arm64)
CFLAGS=-g -std=gnu99 $(ARCHFLAGS) -O3 -fstrict-aliasing -ffast-math -funroll-loops \
	-fvectorize -fslp-vectorize -fsave-optimization-record -Rpass=".*loop.*" -Rpass-missed=".*" 
##########################################################################
else ifeq ($(COMPILER),intel-classic)
## Intel oneAPI 2022.3.1
CC=icc
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
CFLAGS=-g -std=gnu99 $(ARCHFLAGS) -O3 -fstrict-aliasing -restrict -diag-disable=10441 \
	-qopt-report=5 -qopt-report-phase=vec
##########################################################################
else ifeq ($(COMPILER),fjclang)
# Fujitsu Clang (4.10.0)
CC=fccpx 
#CC=fcc # for native compiler
ARCHFLAGFS=-march=armv8.3-a+sve -mcpu=a64fx -msve-vector-bits=512  # aarch64 (arm64), SVE (fixed 512-bit)
CFLAGS=-Nclang -g -std=gnu99 $(ARCHFLAGS) -Ofast -fno-omit-frame-pointer -fvectorize -fslp-vectorize \
	-ffj-no-interleave-loop-insns -ffj-swp -fstrict-aliasing -ffj-lst=t -ffj-line \
	-Rpass=".*loop.*|.*pipeline.*" -Rpass-missed=".*loop.*|.*pipeline.*" #-Rpass-analysis=".*"
#ARCHFLAGFS=-march=armv8.3-a+sve -mcpu=a64fx -msve-vector-bits=scalable  # aarch64 (arm64), SVE (vector-length Agnostic)
#CFLAGS=-Nclang -g -std=gnu99 $(ARCHFLAGS) -Ofast -fno-omit-frame-pointer -fvectorize -fslp-vectorize \
	-ffj-interleave-loop-insns -ffj-no-swp -fstrict-aliasing -ffj-lst=t -ffj-line \
	-Rpass=".*loop.*" -Rpass-missed=".*loop.*" #-Rpass-analysis=".*"
##########################################################################
endif

SRC= \
     main.c mykernel.c

OBJS = \
       $(SRC:.c=.o)

.SUFFIXES: .c .o

run.x: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

main.o: mykernel.o mykernel.h main.c

.PHONY: clean
clean:
	rm -f *.o *~ optinfo.all

.PHONY: veryclean
veryclean: clean
	rm -f *.x
