CC=nvc
CFLAGS=-g -O0 -mp=gpu -gpu=cc90,cuda12.3 -Minfo=accel,mp

.SUFFIXES: .c .o

run.x: main.o 
	$(CC) $(CFLAGS) main.o -o run.x

main.o: main.c

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *.x
