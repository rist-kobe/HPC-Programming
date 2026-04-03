CC=gcc
CCFLAGS=-g -fopenmp -O0

.SUFFIXES: .c .o

SRC=main.c

run.x: main.o
	$(CC) $(CCFLAGS) main.o -o $@ 

.c.o:
	$(CC) $(CCFLAGS) -c $< -o  $@ 

main.o: main.c

.PHONY:
clean:
	rm -f *.o run.x
