CC=gcc
CFLAGS=-g -O0 -std=c99 -fopenmp

.SUFFIXES: .c .o

run.x: main.o
	$(CC) $(CFLAGS) main.o -o run.x

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ 

.PHONY:
clean:
	rm -f *.o *.x
