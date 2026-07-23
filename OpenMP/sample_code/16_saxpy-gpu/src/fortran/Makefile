## Nvidia HPC SDK
FC=nvfortran
## Set GPU=0 for a CPU-only build (OpenMP threading stays enabled)
GPU ?= 1
## Target CPU type (e.g., TP=zen2 for AMD EPYC Zen2)
TP ?= icelake
FCFLAGS=-g -Wall -O3
LIB=
INC=
FCFLAGS += $(INC) -tp=$(TP)

ifeq ($(GPU),1)
  ## GPU offload build
  FCFLAGS += -mp=gpu -Minfo=accel,mp
  ## 23.11, Xeon(Icelake) + NVIDIA A100
  FCFLAGS += -gpu=cc80,cuda12.3,ptxinfo,pinned
  ## 22.2,  AMD EPYC Zen2 + NVIDIA RTX A5000
  #FCFLAGS += -gpu=cc86,cuda11.6,ptxinfo,pinned
else
  ## CPU-only build: OpenMP remains enabled, no GPU offloading
  FCFLAGS += -mp -Minfo=mp
endif

.SUFFIXES: .o .F90 .c

all: run.x

run.x: mytype.o mykernel.o timer.o main.o
	$(FC) $(FCFLAGS) -O0 mytype.o mykernel.o timer.o main.o $(LIB) -o $@	

main.o: timer.o mytype.o mykernel.o main.F90

mykernel.o: mytype.o mykernel.F90

mytype.o: mytype.F90

timer.o: mytype.o timer.F90

.F90.o:
	$(FC) $(FCFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *~ *.o *.mod *.x
