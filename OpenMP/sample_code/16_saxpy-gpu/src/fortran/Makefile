## Nvidia HPC SDK
FC=nvfortran
## Offload mode: omp (OpenMP target offload), acc (OpenACC), nogpu (host only)
OFFLOAD ?= omp
## Target CPU type (e.g., TP=zen2 for AMD EPYC Zen2)
TP ?= icelake
FCFLAGS=-g -Wall -O3
LIB=
INC=
FCFLAGS += $(INC) -tp=$(TP)

ifeq ($(OFFLOAD),omp)
  ## OpenMP GPU offload build
  FCFLAGS += -mp=gpu -Minfo=accel,mp
  ## 23.11, Xeon(Icelake) + NVIDIA A100
  FCFLAGS += -gpu=cc80,cuda12.3,ptxinfo,pinned
  ## 22.2,  AMD EPYC Zen2 + NVIDIA RTX A5000
  #FCFLAGS += -gpu=cc86,cuda11.6,ptxinfo,pinned
else ifeq ($(OFFLOAD),acc)
  ## OpenACC GPU offload build
  FCFLAGS += -acc=gpu -Minfo=accel
  ## 23.11, Xeon(Icelake) + NVIDIA A100
  FCFLAGS += -gpu=cc80,cuda12.3,ptxinfo,pinned
  ## 22.2,  AMD EPYC Zen2 + NVIDIA RTX A5000
  #FCFLAGS += -gpu=cc86,cuda11.6,ptxinfo,pinned
else ifeq ($(OFFLOAD),nogpu)
  ## Host-only build (no GPU offloading)
  FCFLAGS += -Minfo
else
  $(error Unknown OFFLOAD=$(OFFLOAD) (expected omp, acc, or nogpu))
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
