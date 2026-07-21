CC=gcc
CCFLAGS=-g -std=c99 -O2 -fopenmp #-DUSE_STATICMEM

.SUFFIXES: .c .o

EXE=run.x

OBJS= main.o memory.o

$(EXE): $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) -o $(EXE)

main.o: memory.o main.c

main.o: memory.c

.c.o:
	$(CC) $(CCFLAGS) -c $< -o $@

.PHONY:
clean:
	rm -f *.o $(EXE)
