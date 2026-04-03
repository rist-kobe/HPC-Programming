CC=nvc
CFLAGS=-g -O0 -acc=gpu -gpu=cc90,sm_90,cuda12.3,pinned -Minfo=accel

.SUFFIXES: .c .o

run.x: main.o 
	$(CC) $(CFLAGS) main.o -o run.x

main.o: main.c

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *.x
