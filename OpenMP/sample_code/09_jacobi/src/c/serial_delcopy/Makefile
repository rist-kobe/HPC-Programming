CC=gcc
CCFLAGS=-g -std=c99 -O2 -fopenmp

.SUFFIXES: .c .o

EXE=run.x

OBJS= main.o 

$(EXE): main.o
	$(CC) $(CCFLAGS) $(OBJS) -o $(EXE)

main.o: main.c

.c.o:
	$(CC) $(CCFLAGS) -c $< -o $@

.PHONY:
clean:
	rm -f *.o $(EXE)
