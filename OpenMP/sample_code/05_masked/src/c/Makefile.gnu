## GNU 14.1.0 and 12.2.0 (miniforge3 for x86_64)
CC = gcc
DFLAGS := $(DFLAGS)
#DFLAGS := -DNOT_USE_MASKED # for GNU 8.5.0
CFLAGS = -g -fopenmp -O0 $(DFLAGS)

.SUFFIXES: .c .o

SRC=main.c

run.x: main.o
	$(CC) $(CFLAGS) main.o -o $@ 

.c.o:
	$(CC) $(CFLAGS) -c $< -o  $@ 

main.o: main.c

.PHONY:
clean:
	rm -f *.o run.x
