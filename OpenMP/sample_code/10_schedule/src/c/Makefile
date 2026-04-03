CC=gcc
CCFLAGS=-g -O2 -fopenmp 
LIB=-lm

.SUFFIXES: .c .o

run.x: main.o
	$(CC) $(CCFLAGS) main.o $(LIB) -o run.x

.c.o:
	$(CC) $(CCFLAGS) -c $< -o $@ 

.PHONY:
clean:
	rm -f *.o  *.x
