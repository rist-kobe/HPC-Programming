CC=nvfortran
CFLAGS=-g -O0 -mp=gpu -gpu=cc90,cuda12.3 -Minfo=accel,mp

.SUFFIXES: .F90 .o

run.x: main.o 
	$(CC) $(CFLAGS) main.o -o run.x

main.o: main.F90

.F90.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *.mod *.x
