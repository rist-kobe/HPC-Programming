## Nvidia HPC SDK 
CC=nvc
## Set GPU=0 for a CPU-only build (OpenMP threading stays enabled)
GPU ?= 1
CFLAGS=-g -Wall -c99 -O3
LIB=
INC=
CFLAGS += $(INC) -tp=icelake

ifeq ($(GPU),1)
  ## GPU offload build
  CFLAGS += -mp=gpu -Minfo=mp,accel
  ## 23.11, Xeon(Icelake) + NVIDIA A100
  CFLAGS += -gpu=cc80,cuda12.3,ptxinfo,pinned
  ## 22.2,  AMD EPYC Zen2 + NVIDIA RTX A5000
  #CFLAGS += -tp=zen2 -gpu=cc86,cuda11.6,ptxinfo,pinned
else
  ## CPU-only build: OpenMP remains enabled, no GPU offloading
  CFLAGS += -mp -Minfo=mp
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
