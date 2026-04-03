CC=gcc
CCFLAGS=-g -fopenmp -O0

SRC=main.c

OBJ=main.o

.SUFFIXES: .c .o

run.x: $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) -o $@

main.o: main.c

.c.o:
	$(CC) $(CCFLAGS) -c $< -o $@

.PHONY:
clean:
	rm -f *.o run.x
