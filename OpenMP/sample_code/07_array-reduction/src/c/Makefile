CC=gcc
CFLAGS=-g -Wall -O0 -fopenmp

.SUFFIXES: .c .o

run.x: main.o 
	$(CC) $(CFLAGS) main.o -o run.x

main.o: main.c

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *.x
