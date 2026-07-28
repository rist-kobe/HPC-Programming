COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
## GNU 8.5.0, 9.3.1
CC=gcc
ARCHFLAGS=-mavx2 # x86 with AVX2
COPT=-O2 -fopt-info-all=optinfo.all 
LIB= 
INC=
CFLAGS=-g -Wall -std=gnu11 $(ARCHFLAGS) $(COPT) $(INC) 

CFLAGS_O0=-g -O0 -std=gnu11 $(INC)
else ifeq ($(COMPILER),intel)
## Intel DPC++/C++ (Intel oneAPI 2022.3.1)
CC=icx
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
COPT=-O2 -qopt-report 
LIB= 
INC=
CFLAGS=-g -Wall -std=gnu11 $(ARCHFLAGS) $(COPT) $(INC) 

CFLAGS_O0=-g -O0 -std=gnu11 $(INC)
else ifeq ($(COMPILER),clang)
## LLVM 13.0.0
CC=clang
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
#ARCHFLAGS=-mcpu=thunderx2t99 # ThunderX2 (Arm64)
#ARCHFLAGS=-arch arm64 -mcpu=apple-a13 # MacM1 (Arm64)
COPT=-O2 -fsave-optimization-record -Rpass=".*loop.*" -Rpass-missed=".*loop.*"
LIB= 
INC=
CFLAGS=-g -Wall -std=gnu11 $(ARCHFLAGS) $(COPT) $(INC) 

CFLAGS_O0=-g -O0 -std=gnu11 $(INC)
else ifeq ($(COMPILER),intel-classic)
## Intel oneAPI 2022.3.1
CC=icc
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
#ARCHFLAGS=-xCORE_AVX512 # xeon (with CORE-AVX512)
COPT=-O2 -qopt-report=2
LIB= 
INC=
CFLAGS=-g -Wall -std=gnu11 $(ARCHFLAGS) $(COPT) $(INC) -diag-disable=10441

CFLAGS_O0=-g -O0 -std=gnu11 $(INC)
endif

SRC = \
	main.c     \
	mykernel.c

OBJ  = $(SRC:.c=.o)

.SUFFIXES: .o .c

all: run.x

run.x: timer.o $(OBJ)
	$(CC) $(CFLAGS) timer.o $(OBJ) $(LIB) -o $@	

main.o: mykernel.o main.c

mykernel.o: mykernel.c

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

timer.o:
	$(CC) $(CFLAGS_O0) -c timer.c -o $@

.PHONY:
clean:
	rm -f *~ *.o *optinfo*

.PHONY:
veryclean: clean
	rm -f *.x
