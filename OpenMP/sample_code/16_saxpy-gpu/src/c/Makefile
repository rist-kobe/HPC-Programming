## Nvidia HPC SDK 
CC=nvc
## Offload mode: omp (OpenMP target offload), acc (OpenACC), nogpu (host only)
OFFLOAD ?= omp
## Target CPU type (e.g., TP=zen2 for AMD EPYC Zen2)
TP ?= icelake
CFLAGS=-g -Wall -c99 -O3
LIB=
INC=
CFLAGS += $(INC) -tp=$(TP)

ifeq ($(OFFLOAD),omp)
  ## OpenMP GPU offload build
  CFLAGS += -mp=gpu -Minfo=mp,accel
  ## 23.11, Xeon(Icelake) + NVIDIA A100
  CFLAGS += -gpu=cc80,cuda12.3,ptxinfo,pinned
  ## 22.2,  AMD EPYC Zen2 + NVIDIA RTX A5000
  #CFLAGS += -gpu=cc86,cuda11.6,ptxinfo,pinned
else ifeq ($(OFFLOAD),acc)
  ## OpenACC GPU offload build
  CFLAGS += -acc=gpu -Minfo=accel
  ## 23.11, Xeon(Icelake) + NVIDIA A100
  CFLAGS += -gpu=cc80,cuda12.3,ptxinfo,pinned
  ## 22.2,  AMD EPYC Zen2 + NVIDIA RTX A5000
  #CFLAGS += -gpu=cc86,cuda11.6,ptxinfo,pinned
else ifeq ($(OFFLOAD),nogpu)
  ## Host-only build (no GPU offloading)
  CFLAGS += -Minfo
else
  $(error Unknown OFFLOAD=$(OFFLOAD) (expected omp, acc, or nogpu))
endif

.SUFFIXES: .o .c

all: run.x

run.x: mykernel.o main.o timer.o
	$(CC) $(CFLAGS) mykernel.o main.o timer.o $(LIB) -o $@	

main.o: main.c

mykernel.o: mykernel.c

timer.o: timer.c

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *~ *.o *.x
