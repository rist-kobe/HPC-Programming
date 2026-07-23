## Nvidia HPC SDK 
CC=nvc
CFLAGS=-g -Wall -c99 -O3 -mp=gpu -Minfo=mp,accel
LIB=
INC=
## 23.11, Xeon(Icelake) + NVIDIA A100
CFLAGS += $(INC) -tp=icelake -gpu=cc80,cuda12.3,ptxinfo,pinned 
## 22.2,  AMD EPYC Zen2 + NVIDIA RTX A5000
#CFLAGS += $(INC) -tp=zen2 -gpu=cc86,cuda11.6,ptxinfo,pinned

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
