CC=gcc
CFLAGS=-g -O0 -std=c99

.SUFFIXES: .c .o

run.x: serial.o
	$(CC) $(CFLAGS) serial.o -o run.x

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ 

.PHONY: clean
clean:
	rm -f *.o *.x
