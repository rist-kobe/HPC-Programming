## GNU 8.5.0, 9.3.1
CC=gcc
ARCHFLAGS=-mavx2 # amd64 (x86)
CFLAGS=-g -std=gnu99 -Wall $(ARCHFLAGS) -O3 -fopenmp -fstrict-aliasing
FVEC=-ftree-loop-vectorize -ftree-slp-vectorize -ftree-vectorize -fopt-info-all-vec
FNVEC=-fno-tree-loop-vectorize -fno-tree-slp-vectorize -fno-tree-vectorize
LIB=
INC=
CFLAGS0=-g -std=gnu99 -O0

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
	rm -f *.x 
